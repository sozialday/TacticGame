// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AfterMoveInteractionBase.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API UAfterMoveInteractionBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
	class USizeBox* GetSizeBox_Container() const;
};
