// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TacticGame/Gameplay/LevelPrerequisits/LevelStateBase.h"
#include "GlobalStructureContainer.generated.h"

USTRUCT(BlueprintType)
struct FLevelProgression
{
	GENERATED_BODY()

private:

	// interpolation using a curve
	float CurveInterp(float A, float B, float Alpha, UCurveFloat* curve = nullptr)
	{
		if (!curve)
		{
			return xerp(A, B, Alpha); // fallback exponential
		}

		// Wert aus der Kurve im Bereich [0..1] abfragen
		float curveValue = curve->GetFloatValue(Alpha);

		// Mit Kurvenwert statt powf interpolieren
		return FMath::Lerp(A, B, curveValue);
	}

	// exponential lerp
	float xerp(float A, float B, float Alpha) 
	{
		// Exponent (z.B. 2.0f für quadratisch, 3.0f für kubisch)
		return A + (B - A) * powf(Alpha, 2);
	}

	float CalculateMaxLevelProgress(int CurrentLevel)
	{
		auto Alpha = FMath::GetMappedRangeValueClamped(FVector2D(1.0, 100.0), FVector2D(0.0, 1.0), (double)CurrentLevel);
		return xerp(100.0f, 1000000.0f, Alpha);
	}

public:

	void AddXP(float XPValue)
	{
		if (Level == ALevelStateBase::GetMaxLevel())
			return;

		OverflowingXP = 0.0;

		do
		{
			float ValueAdded;
			if (OverflowingXP != 0.0)
			{
				ValueAdded = OverflowingXP;
				OverflowingXP = 0.0;
			}
			else
			{
				ValueAdded = XPValue;
			}

			LevelProgress += ValueAdded;

			if (LevelProgress >= MaxLevelProgress)
			{
				OverflowingXP = LevelProgress - MaxLevelProgress;

				UE_LOG(LogTemp, Error, TEXT("Overflowing XP : %f"), OverflowingXP);

				Level++;
				LevelProgress = 0.0;
				MaxLevelProgress = CalculateMaxLevelProgress(Level);
			}

		} while (OverflowingXP != 0.0 && Level < ALevelStateBase::GetMaxLevel());
	}

	FLevelProgression() 
	{
		OverflowingXP = 0.0f;
		Level = 1.0f;
		MaxLevelProgress = 100.0f;
	}
	FLevelProgression(int CurrentLevel, float CurrentProgress)
	{
		Level = CurrentLevel;
		MaxLevelProgress = CalculateMaxLevelProgress(CurrentLevel);
		LevelProgress = CurrentProgress;
	}

	int Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LevelProgress = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxLevelProgress = 100.0f;

private:

	float OverflowingXP = 0.0f;
};

class TACTICGAME_API GlobalStructureContainer
{
public:
};
