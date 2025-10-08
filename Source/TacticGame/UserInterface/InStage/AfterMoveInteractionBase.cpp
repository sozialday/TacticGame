// Fill out your copyright notice in the Description page of Project Settings.


#include "AfterMoveInteractionBase.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "TacticGame/Gameplay/Units/UnitCharacterBase.h"

FVector2D UAfterMoveInteractionBase::CalculateWidgetPositioning(class AUnitCharacterBase* Unit)
{
	if (!IsValid(Unit))
	{
		if (!IsValid(m_UnitReference))
			return FVector2D();
	}
	else
	{
		m_UnitReference = Unit;
	}
	

	const auto& pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!pawn)
		return FVector2D();

	const auto& playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController)
		return FVector2D();

	FVector worldLocation = Unit->GetActorLocation();
	{
		worldLocation += (pawn->GetActorRightVector() * 90.0f);
		worldLocation += FVector(0, 0, 150.0f);
	}

	// adjust transformation of the widget
	FVector2D NewRenderTranslation;
	if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(playerController, worldLocation, NewRenderTranslation, false))
	{
		return NewRenderTranslation;
	}

	return FVector2D();
}