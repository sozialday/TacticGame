// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TacticGame/Gameplay/Units/InventoryItems/ItemBase.h"
#include "ConsumableItemBase.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API AConsumableItemBase : public AItemBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int HealthAddition = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TObjectPtr<class UHealthComponentBase> UnitHealthComp = nullptr;

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Consume Item"))
	bool BP_ConsumeItem();
};
