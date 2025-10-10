// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticGame_StageGamemodeBase.h"

#include "Components/SizeBox.h"

#include "Kismet/GameplayStatics.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "TacticGame/UserInput/CursorBase.h"
#include "TacticGame/Gameplay/Units/UnitCharacterBase.h"
#include "TacticGame/Gameplay/Vision/MinimapRendererBase.h"
#include "TacticGame/UserInterface/InStage/PlayerScreenBase.h"
#include "TacticGame/UserInterface/InStage/AfterMoveInteractionBase.h"
#include "TacticGame/Gameplay/Vision/Attachments/MinimapCameraTransformPointBase.h"


ATacticGame_StageGamemodeBase::ATacticGame_StageGamemodeBase()
{
	PrimaryActorTick.bCanEverTick = false;

	{
		static ConstructorHelpers::FClassFinder<UPlayerScreenBase> playerScreen_tmp
		(TEXT("/Game/UserInterface/InStage/WB_PlayerScreen"));

		if (playerScreen_tmp.Succeeded())
			PlayerScreen_ClassReferenceBlueprint = playerScreen_tmp.Class;
	}

	{
		static ConstructorHelpers::FClassFinder<UAfterMoveInteractionBase> aftermoveInteraction_tmp
		(TEXT("/Game/UserInterface/InStage/WB_AfterMoveInteraction"));

		if (aftermoveInteraction_tmp.Succeeded())
			AfterMoveInteraction_ClassReferenceBlueprint = aftermoveInteraction_tmp.Class;
	}

	{
		static ConstructorHelpers::FClassFinder<UUserWidget> minimap_tmp
		(TEXT("/Game/UserInterface/InStage/Overlays/WB_Minimap"));

		if (minimap_tmp.Succeeded())
			MinimapWidget_ClassReferenceBlueprint = minimap_tmp.Class;
	}
}

void ATacticGame_StageGamemodeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// when the player got spawned in successfully

	PlayerScreen = CreateWidget<UPlayerScreenBase>(GetWorld(), PlayerScreen_ClassReferenceBlueprint);
	if (PlayerScreen)
		PlayerScreen->AddToViewport();
}

// creates and displays a ui element that allows the player to choose what to do after moving
void ATacticGame_StageGamemodeBase::ShowAfterMovementActionUIElement(TObjectPtr<class AUnitCharacterBase> UnitReference)
{
	const auto& widget = CreateWidget<UAfterMoveInteractionBase>(GetWorld(), AfterMoveInteraction_ClassReferenceBlueprint);
	if (!IsValid(widget))
		return;

	const FVector2D WidgetPosition = widget->CalculateWidgetPositioning(UnitReference);
	widget->GetSizeBox_Container()->SetRenderTranslation(WidgetPosition);

	widget->AddToViewport();
}

// Tries to show the minimap on the playerscreen
void ATacticGame_StageGamemodeBase::ShowMinimapOnPlayerScreen()
{
	if (!IsValid(PlayerScreen))
		return;

	// spawns the Minimap Recorder Actor which contains the SceneCapture2D component
	SpawnMinimapRecorder();

	MinimapWidgetReference = CreateWidget<UUserWidget>(GetWorld(), MinimapWidget_ClassReferenceBlueprint);
	PlayerScreen->AttachMinimapToPlayerScreen(MinimapWidgetReference);
}
// Tries to hide the minimap on the playerscreen
void ATacticGame_StageGamemodeBase::HideMinimapOnPlayerScreen()
{
	if (!IsValid(PlayerScreen))
		return;

	if (!IsValid(MinimapWidgetReference))
		return;

	PlayerScreen->RemoveMinimapFromPlayerScreen(MinimapWidgetReference);
	
	// destroys the Minimap Renderer Actor
	if (IsValid(MinimapRendererReference))
		MinimapRendererReference->Destroy();
}

// Spawns the Minimap Recorder Actor which contains the SceneCapture2D component
void ATacticGame_StageGamemodeBase::SpawnMinimapRecorder()
{
	MinimapRendererReference = GetWorld()->SpawnActor<AMinimapRendererBase>(AMinimapRendererBase::StaticClass());
	if (!IsValid(MinimapRendererReference))
		return;

	MinimapRendererReference->SetCursorReference(Cast<ACursorBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACursorBase::StaticClass())));
	
	FTransform initialTransform = MinimapRendererReference->CalculateMinimapCameraTransform();
	initialTransform.SetRotation(MinimapRendererReference->DefaultMinimapRotation.Quaternion());

	MinimapRendererReference->SetActorTransform(initialTransform);
	MinimapRendererReference->EnableTracking();
}

// creates a widget that contains information regarding the minimap in fullscreen
void ATacticGame_StageGamemodeBase::RequestMinimap_Fullscreen()
{

}

// creates the camera manager for the minimap functionality
// Moves the Camera upwards to make the GameplayCameraBase function as a fullscreen minimap
void ATacticGame_StageGamemodeBase::CreateAndFillInMinimapCameraManager(TObjectPtr<class AActor> CursorReference, TObjectPtr<class AGameplayCameraBase> CameraReference)
{
	MinimapCameraReference = CameraReference;

	MinimapCameraTransformPointReference = GetWorld()->SpawnActor<AMinimapCameraTransformPointBase>(AMinimapCameraTransformPointBase::StaticClass());
	if (!MinimapCameraTransformPointReference)
		return;

	MinimapCameraTransformPointReference->SetActorLocation(CursorReference->GetActorLocation() + FVector(0, 0, 2000));

	CameraReference->SetTickingEnabled(false);
	MinimapCameraTransformPointReference->PassCameraReference(CameraReference);
	//	//
	MinimapCameraTransformPointReference->EnableCameraFollowing();
}

// destroys the camera manager for the minimap functionality
// returns the camera to normal functionality
void ATacticGame_StageGamemodeBase::DestroyMinimapCameraManager()
{
	if (!IsValid(MinimapCameraReference))
		MinimapCameraReference = 
			Cast<AGameplayCameraBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameplayCameraBase::StaticClass()));

	MinimapCameraReference->SetTickingEnabled(true);
	if (IsValid(MinimapCameraTransformPointReference))
	{
		MinimapCameraTransformPointReference->Destroy();
		MinimapCameraTransformPointReference = nullptr;
	}
}