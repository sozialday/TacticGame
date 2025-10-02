// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitStatsComponent.h"

void UUnitStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (EnableUnitStatManipulation)
	{
		Attack = DEV_ONLY__UnitStats.Attack;
		Speed = DEV_ONLY__UnitStats.Speed;
		Luck = DEV_ONLY__UnitStats.Luck;
		Defense = DEV_ONLY__UnitStats.Defense;
		CriticalHitChance = DEV_ONLY__UnitStats.CriticalHitChance;
		Resistance = DEV_ONLY__UnitStats.Resistance;

		// should take the value of the save
		LevelProgression = FLevelProgression(DEV_ONLY__UnitLevel, 0.0);
	}
}

// Sets default values for this component's properties
UUnitStatsComponent::UUnitStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
