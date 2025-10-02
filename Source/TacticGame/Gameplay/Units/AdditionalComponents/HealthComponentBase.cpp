// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponentBase.h"

// Sets default values for this component's properties
UHealthComponentBase::UHealthComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	CurrentHealth = MaxHealth;
}

void UHealthComponentBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

bool UHealthComponentBase::DecreaseHealth(int Amount)
{
	CurrentHealth -= Amount;
	if (CurrentHealth <= 0)
	{
		return true;
	}

	return false;
}

void UHealthComponentBase::IncreaseHealth(int Amount)
{
	if (CurrentHealth <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Already dead... no revives to fallen units"));
		return;
	}

	CurrentHealth += Amount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
}