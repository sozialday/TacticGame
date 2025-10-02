// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponentBase.h"

// Sets default values for this component's properties
UInventoryComponentBase::UInventoryComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponentBase::BeginPlay()
{
	Super::BeginPlay();

	m_EquippedWeaponSlot = Weapon;
	
	for (const auto& ItemSingle : ItemsInInventory)
	{
		AddItem(ItemSingle);
	}
}

void UInventoryComponentBase::AddItem(TSubclassOf<class AItemBase> Item)
{
	if (Item == nullptr) return;

	int32& Count = Items.FindOrAdd(Item);
	Count++;
}