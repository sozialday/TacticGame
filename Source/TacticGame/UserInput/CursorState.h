#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TacticGame/UserInput/Interfaces/DynamicInputMethodInterfaceBase.h"
#include "CursorState.generated.h"

UENUM(Blueprintable)
enum EInputGameplayMethod : uint8
{
	Gamepad,
	KeyboardMouse
};

UCLASS()
class TACTICGAME_API ACursorState : public APlayerState, public IDynamicInputMethodInterfaceBase
{
	GENERATED_BODY()
	
	// interface function implementation

	virtual void SetInputMethod(TEnumAsByte<EGameplayInputMethod_FORBLUEPRINTS> NewInputMethod) override
	{
		uint8 conv_toNum = NewInputMethod;
		const auto inputMethod_conv = static_cast<EInputGameplayMethod>(conv_toNum);

		this->SetGameplayInputMethod(inputMethod_conv);
	}

	virtual void UpdateKey(FKey Key) override;
	virtual TEnumAsByte<EGameplayInputMethod_FORBLUEPRINTS> GetInput() override;

public:

	// directly sets up the input method [good for starup value]
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Input Method"))
	void SetGameplayInputMethod(TEnumAsByte<EInputGameplayMethod> NewInputMethod)
	{
		m_currentInputMethod = NewInputMethod;
	}
	
	// uses a key to determine what input method the player is using
	UFUNCTION(BlueprintCallable)
	void UpdateKeyInput(FKey Key);

	UFUNCTION(BlueprintPure)
	TEnumAsByte<EInputGameplayMethod> GetInputMethod()
	{
		return m_currentInputMethod;
	}
	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsGamepad inControl"))
	bool GetIsGamepadInputMethod()
	{
		return m_currentInputMethod == Gamepad;
	}

private:

	TEnumAsByte<EInputGameplayMethod> m_currentInputMethod;
};
