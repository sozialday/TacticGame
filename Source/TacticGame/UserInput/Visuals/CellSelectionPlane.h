// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Components/TimelineComponent.h"
#include "CellSelectionPlane.generated.h"

/**
 * This actor is going to be a visual representation of the cell the cursor is currently hovering over.
 * 
 * it is going to smoothly follow the cursor position snapped to grid
 * only visual, no collision
 */
UCLASS()
class TACTICGAME_API ACellSelectionPlane : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:

	ACellSelectionPlane();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetupAndUpdateCellSelectionPlaneColour(float DeltaSeconds);

public:

	void SetAccelerationValue(float value) { CurrentAccelerationValue = value; }
	void SetCursorReference(TObjectPtr<class ACursorBase> cursor) { m_Cursor = cursor; }
	void PassHitResultOfGround(FHitResult Hit) { HitResult = Hit; }

	void StartMovement()
	{
		if (!m_CellAnimation->IsPlaying())
			m_CellAnimation->PlayFromStart();
	}
	void StopMovement(bool ResetToStartValues = true)
	{
		m_CellAnimation->Stop();
	
		if (ResetToStartValues)
			m_CellAnimation->SetPlaybackPosition(0.0, false, true);
	}

	// adjusts the color of the plane
	void SetPlaneColour(FLinearColor NewColour = FColor::White)
	{
		m_TargetPlaneColour = NewColour;
	}

private:

	float CurrentAccelerationValue = 0.0f;
	FHitResult HitResult;

	UPROPERTY()
	TObjectPtr<class ACursorBase> m_Cursor = nullptr;

	float GridSize;

	// the current colour of the plane [inside the material]
	FLinearColor m_TargetPlaneColour = FColor::White;

private:

	FVector startingSize, endSize;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> m_CellAnimation;

	TObjectPtr<UCurveFloat> m_FloatCurve;

	// Callback Funktion
	UFUNCTION()
	void TimelineProgress(float Value);
};
