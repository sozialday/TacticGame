// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TracingLibrary.generated.h"


UCLASS()
class TACTICGAME_API UTracingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	// will return true if the cell contains a unit that is not "self"
	static bool TraceUnit(UWorld* WorldContext, TObjectPtr<class AActor> self, FName& TagHit, FHitResult& HitResult);

	// will return true if the cell contains a unit that is not "self"
	static bool TraceUnit(UWorld* WorldContext, TObjectPtr<class AActor> TargetUnit, FVector StartPosition, FName& TagHit, FHitResult& HitResult);

	// checks if the Actor contains any tags from the provided array of tags will also return the array that got found
	static bool HasTags(AActor* Actor, TArray<FName> RequiredTags, FName& TagHit)
	{
		for (const auto& tag : RequiredTags)
		{
			if (Actor->ActorHasTag(tag))
			{
				TagHit = tag;	// the tag that was hit
				return true;
			}
		}
		return false;
	}
};
