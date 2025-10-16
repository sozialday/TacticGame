// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PerkFullscreenListBase.generated.h"

/**
 * 
 */
UCLASS()
class TACTICGAME_API UPerkFullscreenListBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPerkFullscreenListBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent)
	class UGridPanel* GetGridPanel();

private:

	virtual void NativeOnInitialized() override;
	void CreateAndAddCardsToGrid();

	int Amount = 29;
	int CurrentAmount = 0;
	
	const int MaxAmount = 28;


	TSubclassOf<class UUserWidget> PerkCardWidgetClass;
	TSubclassOf<class USpacerWidget> SpacerWidgetClass;
};
