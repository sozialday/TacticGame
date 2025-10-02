// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCameraBase.h"
#include "Components/ArrowComponent.h"

#include "Kismet/GameplayStatics.h"
#include "TacticGame/UserInput/CursorBase.h"
#include "E:/UE5_Default/UE_5.6/Engine/Source/Runtime/CinematicCamera/Public/CineCameraComponent.h"

// Sets default values
AGameplayCameraBase::AGameplayCameraBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	m_ForwardFacing_ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardFacing_ArrowComponent"));
	m_ForwardFacing_ArrowComponent->SetupAttachment(RootComponent);
	//	//
	m_ForwardFacing_ArrowComponent->SetAbsolute(false, true, false);

	m_CameraOffset = FVector(0, 0, 600.0);
	m_TargetCameraRotation = GetActorRotation();


	// Camera Setup [..]

	FCameraFilmbackSettings FilmbackSettings;
	FilmbackSettings.SensorHeight = 56.630001;
	FilmbackSettings.SensorWidth = 70.410004;

	GetCineCameraComponent()->Filmback = FilmbackSettings;
	GetCineCameraComponent()->LensSettings.MinFStop = 0.0f;
	GetCineCameraComponent()->CurrentFocalLength = 20.0f;
	GetCineCameraComponent()->LensSettings.MinFocalLength = 1.0f;
	GetCineCameraComponent()->LensSettings.MaxFocalLength = 100.0f;
	GetCineCameraComponent()->CurrentAperture = 0.18;

	GetCineCameraComponent()->bConstrainAspectRatio = false;
	GetCineCameraComponent()->PostProcessSettings.bOverride_VignetteIntensity = true;
}

// Called when the game starts or when spawned
void AGameplayCameraBase::BeginPlay()
{
	Super::BeginPlay();

	m_TargetCameraRotation = GetActorRotation();
	SetRelativeOffsetsToGlobalOffset();

	m_Cursor = Cast<ACursorBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACursorBase::StaticClass()));
	if (m_Cursor)
		m_enableTicking = true;
}

// calculates the new location for the camera based on the cursor position and the camera offset
FVector CalculateNewActorLocation(TObjectPtr<class AActor> CursorActor, FVector CameraOffset, float CameraDistanceToCursor_absolute)
{
	const FVector CursorLocation = CursorActor->GetActorLocation();
	const FRotator CursorRotation = Cast<ACursorBase>(CursorActor)->GetCameraOffsetDirection()->GetComponentRotation();

	const FVector BackwardVector = CursorRotation.Vector() * (-CameraDistanceToCursor_absolute);

	return BackwardVector + (CursorLocation + CameraOffset);
}

// Called every frame
void AGameplayCameraBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_enableTicking)
		return;

	if (m_enableTicking__Inspection)
	{
		InspectionTicking(DeltaTime);
		return;
	}

	if (m_customCameraMovement)
	{
		SetActorLocation(m_CameraLocation);
	}
	else
	{
		m_CameraLocation = GetActorLocation();

		SetActorLocation(FMath::VInterpTo(m_CameraLocation,
			CalculateNewActorLocation(m_Cursor, m_CameraOffset, m_CameraDistanceToCursor), DeltaTime, 6.5));

	}

	// smooth camera rotation to desired rotation
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), m_TargetCameraRotation, DeltaTime, 3.5));

	// Focal Length
	GetCineCameraComponent()->CurrentFocalLength = FMath::FInterpTo(GetCineCameraComponent()->CurrentFocalLength, 20.0f, DeltaTime, 10);
	GetCineCameraComponent()->CurrentAperture = FMath::FInterpTo(GetCineCameraComponent()->CurrentAperture, 0.18, DeltaTime, 10);
	GetCineCameraComponent()->PostProcessSettings.VignetteIntensity =
		FMath::FInterpTo(GetCineCameraComponent()->PostProcessSettings.VignetteIntensity, 0.4, DeltaTime, 15);

	// camera focal point 

	if (m_Cursor)
	{
		// Focal Length 

		const FVector FocalPoint = m_Cursor->GetActorLocation();
		const float FocalDistance = FVector::Distance(FocalPoint, GetActorLocation());

		GetCineCameraComponent()->FocusSettings.ManualFocusDistance = 
			FMath::FInterpTo(GetCineCameraComponent()->FocusSettings.ManualFocusDistance, FocalDistance, DeltaTime, 9);
	}
}

void AGameplayCameraBase::InspectionTicking(float DeltaSeconds)
{
	const FVector actor_loc = FMath::VInterpTo(GetActorLocation(), m_InspectionCameraLocation, DeltaSeconds, 12);
	SetActorLocation(actor_loc);

	const FRotator actor_rot = FMath::RInterpTo(GetActorRotation(), m_InspectionCameraRotation, DeltaSeconds, 9);
	SetActorRotation(actor_rot);

	GetCineCameraComponent()->CurrentFocalLength = FMath::FInterpTo(GetCineCameraComponent()->CurrentFocalLength, 60.0, DeltaSeconds, 10);
	GetCineCameraComponent()->FocusSettings.ManualFocusDistance =
		FMath::FInterpTo(GetCineCameraComponent()->FocusSettings.ManualFocusDistance, 690.0, DeltaSeconds, 10);
	GetCineCameraComponent()->CurrentAperture = FMath::FInterpTo(GetCineCameraComponent()->CurrentAperture, 0.45, DeltaSeconds, 10);

	GetCineCameraComponent()->PostProcessSettings.VignetteIntensity =
		FMath::FInterpTo(GetCineCameraComponent()->PostProcessSettings.VignetteIntensity, 1.0f, DeltaSeconds, 4);
}