// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DynamicInputMethodInterfaceBase.generated.h"

UENUM(Blueprintable, meta = (DisplayName = "Gameplay Input Method"))
enum EGameplayInputMethod_FORBLUEPRINTS : uint8
{
	Controller		UMETA(DisplayName = "Gamepad"),
	TastaturMaus	UMETA(DisplayName = "Keyboard/Mouse")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UDynamicInputMethodInterfaceBase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TACTICGAME_API IDynamicInputMethodInterfaceBase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// c++ only method
	virtual void SetInputMethod(TEnumAsByte<EGameplayInputMethod_FORBLUEPRINTS> NewInputMethod) = 0;

	UFUNCTION(BlueprintCallable, Category = "Input Detection", meta = (DisplayName = "Update Key Input"))
	virtual void UpdateKey(FKey Key) = 0;

	UFUNCTION(BlueprintCallable, Category = "Input Detection", meta = (DisplayName = "Get Current Input Method"))
	virtual TEnumAsByte<EGameplayInputMethod_FORBLUEPRINTS> GetInput() = 0;
};
