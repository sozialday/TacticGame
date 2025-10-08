// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticGame_StageGamemodeBase.h"

#include "Components/SizeBox.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Kismet/GameplayStatics.h"

#include "TacticGame/Gameplay/Units/UnitCharacterBase.h"
#include "TacticGame/UserInterface/InStage/PlayerScreenBase.h"
#include "TacticGame/UserInterface/InStage/AfterMoveInteractionBase.h"


ATacticGame_StageGamemodeBase::ATacticGame_StageGamemodeBase()
{
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