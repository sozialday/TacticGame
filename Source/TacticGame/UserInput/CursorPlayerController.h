// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "GameFramework/PlayerState.h"
#include "TacticGame/UserInput/Interfaces/DynamicInputMethodInterfaceBase.h"

#include "CursorPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API ACursorPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	// will return true if the player is using a gamepad
	bool bCanPlayForceFeedback()
	{
		/*const auto inputMethod = Cast<IDynamicInputMethodInterfaceBase>(PlayerState)->GetInput();
		return inputMethod == Controller ? true : false;*/
		return true;
	}
};
