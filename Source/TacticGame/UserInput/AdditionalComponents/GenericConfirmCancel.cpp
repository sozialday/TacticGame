// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericConfirmCancel.h"
#include "Kismet/GameplayStatics.h"
#include "TacticGame/Gameplay/LevelPrerequisits/TacticGame_StageGamemodeBase.h"

// closes the fullscreen minimap
bool UFullscreenMinimapMenu::CancelAction()
{
	const auto& gamemode = Cast<ATacticGame_StageGamemodeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(gamemode))
		return false;

	// disable movement of the cursor
	m_CursorRef->EnableMovement();

	// request the gamemode to close the minimap overlay on the playerscreen
	gamemode->DestroyMinimapCameraManager();

	return Super::CancelAction();
}

// closes the inspection menu
bool UInspectionMenu::CancelAction()
{
	const auto& gamemode = Cast<ATacticGame_StageGamemodeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(gamemode))
		return false;

	m_CachedCamera->SetEnableInspectionMovement(false);
	
	if (const auto& playerscreen = gamemode->GetPlayerScreen())
		playerscreen->ShowPlayerScreen();

	if (IsValid(m_InspectionUI_Overlay))
	{
		// can be replaced with a blueprint function to smoothly remove it from the display
		m_InspectionUI_Overlay->RemoveFromParent();
	}
	if (IsValid(m_InspectionRenderCapture))
	{
		m_InspectionRenderCapture->Destroy();
	}

	return Super::CancelAction();
}