// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovementPath.generated.h"

UCLASS()
class TACTICGAME_API AMovementPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovementPath();

	UFUNCTION(BlueprintPure)
	USplineComponent* GetSplineComponent() const { return SplineComponent; }

private:

	// spline
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USplineComponent> SplineComponent;

public:

	void AddPoint(const FVector& Point);
	void AddPoint(const FVector& Point, const FVector& Tangent);
};
