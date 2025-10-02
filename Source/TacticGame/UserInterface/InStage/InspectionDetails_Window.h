// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TacticGame/Gameplay/Units/UnitCharacterBase.h"
#include "InspectionDetails_Window.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API UInspectionDetails_Window : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void PassUnitCharacterDetails(FUnitDetails UnitDetails);
};
