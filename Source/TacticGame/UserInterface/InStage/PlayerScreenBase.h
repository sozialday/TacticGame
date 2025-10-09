// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerScreenBase.generated.h"


UCLASS()
class TACTICGAME_API UPlayerScreenBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void AddUnitDetailsToContainer(class UUserWidget* TargetWidget);

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveUnitDetailsFromContainer();

	UFUNCTION(BlueprintImplementableEvent)
	void SetVisibilityStatus(bool bVisible);

	// Attaches the Minimap to the Player Screen
	UFUNCTION(BlueprintImplementableEvent)
	void AttachMinimapToPlayerScreen(class UUserWidget* MinimapWidgetReference);

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveMinimapFromPlayerScreen(class UUserWidget* MinimapWidgetReference);

	// Visibility Function - Animated Hiding/Showing

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowPlayerScreen();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HidePlayerScreen();
};
