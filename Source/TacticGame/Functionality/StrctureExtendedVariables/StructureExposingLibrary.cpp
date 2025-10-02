// Fill out your copyright notice in the Description page of Project Settings.


#include "StructureExposingLibrary.h"
#include "TacticGame/Gameplay/Units/UnitCharacterBase.h"

class AUnitCharacterBase* UStructureExposingLibrary::GetSelectedUnitFromStructure(struct FUnitDetails TargetStructure)
{
	return TargetStructure.Unit;
}