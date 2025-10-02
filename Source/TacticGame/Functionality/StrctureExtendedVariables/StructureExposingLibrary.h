// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StructureExposingLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API UStructureExposingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure)
	static class AUnitCharacterBase* GetSelectedUnitFromStructure(struct FUnitDetails TargetStructure);
};
