// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TransformationMatrixLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API UTransformationMatrixLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure)
	static FRotator RotateAlongHitSurface(FHitResult HitResult, FVector ForwardVector)
	{
		FVector ImpactNormal = HitResult.ImpactNormal;
		//	//
		FRotator NewRotation = FRotationMatrix::MakeFromZX(ImpactNormal, ForwardVector).Rotator();
		return NewRotation;
	}
};
