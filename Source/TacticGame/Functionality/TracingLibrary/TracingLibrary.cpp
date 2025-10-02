// Fill out your copyright notice in the Description page of Project Settings.


#include "TracingLibrary.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "TacticGame/Gameplay/LevelPrerequisits/LevelStateBase.h"

bool UTracingLibrary::TraceUnit(UWorld* WorldContext, TObjectPtr<class AActor> self, FName& TagHit, FHitResult& HitResult)
{
	if (!self)
	{
		HitResult = FHitResult();
		return false;
	}

	TArray<FHitResult> HitResults;

	const FVector Start_End__Tracing = UKismetMathLibrary::Vector_SnappedToGrid(self->GetActorLocation(), ALevelStateBase::GetLevelGridSize());
	const FVector BoxSize = FVector(ALevelStateBase::GetCellDimension(), 200);

	if (UKismetSystemLibrary::BoxTraceMultiForObjects(WorldContext, Start_End__Tracing, Start_End__Tracing,
		BoxSize, FRotator::ZeroRotator, { EObjectTypeQuery::ObjectTypeQuery3 }, false, { self },
		ALevelStateBase::GetTraceVisibility(), HitResults, true))
	{
		for (const auto& HitSingle : HitResults)
		{
			if (HasTags(HitSingle.GetActor(), { ALevelStateBase::GetTag_Characters_Allies(), ALevelStateBase::GetTag_Characters_Enemies() }, TagHit))
			{
				HitResult = HitSingle;
				return true;
			}
		}
	}

	HitResult = FHitResult();
	return false;
}
bool UTracingLibrary::TraceUnit(UWorld* WorldContext, TObjectPtr<class AActor> TargetUnit, FVector StartPosition, FName& TagHit, FHitResult& HitResult)
{
	if (!TargetUnit)
	{
		HitResult = FHitResult();
		return false;
	}

	TArray<FHitResult> HitResults;

	const FVector Start_End__Tracing = UKismetMathLibrary::Vector_SnappedToGrid(StartPosition, ALevelStateBase::GetLevelGridSize());
	const FVector BoxSize = FVector(ALevelStateBase::GetCellDimension(), 200);

	if (UKismetSystemLibrary::BoxTraceMultiForObjects(WorldContext, Start_End__Tracing, Start_End__Tracing,
		BoxSize, FRotator::ZeroRotator, { EObjectTypeQuery::ObjectTypeQuery3 }, false, { TargetUnit },
		ALevelStateBase::GetTraceVisibility(), HitResults, true))
	{
		for (const auto& HitSingle : HitResults)
		{
			if (HasTags(HitSingle.GetActor(), { ALevelStateBase::GetTag_Characters_Allies(), ALevelStateBase::GetTag_Characters_Enemies() }, TagHit))
			{
				HitResult = HitSingle;
				return true;
			}
		}
	}

	HitResult = FHitResult();
	return false;
}
