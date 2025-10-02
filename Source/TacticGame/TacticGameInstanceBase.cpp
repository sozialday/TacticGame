// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticGameInstanceBase.h"
#include "TacticGame/UserInput/CursorPlayerController.h"

void UTacticGameInstanceBase::TryPlayForceFeedback(class ACursorPlayerController* PlayerController, class UForceFeedbackEffect* ForceFeedback)
{
	if (!PlayerController)
		return;

	if (!PlayerController->bCanPlayForceFeedback())
		return;

	PlayerController->ClientPlayForceFeedback(ForceFeedback);
}