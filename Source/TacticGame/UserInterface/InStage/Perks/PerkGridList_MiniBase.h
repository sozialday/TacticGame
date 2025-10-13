// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PerkGridList_MiniBase.generated.h"

UCLASS()
class TACTICGAME_API UPerkGridList_MiniBase : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UFUNCTION(BlueprintCallable)
	void RedirectToPerListWindow(TSubclassOf<class UUserWidget> PerkFullscreen_List);
};
