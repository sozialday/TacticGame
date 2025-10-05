// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitCharacterBase.h"

#include "NavigationSystem.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "TacticGame/UserInput/CursorBase.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TacticGame/UserInput/Visuals/CellSelectionPlane.h"
#include "TacticGame/Gameplay/Units/Movement/MovementPath.h"
#include "TacticGame/Gameplay/Units/UnitVisualRepresentation.h"
#include "TacticGame/Gameplay/FieldConstruction/GameplayGrid.h"
#include "TacticGame/Gameplay/LevelPrerequisits/LevelStateBase.h"

#include "TacticGame/Functionality/PathFinding/GridPathFinder.h"
#include "TacticGame/Functionality/TracingLibrary/TracingLibrary.h"

#include "TacticGame/Gameplay/Units/AdditionalComponents/UnitStatsComponent.h"
#include "TacticGame/Gameplay/Units/AdditionalComponents/HealthComponentBase.h"
#include "TacticGame/Gameplay/Units/AdditionalComponents/InventoryComponentBase.h"

#include "TacticGame/Gameplay/LevelPrerequisits/TacticGame_StageGamemodeBase.h"

// Sets default values
AUnitCharacterBase::AUnitCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = CreateDefaultSubobject<UHealthComponentBase>(TEXT("Health Component"));
	UnitStats = CreateDefaultSubobject<UUnitStatsComponent>(TEXT("Unit Stats"));
	Inventory = CreateDefaultSubobject<UInventoryComponentBase>(TEXT("Unit Inventory"));
}

void AUnitCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	// snaps the unit the the grid on begin play
	FVector SnappedLocation = UKismetMathLibrary::Vector_SnappedToGrid(GetActorLocation(), ALevelStateBase::GetLevelGridSize());
	SnappedLocation.Z = GetActorLocation().Z;
	//	//
	SetActorLocation(SnappedLocation);
}

// will be used to move the character along a set path
void AUnitCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_isPicked)
	{
		if (!m_CurrentCursor)
			m_CurrentCursor = Cast<ACursorBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		const FVector CursorLocation = m_CurrentCursor->GetActorLocation();
		//	//
		FVector CurrentPos = UKismetMathLibrary::Vector_SnappedToGrid(CursorLocation, ALevelStateBase::GetLevelGridSize());
		CurrentPos.Z = CursorLocation.Z;

		float Distance = FVector::Dist(CurrentPos, m_LastSavedPosition);

		if (Distance >= 90.0f)
		{
			// check if the field is legal for placement
			{
				if (!Grid)
					Grid = Cast<AGameplayGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameplayGrid::StaticClass()));

				const FVector snappedLocation = UKismetMathLibrary::Vector_SnappedToGrid(m_CurrentCursor->GetActorLocation(), ALevelStateBase::GetLevelGridSize());

				auto box = UKismetMathLibrary::MakeBoxWithOrigin(snappedLocation, FVector(80, 80, 250));
				auto box_array = Grid->GetInstancedMesh()->GetInstancesOverlappingBox(box, true);

				FHitResult Hit;
				bool otherUnitHit = UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(), snappedLocation, snappedLocation, FVector(80, 80, 450),
					FRotator::ZeroRotator, { EObjectTypeQuery::ObjectTypeQuery3 }, false, { this }, ALevelStateBase::GetTraceVisibility(), Hit, true);

				if (!box_array.IsEmpty() && ((!legalFields.Contains(box_array[0])) || otherUnitHit))
				{
					UE_LOG(LogTemp, Error, TEXT("Illegal Field"));
					return;
				}
			}

			// already snapped to grid therefore no need to snap again
			const FVector NewPosition = CurrentPos;
			AddMovementTrackPoint(NewPosition);

			m_LastSavedPosition = CurrentPos;

			// Optional: Debug-Ausgabe
			UE_LOG(LogTemp, Error, TEXT("Position gespeichert: %s"), *CurrentPos.ToString());
		}
	}

	// for the movement to the said point
	if (m_enableTickingMovement)
	{
		const auto& spline = m_CurrentMovementPath->GetSplineComponent();
		const float splineLength = spline->GetSplineLength();

		if (m_currentDistanceAlongSpline >= splineLength)
		{
			MovementProcessFinished();
		}
		else
		{
			m_currentDistanceAlongSpline += m_MovementSpeed * DeltaTime;
			m_currentDistanceAlongSpline = FMath::Clamp(m_currentDistanceAlongSpline, 0.0, splineLength);

			auto location = spline->GetLocationAtDistanceAlongSpline(m_currentDistanceAlongSpline, ESplineCoordinateSpace::World);
			auto rotation = spline->GetRotationAtDistanceAlongSpline(m_currentDistanceAlongSpline, ESplineCoordinateSpace::World);

			SetActorLocationAndRotation(location, rotation);
		}
	}
}

// adds a point to the movement track
void AUnitCharacterBase::AddMovementTrackPoint(FVector Location)
{
	m_MovementTrackPoints.Add(Location);

	// check here for the quickest route if allowed

	if ((!m_MovementTrackPoints.IsEmpty()) && (!AllPossiblePointsToTravel.IsEmpty()))
	{
		const float GridSize = ALevelStateBase::GetLevelGridSize();

		// GRID - path finder
		GridPathFinder PathFinder = GridPathFinder(ALevelStateBase::GetLevelGridSize());

		m_shortestPathPoints = PathFinder.FindPath(AllPossiblePointsToTravel,
			UKismetMathLibrary::Vector_SnappedToGrid(m_MovementTrackPoints[0], GridSize),
			UKismetMathLibrary::Vector_SnappedToGrid(m_MovementTrackPoints[m_MovementTrackPoints.Num() - 1], GridSize), m_SuccessfulPathFound);
	}
}


void AUnitCharacterBase::PickUnit()
{
	m_isPicked = true;
	SetActorEnableCollision(false);

	if (!Grid)
		Grid = Cast<AGameplayGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameplayGrid::StaticClass()));

	const auto& InstancedMesh = Grid->GetInstancedMesh();
	if (InstancedMesh)
	{
		for (auto field : legalFields)
		{
			FTransform Transform;
			InstancedMesh->GetInstanceTransform(field, Transform);

			// check if a player is on one of the cells you are able to move in
			// and only add them to the array if there is no hit [except for this unit itself]

			FName HitName;
			FHitResult HitRes;
			const bool unitHit = UTracingLibrary::TraceUnit(GetWorld(), this, Transform.GetLocation(), HitName, HitRes);

			if (!unitHit)
				AllPossiblePointsToTravel.Add(Transform.GetLocation());
		}
	}

	// spawn an actor which will visualize the characters movement 

	m_VisualRepresentation = GetWorld()->SpawnActor<AUnitVisualRepresentation>(AUnitVisualRepresentation::StaticClass(), GetActorTransform());
	m_VisualRepresentation->AddActorWorldOffset(FVector(0, 0, -90));
	m_VisualRepresentation->AddActorWorldRotation(FRotator(0, -90, 0));
	m_VisualRepresentation->SetSkeletalMesh(GetMesh()->GetSkeletalMeshAsset());

	const auto& cursorPlate = UGameplayStatics::GetActorOfClass(GetWorld(), ACellSelectionPlane::StaticClass());
	if (cursorPlate)
		m_VisualRepresentation->AttachToActor(cursorPlate, FAttachmentTransformRules::KeepWorldTransform);
}
void AUnitCharacterBase::ReleaseUnit()
{
	Cancel();

	// decides which path the unit should move

	const bool pathSelectionType = ALevelStateBase::GetPathSelectionType() == CustomPath;
	TArray<FVector> MovementPoints = pathSelectionType ? m_MovementTrackPoints : m_shortestPathPoints;
	
	if (!m_SuccessfulPathFound)
		MovementPoints = m_MovementTrackPoints;

	// spawns a movement track for the character to follow
	m_CurrentMovementPath = ALevelStateBase::AddMovementTrackFilled(GetWorld(), GetActorLocation(), MovementPoints);

	// start moving along the track
	MoveAlongTrack();
}

void AUnitCharacterBase::Cancel()
{
	m_isPicked = false;

	AllPossiblePointsToTravel.Empty();

	SetActorEnableCollision(true);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	if (m_VisualRepresentation)
	{
		m_VisualRepresentation->Destroy();
		m_VisualRepresentation = nullptr;
	}
}

void AUnitCharacterBase::MoveAlongTrack()
{
	if (!m_CurrentMovementPath)
		return;

	if (Grid)
		Grid->ClearAllCells();

	if(m_CurrentCursor)
		m_CurrentCursor->RemoveUnitDetailsFromParent();

	const auto& spline = m_CurrentMovementPath->GetSplineComponent();
	m_maxSplineDistance = spline->GetDistanceAlongSplineAtSplinePoint(spline->GetNumberOfSplinePoints() - 1);

	m_enableTickingMovement = true;
}
void AUnitCharacterBase::MovementProcessFinished()
{
	if (!m_CurrentCursor)
		m_CurrentCursor = Cast<ACursorBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACursorBase::StaticClass()));

	if (Grid)
		Grid->ClearAllCells();

	m_CurrentCursor->SetCameraCustomMovement_UnitMovement(false);
	m_CurrentCursor->TraceDownForPlayerGridSelection();

	m_CurrentCursor = nullptr;
	m_enableTickingMovement = false;
	m_currentDistanceAlongSpline = 0.0f;

	m_shortestPathPoints.Empty();
	m_MovementTrackPoints.Empty();
	m_LastSavedPosition = FVector::ZeroVector;

	if (m_CurrentMovementPath)
	{
		m_CurrentMovementPath->Destroy();
		m_CurrentMovementPath = nullptr;
	}

	// make a request to the gamemode to show the ui 
	if (const auto& gamemode = Cast<ATacticGame_StageGamemodeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gamemode->ShowAfterMovementActionUIElement();
	}
}