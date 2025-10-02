// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemWeapon_ClassContainer.generated.h"


UCLASS()
class TACTICGAME_API UItemWeapon_ClassContainer : public UObject
{
	GENERATED_BODY()
	
};

// Container for the Item [contains the actor reference data and more]
UCLASS()
class TACTICGAME_API UItemContainer : public UItemWeapon_ClassContainer
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TSubclassOf<class AItemBase> ItemToSpawn = nullptr;



public:

	void SetItem(TSubclassOf<class AItemBase> TargetItem)
	{
		ItemToSpawn = TargetItem;
	}
};

// Container for the Weapon [contains additional parameters such as the BaseDamage]
UCLASS()
class TACTICGAME_API UWeaponContainer : public UItemContainer
{
	GENERATED_BODY()


private:

	int BaseDamage;
};
