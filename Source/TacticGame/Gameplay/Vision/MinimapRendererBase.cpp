// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapRendererBase.h"

#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"

#include "Kismet/KismetMathLibrary.h"

#include "TacticGame/UserInput/CursorBase.h"
#include "Components/SceneCaptureComponent2D.h"

AMinimapRendererBase::AMinimapRendererBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//	//	//

	GetCaptureComponent2D()->FOVAngle = 50.0f;
	GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

	{
		static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> minimap_texture_tmp
		(TEXT("/Game/Miscellaneous/FX/Textures/RenderTextures/RT_MinimapOverlay.RT_MinimapOverlay"));

		if (minimap_texture_tmp.Succeeded())
			GetCaptureComponent2D()->TextureTarget = minimap_texture_tmp.Object;
	}
}

// Calculates the desired transform of the minimap camera based on the current game state
FTransform AMinimapRendererBase::CalculateMinimapCameraTransform()
{
	if (!IsValid(m_CursorReference))
		return FTransform();

	// Location Calculation

	const FVector cursorLocation = m_CursorReference->GetActorLocation();
	const FVector cursorForward = m_CursorReference->GetActorForwardVector();

	// Rotation Calculation

	FQuat qnewRotation = DefaultMinimapRotation.Quaternion();
	
	float BackwardDistance = 1000.0f;
	float CameraHeight = m_MinimapCameraHeight;

	if (const auto& Unit = m_CursorReference->GetHoveredUnit())
	{
		float Alpha = UKismetMathLibrary::MapRangeClamped(Unit->GetSphereRadius(), 500, 1800, 0.0, 1.0);
		qnewRotation = FMath::Lerp(DefaultMinimapRotation, FRotator(-90, 0, 0), Alpha).Quaternion();

		BackwardDistance = FMath::Lerp(1000.0f, 0.0, Alpha);
		CameraHeight = FMath::Lerp(m_MinimapCameraHeight, 2000, Alpha);
	}

	const FVector newLocation = ((cursorForward * (-BackwardDistance)) + cursorLocation) + FVector(0, 0, CameraHeight);

	return FTransform(qnewRotation, newLocation, FVector(1, 1, 1));
}

// Calculates the desired FOV angle of the minimap camera based on the current game state
float CalculateDesiredFOVAngle(TObjectPtr<class AUnitCharacterBase> Unit, float DefaultFoV)
{
	if (!IsValid(Unit))
		return DefaultFoV;

	float Alpha = UKismetMathLibrary::MapRangeClamped(Unit->GetSphereRadius(), 500, 1800, 0.0, 1.0);
	return FMath::Lerp(50, 110, Alpha);
}

// Called every frame
void AMinimapRendererBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!m_bIsTrackingEnabled)
		return;

	if (!IsValid(m_CursorReference))
		return;

	// Field of View Adjustment

	m_calculatedFOV = CalculateDesiredFOVAngle(m_CursorReference->GetHoveredUnit(), DefaultMinimapFOV);
	GetCaptureComponent2D()->FOVAngle = FMath::FInterpTo(GetCaptureComponent2D()->FOVAngle, m_calculatedFOV, DeltaSeconds, 6.0);

	// Transform Adjustment

	FTransform newTransform;
	
	const FTransform currentTransform = GetActorTransform();
	const FTransform targetTransform = CalculateMinimapCameraTransform();

	newTransform.SetLocation(FMath::VInterpTo(currentTransform.GetLocation(), targetTransform.GetLocation(), DeltaSeconds, 15));
	newTransform.SetRotation(FMath::QInterpTo(currentTransform.GetRotation(), targetTransform.GetRotation(), DeltaSeconds, 6.0));

	SetActorTransform(newTransform);
}