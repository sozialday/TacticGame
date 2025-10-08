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
				}
			}
		}
	}

	m_AllyUnitsThatHadTurn.Empty();
	m_EnemyUnitsThatHadTurn.Empty();
}

// tries to let a unit take its turn if it is allowed to do so
void ATurnBasedGameManager::TryTurn(AUnitCharacterBase* unit, bool isEnemy)
{
	if (!VerifyIfUnitCanTakeTurn(unit, isEnemy))
		return;

	// let the unit take its turn
	AddTurnToList(unit, isEnemy);

	// check if all units had their turn already
	if (AllUnitsOfFactionHadTurn(isEnemy))
	{
		// if all units of the faction had their turn already
		// reset the list of units that had their turn already
		UE_LOG(LogTemp, Warning, TEXT("All units of faction had their turn already. Resetting turn list."));
	}
}

// adds a unit to the turn list making it not able to take another turn until all other units had their turn
void ATurnBasedGameManager::AddTurnToList(AUnitCharacterBase* unit, bool isEnemy)
{
	if (!IsValid(unit))
		return;

	if (isEnemy)
	{
		// adds the enemy unit to the list of units that had their turn already
		m_EnemyUnitsThatHadTurn.Add(unit);
	}
	else
	{
		// adds the ally unit to the list of units that had their turn already
		m_AllyUnitsThatHadTurn.Add(unit);
	}
}

// loops through an array and checks if it contains a specific unit
bool ArrayContains(const TArray<TObjectPtr<AUnitCharacterBase>>& array, TObjectPtr<AUnitCharacterBase> unit)
{
	for (const auto& element : array)
	{		
		if (element == unit)
			return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Unit %s not found in array."), IsValid(unit) ? *unit->GetName() : TEXT("Invalid Unit"));
	return false;
}

// checks if a unit is allowed to take its turn
bool ATurnBasedGameManager::VerifyIfUnitCanTakeTurn(TObjectPtr<class AUnitCharacterBase> unit, bool isEnemy)
{
	// THIS METHOD SOMETIMES CRASHES ON LINE 112

	//UE_LOG(LogTemp, Warning, TEXT("Verifying if unit %s can take its turn."), IsValid(unit) ? *unit->GetName() : TEXT("Invalid Unit"));

	//if (!IsValid(unit))
	//	return false;

	//UE_LOG(LogTemp, Warning, TEXT("Unit %s is valid."), *unit->GetName());

	//if (isEnemy)	// enemy
	//{
	//	// if you had your turn already this round you cannot take another one
	//	if (ArrayContains(m_EnemyUnitsThatHadTurn, unit))
	//		return false;

	//	UE_LOG(LogTemp, Warning, TEXT("Enemy Unit %s is allowed to take its turn."), *unit->GetName());

	//}
	//else			// ally
	//{
	//	// if you had your turn already this round you cannot take another one
	//	if (ArrayContains(m_AllyUnitsThatHadTurn, unit))
	//		return false;

	//	UE_LOG(LogTemp, Warning, TEXT("Ally Unit %s is allowed to take its turn."), *unit->GetName());
	//}

	return true;
}