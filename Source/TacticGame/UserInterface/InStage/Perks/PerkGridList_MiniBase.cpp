// Fill out your copyright notice in the Description page of Project Settings.


#include "PerkGridList_MiniBase.h"

#include "Kismet/GameplayStatics.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "TacticGame/UserInput/CursorBase.h"
#include "TacticGame/UserInput/AdditionalComponents/GenericConfirmCancel.h"

#include "TacticGame/UserInterface/InStage/Perks/PerkFullscreenListBase.h"

void UPerkGridList_MiniBase::RedirectToPerListWindow(TSubclassOf<class UUserWidget> PerkFullscreen_List)
{
	const auto& widget = CreateWidget<UUserWidget>(GetWorld(), PerkFullscreen_List);
	if (!IsValid(widget))
		return;

	const auto& cursor = Cast<ACursorBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACursorBase::StaticClass()));
	if (!IsValid(cursor))
		return;

	const auto& perkFullscreenWindow = NewObject<UPerkFullscreen_List>(this);
	perkFullscreenWindow->Initialize(cursor, Cast<UPerkFullscreenListBase>(widget));

	perkFullscreenWindow->SavePreviousMapping(cursor->GetCurrentConfirmCancelHandler());
	cursor->SetConfirmCancelHandler(perkFullscreenWindow);

	widget->AddToViewport();

	// input mode to the new ui
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(UGameplayStatics::GetPlayerController(GetWorld(), 0), widget);
}