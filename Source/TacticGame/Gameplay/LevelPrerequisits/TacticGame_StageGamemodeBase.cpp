// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticGame_StageGamemodeBase.h"

#include "Blueprint/UserWidget.h"

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
void ATacticGame_StageGamemodeBase::ShowAfterMovementActionUIElement()
{
	const auto& widget = CreateWidget<UAfterMoveInteractionBase>(GetWorld(), AfterMoveInteraction_ClassReferenceBlueprint);
	if (widget)
	{

		widget->AddToViewport();
	}
}