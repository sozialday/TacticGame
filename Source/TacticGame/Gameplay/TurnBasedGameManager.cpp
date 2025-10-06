// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedGameManager.h"

#include "Kismet/GameplayStatics.h"

#include "TacticGame/Gameplay/Units/UnitCharacterBase.h"
#include "TacticGame/Gameplay/LevelPrerequisits/LevelStateBase.h"

// Sets default values
ATurnBasedGameManager::ATurnBasedGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATurnBasedGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (m_AutoFindAllUnits)
	{
		TArray<FName> tags;
		{
			tags.Add(ALevelStateBase::GetTag_Characters_Allies());
			tags.Add(ALevelStateBase::GetTag_Characters_Enemies());
		}

		for (const auto& tagSingle : tags)
		{
			TArray<AActor*> foundActors;

			UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),
				AUnitCharacterBase::StaticClass(), tagSingle, foundActors);

			// cast and add to the actual array which stores the units
			for (const auto& actor : foundActors)
			{
				if (const auto& unit = Cast<AUnitCharacterBase>(actor))
				{
					if (tagSingle == ALevelStateBase::GetTag_Characters_Enemies())
						m_AllEnemyUnits.Add(unit);
					else if (tagSingle == ALevelStateBase::GetTag_Characters_Allies())
						m_AllAllyUnits.Add(unit);

					UE_LOG(LogTemp, Warning, TEXT("Found Unit : %s"), *unit->GetName());
				}
			}
		}
	}
}

// tries to let a unit take its turn if it is allowed to do so
void ATurnBasedGameManager::TryTurn(AUnitCharacterBase* unit, bool isEnemy)
{
	if (!VerifyIfUnitCanTakeTurn(unit, isEnemy))
		return;

	// let the unit take its turn
	AddTurnToList(unit, isEnemy);

	// check if all units had their turn already
}

// adds a unit to the turn list making it not able to take another turn until all other units had their turn
void ATurnBasedGameManager::AddTurnToList(AUnitCharacterBase* unit, bool isEnemy)
{
	if (!unit)
		return;

	if (isEnemy)
	{
		// adds the enemy unit to the list of units that had their turn already
		if (!m_EnemyUnitsThatHadTurn.Contains(unit))
			m_EnemyUnitsThatHadTurn.Add(unit);
	}
	else
	{
		// adds the ally unit to the list of units that had their turn already
		if (!m_AllyUnitsThatHadTurn.Contains(unit))
			m_AllyUnitsThatHadTurn.Add(unit);
	}
}

// checks if a unit is allowed to take its turn
bool ATurnBasedGameManager::VerifyIfUnitCanTakeTurn(AUnitCharacterBase* unit, bool isEnemy)
{
	// THIS METHOD SOMETIMES CRASHES ON LINE 106

	if (!unit)
		return false;

	if (isEnemy)	// enemy
	{
		// if the unit is not in the list of all enemy units it cannot take a turn
		if (!m_AllEnemyUnits.Contains(unit))
			return false;
		
		// if you had your turn already this round you cannot take another one
		if (m_EnemyUnitsThatHadTurn.Contains(unit))
			return false;
			
	}
	else			// ally
	{
		// if the unit is not in the list of all ally units it cannot take a turn
		if (!m_AllAllyUnits.Contains(unit))
			return false;
		
		// if you had your turn already this round you cannot take another one
		if (m_AllyUnitsThatHadTurn.Contains(unit))
			return false;
	}

	return true;
}
