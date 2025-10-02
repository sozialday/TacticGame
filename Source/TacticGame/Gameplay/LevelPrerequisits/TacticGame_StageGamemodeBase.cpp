// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticGame_StageGamemodeBase.h"

#include "Blueprint/UserWidget.h"
#include "TacticGame/UserInterface/InStage/PlayerScreenBase.h"

ATacticGame_StageGamemodeBase::ATacticGame_StageGamemodeBase()
{
	static ConstructorHelpers::FClassFinder<UPlayerScreenBase> playerScreen_tmp
	(TEXT("/Game/UserInterface/InStage/WB_PlayerScreen"));

	if (playerScreen_tmp.Succeeded())
		PlayerScreen_ClassReferenceBlueprint = playerScreen_tmp.Class;
}

void ATacticGame_StageGamemodeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// when the player got spawned in successfully

	PlayerScreen = CreateWidget<UPlayerScreenBase>(GetWorld(), PlayerScreen_ClassReferenceBlueprint);
	if (PlayerScreen)
		PlayerScreen->AddToViewport();
}