// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponentBase.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICGAME_API UHealthComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponentBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentHealth;

	UFUNCTION(BlueprintCallable)
	bool DecreaseHealth(int Amount);

	UFUNCTION(BlueprintCallable)
	void IncreaseHealth(int Amount);

	bool isFull()
	{
		return CurrentHealth >= MaxHealth;
	}
	bool isEmpty()
	{
		return CurrentHealth <= 0;
	}

protected:

	virtual void BeginPlay() override;
};
