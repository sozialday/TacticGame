// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TacticGame\Functionality\StrctureExtendedVariables/GlobalStructureContainer.h"
#include "UnitStatsComponent.generated.h"

USTRUCT(Blueprintable)
struct FStatsValues
{
	GENERATED_BODY()

	FStatsValues()
	{

	}
	FStatsValues(int Attack, int Speed, int Luck, int Defense, int CritHitChance, int Resis)
	{
		this->Attack = Attack;
		this->Speed = Speed;
		this->Luck = Luck;
		this->Defense = Defense;
		this->CriticalHitChance = CritHitChance;
		this->Resistance = Resis;
	}

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "100", UIMin = "1", UIMax = "100"))
	int Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "100", UIMin = "1", UIMax = "100"))
	int Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "100", UIMin = "1", UIMax = "100"))
	int Luck;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Defense;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "100", UIMin = "1", UIMax = "100"))
	int CriticalHitChance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "100", UIMin = "1", UIMax = "100"))
	int Resistance;
};

UENUM(BlueprintType)
enum EStatTypeName : uint8
{
	ATK,
	SPD,
	LCK,
	DEF,
	CRIT,
	RES
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICGAME_API UUnitStatsComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UUnitStatsComponent();

	UFUNCTION(BlueprintPure)
	FStatsValues GetUnitStats()
	{
		return { Attack, Speed, Luck, Defense, CriticalHitChance, Resistance };
	}

	UFUNCTION(BlueprintCallable)
	void AddXP(float XPValue)
	{
		LevelProgression.AddXP(XPValue);
	}

	UFUNCTION(BlueprintPure)
	TEnumAsByte<EStatTypeName> GetStatTypeFromName(FText Stat)
	{
		FString Name = Stat.ToString();
		UEnum* EnumPtr = StaticEnum<EStatTypeName>();
		if (!EnumPtr) return EStatTypeName::ATK; // Fallback

		int64 Value = EnumPtr->GetValueByNameString(Name);
		if (Value == INDEX_NONE) return EStatTypeName::ATK; // Fallback

		return static_cast<EStatTypeName>(Value);
	}


private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool EnableUnitStatManipulation = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", EditCondition = "EnableUnitStatManipulation == true"))
	FStatsValues DEV_ONLY__UnitStats;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", EditCondition = "EnableUnitStatManipulation == true"))
	int DEV_ONLY__UnitLevel;

private:

	// Current XP Progress + Level
	FLevelProgression LevelProgression;

	// Stats

	// the strength of the unit when attacking [will be added to a damage calculation formula for damage decision]
	int Attack = 1;
	
	// the speed of the unit [will be responsible for the distance the unit can travel and the chance of retreating of a clash 
	// (luck is the other factor for escaping)]
	int Speed = 1;

	// the luck of the unit [will be responsible for the chance of retreating of a clash (speed is the other factor for escaping)
	// the other responsibility is how lucky you are in the game
	// the chance of certain things can be influenced to your advantage with a high luck value
	// the critical hit chance of the enemy can be reduced because of a high luck value]
	int Luck = 1;

	// the defense of the unit [influences the health value of the unit. Can be higher the higher this value is]
	int Defense = 1;

	// the base critical hit chance [the Luck parameter can further adjust the chance of a critical hit]
	int CriticalHitChance = 1;

	// the resistance value of certain magical attacks [fire resistance etc. The higher the value the lower the damage you get from 
	// certain magical attacks with extra parameters]
	int Resistance = 1;

public:

	// Getter Functions

	UFUNCTION(BlueprintPure, Category = "Stats")
	int GetCurrentLevel() const { return LevelProgression.Level; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	FLevelProgression GetLevelData() const { return LevelProgression; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	int GetAttack() const { return Attack; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	int GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	int GetLuck() const { return Luck; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	int GetDefense() const { return Defense; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	int GetCriticalHitChance() const { return CriticalHitChance; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	int GetResistance() const { return Resistance; }

	// Setter Functions

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetCurrentLevel(int NewLevel) { LevelProgression.Level = NewLevel; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetLevelData(const FLevelProgression& NewLevelData) { LevelProgression = NewLevelData; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetAttack(int NewAttack) { Attack = NewAttack; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetSpeed(int NewSpeed) { Speed = NewSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetLuck(int NewLuck) { Luck = NewLuck; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetDefense(int NewDefense) { Defense = NewDefense; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetCriticalHitChance(int NewCriticalHitChance) { CriticalHitChance = NewCriticalHitChance; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetResistance(int NewResistance) { Resistance = NewResistance; }


	// Increment Functions

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void IncreaseCurrentLevel() { LevelProgression.Level += 1; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void IncreaseAttack() { Attack += 1; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void IncreaseSpeed() { Speed += 1; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void IncreaseLuck() { Luck += 1; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void IncreaseDefense() { Defense += 1; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void IncreaseCriticalHitChance() { CriticalHitChance += 1; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void IncreaseResistance() { Resistance += 1; }

	// generic function 

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void IncreaseStatByText(FText StatText)
	{
		switch (GetStatTypeFromName(StatText))
		{
		case EStatTypeName::ATK:
			Attack += 1;
			break;

		case EStatTypeName::SPD:
			Speed += 1;
			break;

		case EStatTypeName::LCK:
			Luck += 1;
			break;

		case EStatTypeName::DEF:
			Defense += 1;
			break;

		case EStatTypeName::CRIT:
			CriticalHitChance += 1;
			break;

		case EStatTypeName::RES:
			Resistance += 1;
			break;

		default:
			break;
		}
	}

	UFUNCTION(BlueprintPure, Category = "Stats")
	int GetStatByText(FText StatText)
	{
		switch (GetStatTypeFromName(StatText))
		{
		case EStatTypeName::ATK:
			return Attack;

		case EStatTypeName::SPD:
			return Speed;

		case EStatTypeName::LCK:
			return Luck;

		case EStatTypeName::DEF:
			return Defense;

		case EStatTypeName::CRIT:
			return CriticalHitChance;

		case EStatTypeName::RES:
			return Resistance;

		default:
			break;
		}

		return -1;
	}
};
