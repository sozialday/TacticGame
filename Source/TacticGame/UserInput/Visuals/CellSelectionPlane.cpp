// Fill out your copyright notice in the Description page of Project Settings.


#include "CellSelectionPlane.h"
#include "Kismet/KismetMathLibrary.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"

#include "TacticGame/UserInput/CursorBase.h"
#include "TacticGame/Gameplay/LevelPrerequisits/LevelStateBase.h"
#include "TacticGame/Functionality/TransformationLibrary/TransformationMatrixLibrary.h"

// Sets default values
ACellSelectionPlane::ACellSelectionPlane()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetStaticMeshComponent()->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Miscellaneous/Meshes/CursorPlane.CursorPlane")));
	GetStaticMeshComponent()->SetMaterial(0, LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Miscellaneous/FX/Materials/Grid/M_CursorCell.M_CursorCell")));
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetRelativeScale3D(FVector(ALevelStateBase::GetLevelGridSize() / 100.0f));

	GetStaticMeshComponent()->SetCollisionProfileName(FName("NoCollision"));
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_CellAnimation = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	m_FloatCurve = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/Miscellaneous/Curves/FloatCurves/C_CellSelectionPlaneMovement.C_CellSelectionPlaneMovement"));
}

// Called when the game starts or when spawned
void ACellSelectionPlane::BeginPlay()
{
	Super::BeginPlay();

	GridSize = ALevelStateBase::GetLevelGridSize();

	if (m_FloatCurve)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("TimelineProgress"));

		m_CellAnimation->AddInterpFloat(m_FloatCurve, TimelineCallback);
		m_CellAnimation->SetLooping(true); // true falls du sie unendlich laufen lassen willst

		startingSize = GetActorScale3D();
		endSize = startingSize + 0.3;
	
		m_CellAnimation->PlayFromStart();
	}
}

// Called every frame
void ACellSelectionPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetupAndUpdateCellSelectionPlaneColour(DeltaTime);

	if (!m_Cursor)
		return;


	// align to cursor position snapped to grid

	FVector CursorLocation = m_Cursor->GetActorLocation();
	CursorLocation = UKismetMathLibrary::Vector_SnappedToGrid(CursorLocation, GridSize);
	CursorLocation.Z = (HitResult.ImpactPoint + FVector(0, 0, 5)).Z;

	// smooth movement towards the cursor position
	// the faster the cursor moves, the slower(smoother) the plane follows
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), CursorLocation, DeltaTime, FMath::Lerp(30.0, 16.0, CurrentAccelerationValue)));

	// align to ground normal

	FRotator NewRotation = UTransformationMatrixLibrary::RotateAlongHitSurface(HitResult, GetActorForwardVector());
	NewRotation.Yaw = GetActorRotation().Yaw;

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 30.0f));
}

// smooth interpolation for the Material
void ACellSelectionPlane::SetupAndUpdateCellSelectionPlaneColour(float DeltaSeconds)
{
	const FName ParameterName = FName("Cursor Cell Colour");

	const auto& MPC = LoadObject<UMaterialParameterCollection>(nullptr, TEXT("/Game/Miscellaneous/FX/Materials/Grid/MPC/MPC_GridCell.MPC_GridCell"));
	const auto& CurrentColourValue = UKismetMaterialLibrary::GetVectorParameterValue(GetWorld(), MPC, ParameterName);

	// if the target colour is almost equal the current colour then dont adjust it further (slight optimization)
	if (UKismetMathLibrary::LinearColor_IsNearEqual(CurrentColourValue, m_TargetPlaneColour, 0.15))
		return;


	const FLinearColor newColour = FMath::CInterpTo(CurrentColourValue, m_TargetPlaneColour, DeltaSeconds, 45);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC, ParameterName, newColour);

	UE_LOG(LogTemp, Error, TEXT("Is Adjusting"));
}

void ACellSelectionPlane::TimelineProgress(float Value)
{
	const FVector Size = FMath::Lerp(startingSize, endSize, Value);
	GetRootComponent()->SetRelativeScale3D(Size);
}