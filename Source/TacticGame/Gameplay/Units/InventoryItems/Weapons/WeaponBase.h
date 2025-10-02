// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TacticGame/Gameplay/Units/InventoryItems/ItemBase.h"
#include "TacticGame/Functionality/StrctureExtendedVariables/GlobalStructureContainer.h"
#include "WeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int BaseDamageAddition = 1;
};
