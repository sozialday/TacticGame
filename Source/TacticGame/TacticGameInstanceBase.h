// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TacticGameInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API UTacticGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void TryPlayForceFeedback(class ACursorPlayerController* PlayerController, class UForceFeedbackEffect* ForceFeedback);
};
