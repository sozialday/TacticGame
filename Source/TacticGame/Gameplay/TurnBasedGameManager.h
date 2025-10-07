// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnBasedGameManager.generated.h"

UCLASS()
class TACTICGAME_API ATurnBasedGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurnBasedGameManager();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TryTurn(class AUnitCharacterBase* unit, bool isEnemy = false);

	// clears the lists of units that had their turn already
	// used to start a new round
	UFUNCTION(BlueprintCallable)
	void ResetTurnLists()
	{
		m_AllyUnitsThatHadTurn.Empty();
		m_EnemyUnitsThatHadTurn.Empty();
	}

	UFUNCTION(BlueprintPure)
	int GetNumAllyUnitsThatHaveATurnLeft() const
	{
		return m_AllAllyUnits.Num() - m_AllyUnitsThatHadTurn.Num();
	}

	void AddTurnToList(class AUnitCharacterBase* unit, bool isEnemy = false);
	bool VerifyIfUnitCanTakeTurn(TObjectPtr<class AUnitCharacterBase> unit, bool isEnemy = false);

private:

	// A FUNCTION THAT RETURNS TRUE IF ALL UNITS OF THE GIVEN FACTION HAVE TAKEN THEIR TURN ALREADY

	bool AllUnitsOfFactionHadTurn(bool isEnemy = false) const
	{
		if (isEnemy)
		{
			// if the number of enemy units that had their turn already is equal to the total number of enemy units
			return m_EnemyUnitsThatHadTurn.Num() >= m_AllEnemyUnits.Num();
		}
		else
		{
			// if the number of ally units that had their turn already is equal to the total number of ally units
			return m_AllyUnitsThatHadTurn.Num() >= m_AllAllyUnits.Num();
		}
	}

private:

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Auto Find All Units"))
	uint8 m_AutoFindAllUnits : 1;

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true", EditCondition = "m_AutoFindAllUnits == false"))
	TArray<class AUnitCharacterBase*> AllyUnits;

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true", EditCondition = "m_AutoFindAllUnits == false"))
	TArray<class AUnitCharacterBase*> EnemyUnits;

private:

	TArray<TObjectPtr<class AUnitCharacterBase>> m_AllAllyUnits;
	TArray<TObjectPtr<class AUnitCharacterBase>> m_AllEnemyUnits;

	// units that had their turn already [starts empty each round]

	TArray<TObjectPtr<class AUnitCharacterBase>> m_AllyUnitsThatHadTurn;
	TArray<TObjectPtr<class AUnitCharacterBase>> m_EnemyUnitsThatHadTurn;
};
