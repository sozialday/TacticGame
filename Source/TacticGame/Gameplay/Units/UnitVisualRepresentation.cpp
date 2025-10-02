// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitVisualRepresentation.h"

// Sets default values
AUnitVisualRepresentation::AUnitVisualRepresentation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_visualMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VisualMesh"));
	SetRootComponent(m_visualMesh);

	m_visualMesh->SetAbsolute(false, false, true);
}

void AUnitVisualRepresentation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsMoving())
		return;

	FRotator VelocityDirectionRotation = GetActorRotation();
	VelocityDirectionRotation.Yaw = GetPawnVelocity().Rotation().Yaw - 90.0f;

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), VelocityDirectionRotation, DeltaTime, 21));
}

// HE SHOULD PLAY A LOOPED WALK ANIMATION