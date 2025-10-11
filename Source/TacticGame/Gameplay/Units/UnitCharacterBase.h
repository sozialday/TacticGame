// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "TacticGame/Gameplay/Units/AdditionalComponents/UnitStatsComponent.h"
#include "UnitCharacterBase.generated.h"

UENUM(Blueprintable)
enum EGenderSelection
{
	Male,
	Female
};

USTRUCT(Blueprintable)
struct FUnitDetails
{
	GENERATED_BODY()

public:

	// will contain the first and last name
	// 0 = first name
	// 1 = last name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EGenderSelection> Gender = EGenderSelection::Male;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> UnitIcon;

	UPROPERTY()
	TObjectPtr<class AUnitCharacterBase> Unit = nullptr;
};

UCLASS()
class TACTICGAME_API AUnitCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Health"))
	TObjectPtr<class UHealthComponentBase> Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Unit Stats"))
	TObjectPtr<class UUnitStatsComponent> UnitStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Inventory"))
	TObjectPtr<class UInventoryComponentBase> Inventory;

public:
	// Sets default values for this character's properties
	AUnitCharacterBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// calles when the unit takes any damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void PickUnit();
	void ReleaseUnit();
	void Cancel();

	void SetLegalFields(TArray<int> Fields)
	{
		legalFields = Fields;
	}

	// for movement tracking 
	// in order for the character to move along a set path
	void AddMovementTrackPoint(FVector Location);

	void MoveAlongTrack();

	// called when the character is done moving
	void MovementProcessFinished(bool RequestAfterMoveInteraction = true);

	// retrieves the unit data

	FUnitDetails GetUnitConfigurationData()
	{
		UnitConfigurationDetails.Unit = this;
		return UnitConfigurationDetails;
	}

	// retrieves the multiplier for the range using the speed stat
	float GetSpeedMultiplier() const
	{
		const int Speed = UnitStats->GetSpeed();
		const float Alpha =  FMath::GetMappedRangeValueClamped(FVector2D(1.0, 100.0), FVector2D(0.0, 1.0), (float)Speed);

		return FMath::Lerp(1.0, 3.0, Alpha);
	}

private:

	// configure the unit [Name etc.]
	UPROPERTY(/*EditDefaultsOnly*/EditAnywhere, meta = (AllowPrivateAccess = "true", Keywords = "Unit"), Category = "Unit")
	FUnitDetails UnitConfigurationDetails;


	// a list containing all the legal fields this unit can move on
	TArray<int> legalFields;


	/*
	* ADD BFS algorithm for better evaluation of points [for the quickest route between the points]
	* it should be evaluated everytime a new point is added
	* it should then remove all the points from the real array that will be used when moving
	* 
	*/
	TArray<FVector> m_MovementTrackPoints;
	TArray<FVector> m_shortestPathPoints;

	TArray<FVector> AllPossiblePointsToTravel;

	FVector m_LastSavedPosition;
	float m_maxSplineDistance;

	bool m_SuccessfulPathFound = true;

	UPROPERTY()
	TObjectPtr<class ACursorBase> m_CurrentCursor = nullptr;

	bool m_isPicked = false;
	bool m_enableTickingMovement = false;

	UPROPERTY()
	TObjectPtr<class AMovementPath> m_CurrentMovementPath = nullptr;

	UPROPERTY()
	TObjectPtr<class AGameplayGrid> Grid = nullptr;

	UPROPERTY()
	TObjectPtr<class AUnitVisualRepresentation> m_VisualRepresentation = nullptr;

protected:

	// Grid Movement Area Parameters

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true, DisplayName = "Movement Radius"))
	float m_SphereRadiusFromSelf = 500;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true, DisplayName = "Allow Box Movement Option"))
	bool m_allowBoxMovementOption = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "m_allowBoxMovementOption == true", EditConditionHides, AllowPrivateAccess = true, DisplayName = "Box Movement Extend"))
	FVector m_BoxExtend = FVector(400, 400, 100);

public:

	// getter functions

	float GetSphereRadius() const 
	{ 
		return m_SphereRadiusFromSelf * GetSpeedMultiplier();
	}
	FVector GetBoxExtend() const 
	{
		return (m_allowBoxMovementOption ? m_BoxExtend : FVector::ZeroVector) * GetSpeedMultiplier();
	}

private:

	// add Timeline that moves the character along the path
	// the character should have a default speed [e.g. 300 units per second]
	// if the distance between two points is 600 units it should then move faster to not let the player wait too long

	// after the movement is done the path should be destroyed and the array cleared

	//use movement speed from the character movement component

	float m_currentDistanceAlongSpline = 0.0f;
	float m_MovementSpeed = 1500.0f;
};
