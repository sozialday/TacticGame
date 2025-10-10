// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapCameraTransformPointBase.h"
#include "TacticGame/Gameplay/Vision/GameplayCameraBase.h"
#include "E:/UE5_Default/UE_5.6/Engine/Source/Runtime/CinematicCamera/Public/CineCameraComponent.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMinimapCameraTransformPointBase::AMinimapCameraTransformPointBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(m_Root);
}

// Called every frame
void AMinimapCameraTransformPointBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(m_CameraReference))
		return;

	if (!m_enableCameraFollowing)
		return;

	FTransform newTransform = GetActorTransform();
	const FTransform cameraTransform = m_CameraReference->GetActorTransform();

	newTransform.SetLocation(FMath::VInterpTo(cameraTransform.GetLocation(), GetActorLocation(), DeltaTime, 6));
	newTransform.SetRotation(
		FMath::QInterpTo(cameraTransform.GetRotation(), (GetActorUpVector() * -1).ToOrientationQuat(), DeltaTime, 4.5));

	m_CameraReference->SetActorTransform(newTransform);

	// Depth of Field Adjustments

	m_CameraReference->GetCineCameraComponent()->FocusSettings.ManualFocusDistance =
		FMath::FInterpTo(m_CameraReference->GetCineCameraComponent()->FocusSettings.ManualFocusDistance, m_cameraFocalDistance + 500, DeltaTime, 9);
	m_CameraReference->GetCineCameraComponent()->CurrentAperture = 
		FMath::FInterpTo(m_CameraReference->GetCineCameraComponent()->CurrentAperture, 4.0, DeltaTime, 10);

	m_CameraReference->GetCineCameraComponent()->CurrentFocalLength = 
		FMath::FInterpTo(m_CameraReference->GetCineCameraComponent()->CurrentFocalLength, 14.0f, DeltaTime, 10);
}

// retrieves the ground distance for the camera focal distance
void AMinimapCameraTransformPointBase::TraceToGroundForFocalDistance()
{
	FHitResult hit;

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), GetActorLocation() + FVector(0, 0, 5000),
		GetActorLocation() - FVector(0, 0, 5000), { EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery7 }, 
		false, {}, EDrawDebugTrace::ForDuration, hit, true))
	{
		m_cameraFocalDistance = hit.Distance;
	}
}