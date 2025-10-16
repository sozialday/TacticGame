// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpacerWidget.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API USpacerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeSpacerDimensions(FVector2D SpacerDimension);
};
