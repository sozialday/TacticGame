// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "MinimapRendererBase.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API AMinimapRendererBase : public ASceneCapture2D
{
	GENERATED_BODY()
	
public:

	AMinimapRendererBase();

	void SetCursorReference(TObjectPtr<class ACursorBase> NewCursorReference)
	{
		m_CursorReference = NewCursorReference;
	}
	void EnableTracking()
	{
		m_bIsTrackingEnabled = true;
	}

	FTransform CalculateMinimapCameraTransform();

	// default parameters for the minimap camera
	const FRotator DefaultMinimapRotation = FRotator(-53, 0, 0);
	// default FOV for the minimap camera
	const float DefaultMinimapFOV = 50.0f;

protected:

	virtual void Tick(float DeltaSeconds) override;

private:

	bool m_bIsTrackingEnabled = false;

	UPROPERTY()
	TObjectPtr<class ACursorBase> m_CursorReference = nullptr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", DisplayName = "Minimap Camera Height"))
	float m_MinimapCameraHeight = 1600.0f;

	// dynamic minimap adjustment parameters

	float m_calculatedFOV;
};
