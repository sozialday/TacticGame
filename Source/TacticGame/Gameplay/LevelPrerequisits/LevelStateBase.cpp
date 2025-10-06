// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStateBase.h"
#include "Components/SplineComponent.h"

#include "TacticGame/Gameplay/Units/Movement/MovementPath.h"

// Definition der statischen Variable [Referenz zum Game Manager]
TObjectPtr<class ATurnBasedGameManager> ALevelStateBase::m_GameManager = nullptr;

// Definition der statischen Variable [Globale Grid Groesse]
float ALevelStateBase::GridSize = 180.0f;

// Definition der statischen Variable [Path Selection Method]
TEnumAsByte<EPathSelectionType> ALevelStateBase::m_pathSelectionMethod = EPathSelectionType::FastestPath;

// Definition der statischen Variable [Ground Tag for Tracking]
FName ALevelStateBase::Gameplay_Ground = FName("GAMEPLAY-Ground");

// Definition der statischen Variable [Ignore Tag for Grid Generation]
FName ALevelStateBase::Ignore_Gameplay_GridGeneration = FName("IGNORE-GridGeneration");

// Definition der statischen Variable [Tags] fuer Gameplay Pieces [Allies]
FName ALevelStateBase::GameplayPieces_Characters_Allies = FName("GAMEPLAY-Character-Ally");
// Definition der statischen Variable [Tags] fuer Gameplay Pieces [Enemies]
FName ALevelStateBase::GameplayPieces_Characters_Enemies = FName("GAMEPLAY-Character-Enemy");

// Farbe der Zelle wenn man sich dahinbewegen kann
FLinearColor ALevelStateBase::MovableCellColour = FLinearColor(0.033, 0.061, 0.396, 0.33);
// Farbe der Zelle wenn man sich da nicht hinbewegen kann
FLinearColor ALevelStateBase::UnMovableCellColour = FLinearColor(0.396, 0.033, 0.033, 0.33);

// Tracing Visibility
EDrawDebugTrace::Type ALevelStateBase::DebugTraceVisibility = EDrawDebugTrace::None;

// Max Level
int ALevelStateBase::MaxLevel = 100;

// creates a movement path actor and fills it with the given points
TObjectPtr<class AMovementPath> ALevelStateBase::AddMovementTrackFilled(UWorld* WorldContext, FVector StartingLocation, TArray<FVector> Points)
{
	if (!WorldContext)
		return nullptr;

	UE_LOG(LogTemp, Error, TEXT("Spline spawning. Num Points : %i"), Points.Num());

	FTransform SpawnLocation;
	SpawnLocation.SetTranslation(StartingLocation);

	const auto& movementPath = WorldContext->SpawnActor<AMovementPath>(AMovementPath::StaticClass(), SpawnLocation);
	movementPath->GetSplineComponent()->ClearSplinePoints(true);
	for (const auto& point : Points)
	{
		FVector offset = FVector(0);
		offset.Z = FMath::Abs(point.Z - StartingLocation.Z);

		// adds each point to the spline component
		movementPath->AddPoint(point + offset);

		UE_LOG(LogTemp, Error, TEXT("Spline point spawned"));
	}

	return movementPath;
}