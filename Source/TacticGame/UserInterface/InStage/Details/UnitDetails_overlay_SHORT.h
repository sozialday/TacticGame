// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TacticGame/Gameplay/Units/UnitCharacterBase.h"
#include "UnitDetails_overlay_SHORT.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API UUnitDetails_overlay_SHORT : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void PassUnitDetails(FUnitDetails UnitDetails);
};
