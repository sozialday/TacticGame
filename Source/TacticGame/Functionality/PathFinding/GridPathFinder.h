// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TacticGame/Gameplay/LevelPrerequisits/LevelStateBase.h"

/**
 * 
 */
class TACTICGAME_API GridPathFinder
{
public:

    GridPathFinder(float InSpacing);

    // Findet den kürzesten Weg zwischen Start und Ziel im Gitter
    TArray<FVector> FindPath(const TArray<FVector>& Points, const FVector& Start, const FVector& Goal, bool& Success);

private:
    
    // retrieves the value of one cell
    static int OneCell()
    {
        return ALevelStateBase::GetLevelGridSize();
    }

    float Spacing;
    FVector Origin;

    FIntVector FloatToIndex(const FVector& P) const;
    FVector IndexToFloat(const FIntVector& Idx) const;
};
