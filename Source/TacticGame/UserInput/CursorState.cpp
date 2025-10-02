// Fill out your copyright notice in the Description page of Project Settings.


#include "CursorState.h"

void ACursorState::UpdateKey(FKey Key)
{
	UpdateKeyInput(Key);
}
TEnumAsByte<EGameplayInputMethod_FORBLUEPRINTS> ACursorState::GetInput()
{
	return static_cast<EGameplayInputMethod_FORBLUEPRINTS>(static_cast<int>(GetInputMethod()));
}


// uses a key to determine what input method the player is using
void ACursorState::UpdateKeyInput(FKey Key)
{
	if (Key.IsGamepadKey())
	{
		SetGameplayInputMethod(Gamepad);
		return;
	}

	SetGameplayInputMethod(KeyboardMouse);
}