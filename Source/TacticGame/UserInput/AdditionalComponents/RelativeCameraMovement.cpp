// Fill out your copyright notice in the Description page of Project Settings.


#include "RelativeCameraMovement.h"
#include "Kismet/GameplayStatics.h"

#include "Camera/CameraComponent.h"
#include "TacticGame/Gameplay/Vision/GameplayCameraBase.h"

// Sets default values for this component's properties
URelativeCameraMovement::URelativeCameraMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void URelativeCameraMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!m_canTick)
		return;

	if (!m_cam)
		m_cam = Cast<AGameplayCameraBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameplayCameraBase::StaticClass()));

	m_cam->GetCameraComponent()->SetRelativeRotation(FMath::RInterpTo(m_cam->GetCameraComponent()->GetRelativeRotation(),
		m_currentRotation, DeltaTime, 2.6));
}