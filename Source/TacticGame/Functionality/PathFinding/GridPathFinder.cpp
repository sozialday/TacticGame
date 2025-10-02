// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPathFinder.h"
#include <queue>

// default constructor for the grid path finder class
GridPathFinder::GridPathFinder(float InSpacing)
    : Spacing(InSpacing)
{

}

// Hilfsfunktion: FVector -> Index 
FIntVector GridPathFinder::FloatToIndex(const FVector& P) const
{
    return FIntVector(
        FMath::RoundToInt((P.X - Origin.X) / Spacing),
        FMath::RoundToInt((P.Y - Origin.Y) / Spacing),
        FMath::RoundToInt((P.Z - Origin.Z) / Spacing)
    );
}
// Hilfsfunktion: Index -> FVector
FVector GridPathFinder::IndexToFloat(const FIntVector& Idx) const
{
    return FVector(
        Origin.X + Idx.X * Spacing,
        Origin.Y + Idx.Y * Spacing,
        Origin.Z + Idx.Z * Spacing
    );
}

// finds the shortest path in a grid with a constant distance
TArray<FVector> GridPathFinder::FindPath(const TArray<FVector>& Points, const FVector& Start, const FVector& Goal, bool& Success)
{
    TArray<FVector> Result;

    if (Points.Num() == 0)
    {
        Success = true;
        return Result;
    }

    // 1. Origin bestimmen
    float MinX = Points[0].X, MinY = Points[0].Y, MinZ = Points[0].Z;
    for (const FVector& P : Points)
    {
        MinX = FMath::Min(MinX, P.X);
        MinY = FMath::Min(MinY, P.Y);
        MinZ = FMath::Min(MinZ, P.Z);
    }
    Origin = FVector(MinX, MinY, MinZ);

    // 2. Alle Punkte in Index-Set speichern
    TSet<FIntVector> Nodes;
    for (const FVector& P : Points)
    {
        Nodes.Add(FloatToIndex(P));
    }

    FIntVector StartIdx = FloatToIndex(Start);
    FIntVector EndIdx = FloatToIndex(Goal);

    if (!Nodes.Contains(StartIdx) || !Nodes.Contains(EndIdx))
    {
        UE_LOG(LogTemp, Error, TEXT("NO PATH POSSIBLE"));
        Success = false;
        return Result; // Kein Weg möglich
    }

    // 3. BFS vorbereiten
    std::queue<FIntVector> Q;
    Q.push(StartIdx);

    TSet<FIntVector> Visited;
    Visited.Add(StartIdx);

    TMap<FIntVector, FIntVector> Parent;

    // 6-Nachbarschaft (±X, ±Y, ±Z)
    const TArray<FIntVector> Directions = {
        FIntVector(1,0,0), FIntVector(-1,0,0),
        FIntVector(0,1,0), FIntVector(0,-1,0),
        FIntVector(0,0,1), FIntVector(0,0,-1)
    };

    // 4. BFS Loop
    bool bFound = false;
    while (!Q.empty())
    {
        FIntVector Cur = Q.front();
        Q.pop();

        if (Cur == EndIdx)
        {
            bFound = true;
            break;
        }

        for (const FIntVector& D : Directions)
        {
            FIntVector Nb = Cur + D;
            if (Nodes.Contains(Nb) && !Visited.Contains(Nb))
            {
                Visited.Add(Nb);
                Parent.Add(Nb, Cur);
                Q.push(Nb);
            }
        }
    }

    // 5. Pfad rekonstruieren
    if (bFound)
    {
        FIntVector Cur = EndIdx;
        while (Cur != StartIdx)
        {
            Result.Insert(IndexToFloat(Cur), 0);
            Cur = Parent[Cur];
        }
        Result.Insert(IndexToFloat(StartIdx), 0);
    }

    Success = true;
    return Result;
}