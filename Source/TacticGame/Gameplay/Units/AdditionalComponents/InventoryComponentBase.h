// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TacticGame/Gameplay/Units/InventoryItems/ItemBase.h"
#include "TacticGame/Gameplay/Units/InventoryItems/Weapons/WeaponBase.h"

#include "TacticGame/Gameplay/Units/InventoryItems/ItemWeapon_ClassContainer.h"
#include "InventoryComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICGAME_API UInventoryComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponentBase();
		
	virtual void BeginPlay() override;

	// Adds the Item to the Inventory
	UFUNCTION(BlueprintCallable)
	void AddItem(TSubclassOf<class AItemBase> Item);

	// Removes the Item from the Inventory
	UFUNCTION(BlueprintCallable)
	bool RemoveItem(TSubclassOf<class AItemBase> FromItem)
	{
		if (FromItem == nullptr) return false;

		int32* CountPtr = Items.Find(FromItem);
		if (!CountPtr) return false; // Klasse nicht vorhanden

		(*CountPtr)--;

		if (*CountPtr <= 0)
		{
			Items.Remove(FromItem); // letzte Instanz entfernt
		}

		return true; // erfolgreich genommen
	}


	UFUNCTION(BlueprintPure)
	TSubclassOf<class AWeaponBase> GetEquippedWeaponSlot() { return m_EquippedWeaponSlot; }

	UFUNCTION(BlueprintPure)
	TMap<TSubclassOf<class AItemBase>, int32> GetAllItems()
	{
		return Items;
	}

	// NOT FULLY FUNCTIONAL

	UFUNCTION(BlueprintPure)
	int GetItemIndexByName(FName TargetName)
	{
		return -1;
	}

	UFUNCTION(BlueprintCallable)
	void DecrementXPPoint()
	{
		LevelXP_Points--;
	}
	UFUNCTION(BlueprintCallable)
	void AddXPPoints(int PointsAdded = 1)
	{
		LevelXP_Points += PointsAdded;
	}

private:

	// this should only point to an element of the m_Items Array which also has to be of type WeaponBase
	UPROPERTY()
	TSubclassOf<class AWeaponBase> m_EquippedWeaponSlot = nullptr;

	// the equipped weapon should not be included in this array
	// string : contains the name of the item 
	// FItem : contains the Item and the Amount itself
	UPROPERTY()
	TMap<TSubclassOf<class AItemBase>, int32> Items;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "XP Points"))
	int LevelXP_Points = 0;

public:

	// if you want to select the weapon the unit currently is equipped with please do not add it to this array
	UPROPERTY(EditAnywhere, Category = "Item Inventory [Startup]")
	TArray<TSubclassOf<class AItemBase>> ItemsInInventory;

	UPROPERTY(EditAnywhere, Category = "Weapon Equipped Inventory [Startup]")
	TSubclassOf<class AWeaponBase> Weapon;
};
