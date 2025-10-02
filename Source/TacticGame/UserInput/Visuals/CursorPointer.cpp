// Fill out your copyright notice in the Description page of Project Settings.


#include "CursorPointer.h"
#include "Engine/StaticMeshActor.h"

#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"

ACursorPointer::ACursorPointer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetStaticMeshComponent()->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Miscellaneous/Meshes/CursorPointer.CursorPointer")));
	GetStaticMeshComponent()->SetMaterial(0, LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Miscellaneous/FX/Materials/Cursor/M_Cursor.M_Cursor")));
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

	GetStaticMeshComponent()->SetCollisionProfileName(FName("NoCollision"));
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_ZMovement = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	m_FloatCurve = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/Miscellaneous/Curves/FloatCurves/C_CursorZMovementInterpolation.C_CursorZMovementInterpolation"));
}

void ACursorPointer::BeginPlay()
{
	Super::BeginPlay();

	if (m_FloatCurve)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("ZMovementTimelineTick"));

		m_ZMovement->AddInterpFloat(m_FloatCurve, TimelineCallback);
		m_ZMovement->SetPlayRate(1.8);
		m_ZMovement->SetLooping(true); // true falls du sie unendlich laufen lassen willst

		DefaultStartZTranslation = StartZTranslation = 375;
		DefaultEndZTranslation = EndZTranslation = StartZTranslation - 8;

		m_ZMovement->PlayFromStart();
	}
}

void ACursorPointer::ZMovementTimelineTick(const float InterpolationTime)
{
	FVector DefaultLoc = GetStaticMeshComponent()->GetRelativeLocation();

	const float ZVal = FMath::Lerp(StartZTranslation, EndZTranslation, InterpolationTime);
	DefaultLoc.Z = FMath::FInterpTo(DefaultLoc.Z, ZVal, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 3);

	GetStaticMeshComponent()->SetRelativeLocation(DefaultLoc);
}

void ACursorPointer::ChangeZTranslations(float Start, float End, bool startFromCurrentPos, bool useZOffsetFromStartPos)
{
	m_ZMovement->Stop();

	StartZTranslation = startFromCurrentPos ? GetStaticMeshComponent()->GetRelativeLocation().Z : Start;
	EndZTranslation = useZOffsetFromStartPos ? StartZTranslation + End : End;

	m_ZMovement->PlayFromStart();
}
void ACursorPointer::ResetToBasic()
{
	m_ZMovement->Stop();

	StartZTranslation = DefaultStartZTranslation;
	EndZTranslation = DefaultEndZTranslation;

	m_ZMovement->PlayFromStart();
}