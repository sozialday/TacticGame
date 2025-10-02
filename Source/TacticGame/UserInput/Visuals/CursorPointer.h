// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CursorPointer.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API ACursorPointer : public AStaticMeshActor
{
	GENERATED_BODY()
	
	ACursorPointer();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> m_ZMovement;

	TObjectPtr<UCurveFloat> m_FloatCurve;

	UFUNCTION()
	void ZMovementTimelineTick(const float InterpolationTime);


	float StartZTranslation, EndZTranslation, DefaultStartZTranslation, DefaultEndZTranslation;

public:

	UFUNCTION(BlueprintCallable)
	void ChangeZTranslations(float Start, float End, bool startFromCurrentPos = true, bool useZOffsetFromStartPos = true);

	UFUNCTION(BlueprintCallable)
	void ResetToBasic();
};
