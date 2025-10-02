// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LevelStateBase.generated.h"

// global enum which will store the current path selection type
UENUM(Blueprintable)
enum EPathSelectionType
{
	CustomPath,
	FastestPath
};


UCLASS()
class TACTICGAME_API ALevelStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
	ALevelStateBase()
	{
		
	}

private:

	// global Grid Size
	static float GridSize;

	// global Tags for Gameplay Pieces
	static FName GameplayPieces_Characters_Allies;
	static FName GameplayPieces_Characters_Enemies;

	static FName Gameplay_Ground;
	static FName Ignore_Gameplay_GridGeneration;

	static FLinearColor MovableCellColour;
	static FLinearColor UnMovableCellColour;

	// the current path selection method
	static TEnumAsByte<EPathSelectionType> m_pathSelectionMethod;

	// DEBUG ONLY // DEBUG ONLY // DEBUG ONLY // DEBUG ONLY // DEBUG ONLY
				// DEBUG ONLY // DEBUG ONLY // DEBUG ONLY // DEBUG ONLY // DEBUG ONLY
	// DEBUG ONLY // DEBUG ONLY // DEBUG ONLY // DEBUG ONLY // DEBUG ONLY

	static EDrawDebugTrace::Type DebugTraceVisibility;

	static int MaxLevel;

public:

	// for debug use only : for all tracing functions
	static EDrawDebugTrace::Type GetTraceVisibility()
	{
		return DebugTraceVisibility;
	}

	// retrieves the maximum level a unit can have
	UFUNCTION(BlueprintPure, Category = "Unit")
	static int GetMaxLevel() { return MaxLevel; }

	// sets the path selection type
	UFUNCTION(BlueprintCallable, Category = "PathFinding")
	static void SetPathSelectionType(TEnumAsByte<EPathSelectionType> PathSelectionType)
	{
		m_pathSelectionMethod = PathSelectionType;
	}

	// retrieves the current path selection type
	UFUNCTION(BlueprintPure, Category = "PathFinding")
	static TEnumAsByte<EPathSelectionType> GetPathSelectionType()
	{
		return m_pathSelectionMethod;
	}


	// retrieves the grid size for all classes to be used freely
	UFUNCTION(BlueprintPure, Category = "Level")
	static float GetLevelGridSize()
	{
		return GridSize;
	}

	// retrieves the grids dimension in x and y
	UFUNCTION(BlueprintPure, Category = "Level")
	static FVector2D GetCellDimension()
	{
		const float gridSize_halfed = GetLevelGridSize() / 2;
		return FVector2D(gridSize_halfed);
	}



	// Tag for ground pieces
	UFUNCTION(BlueprintPure)
	static FName GetTag_Gameplay_Ground() { return Gameplay_Ground; }

	// Tag getters
	// Allies
	UFUNCTION(BlueprintPure, Category = "Level")
	static FName GetTag_Characters_Allies() { return GameplayPieces_Characters_Allies; }

	// Tag getters
	// Enemies
	UFUNCTION(BlueprintPure, Category = "Level")
	static FName GetTag_Characters_Enemies() { return GameplayPieces_Characters_Enemies; }

	// Tag for ignoring pieces during grid generation
	UFUNCTION(BlueprintPure)
	static FName GetTag_IgnoreGameplayGridGeneration() { return Ignore_Gameplay_GridGeneration; }


	UFUNCTION(BlueprintPure, Category = "Level")
	static FLinearColor GetMovableCellColor() { return MovableCellColour; }

	UFUNCTION(BlueprintPure, Category = "Level")
	static FLinearColor GetUnMovableCellColor() { return UnMovableCellColour; }

public:

	// spawns a movement track containing all movement points for the character to follow
	static TObjectPtr<class AMovementPath> AddMovementTrackFilled(UWorld* WorldContext, FVector StartingLocation, TArray<FVector> Points);
};
