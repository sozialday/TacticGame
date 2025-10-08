// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TacticGame_StageGamemodeBase.generated.h"

UCLASS()
class TACTICGAME_API ATacticGame_StageGamemodeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ATacticGame_StageGamemodeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintPure)
	class UPlayerScreenBase* GetPlayerScreen() const
	{
		return PlayerScreen;
	}

	void ShowAfterMovementActionUIElement(TObjectPtr<class AUnitCharacterBase> UnitReference);

private:

	//		// Player Screen UI element

	UPROPERTY()
	TSubclassOf<class UPlayerScreenBase> PlayerScreen_ClassReferenceBlueprint = nullptr;

	UPROPERTY()
	TObjectPtr<class UPlayerScreenBase> PlayerScreen = nullptr;

	//		// After Move Interaction UI element

	UPROPERTY()
	TSubclassOf<class UAfterMoveInteractionBase> AfterMoveInteraction_ClassReferenceBlueprint = nullptr;
};
