// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItemBase.h"
#include "TacticGame/Gameplay/Units/AdditionalComponents/HealthComponentBase.h"

bool AConsumableItemBase::BP_ConsumeItem()
{
	UnitHealthComp->IncreaseHealth(HealthAddition);
	return UnitHealthComp->isFull();
}