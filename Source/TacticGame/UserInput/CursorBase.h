// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "TacticGame/UserInput/CursorState.h"
#include "TacticGame/Gameplay/Vision/GameplayCameraBase.h"
#include "TacticGame/Gameplay/LevelPrerequisits/LevelStateBase.h"
#include "TacticGame/Gameplay/Vision/InspectionWindowBG_Renderer.h"
#include "TacticGame/UserInterface/InStage/InspectionDetails_Window.h"
#include "TacticGame/UserInput/Interfaces/DynamicInputMethodInterfaceBase.h"

#include "TacticGame/UserInterface/InStage/PlayerScreenBase.h"
#include "TacticGame/Gameplay/LevelPrerequisits/TacticGame_StageGamemodeBase.h"

#include "CursorBase.generated.h"

UCLASS()
class TACTICGAME_API ACursorBase : public APawn
{
	GENERATED_BODY()

private:

	// Cursor Movement Parameters

	float GridSize;
	float AccelerationTime = 0.0f;
	float AccelerationValue = 0.0f;

	// Camera Zooming Parameters

	float m_ZoomLevel = 0.0f;

	// displays the current velocity of the cursor
	FVector m_currentVelocity = FVector::ZeroVector;
	bool m_DoOnce_SnappingToCharacter = true;

	// confirm / cancel handling
	UPROPERTY()
	TObjectPtr<class UGenericConfirmCancel> m_confirmcancelHandler = nullptr;

public:

	// set the Confirm / Cancel Handler
	UFUNCTION(BlueprintCallable)
	void SetConfirmCancelHandler(class UGenericConfirmCancel* NewConfirmCancelHandler);
	// checks if the handler is active
	bool HasConfirmCancelHandler() const;
	// checks if the handler has the class you are looking for
	bool IsHandlerSameClass(TSubclassOf<class UGenericConfirmCancel> HandlerClass);
	// get the current confirm / cancel Handler [Mapping]
	UFUNCTION(BlueprintPure)
	class UGenericConfirmCancel* GetCurrentConfirmCancelHandler() const
	{
		return m_confirmcancelHandler;
	}


	// Sets default values for this pawn's properties
	ACursorBase();

	// checks if a unit is on the cell to then display the cells it can move on
	void TraceDownForPlayerGridSelection();

	// retrives the camera offset arrow
	TObjectPtr<class UArrowComponent> GetCameraOffsetDirection() const { return m_CameraOffsetDirection_facing; }

	UFUNCTION(BlueprintCallable)
	void DisableMovement()
	{
		// m_MovementComponent->StopMovementImmediately();
		AccelerationTime = 0.0f;
		AccelerationValue = 0.0f;

		m_canMove = false;
	}

	UFUNCTION(BlueprintCallable)
	void EnableMovement()
	{
		m_canMove = true;
	}


	// DEVELOPMENT ONLY 
	// REPLACE THE ARROW WITH AN ACTUAL MODELL THAT YOU CAN MANIPULATE IN CODE

	UFUNCTION(BlueprintImplementableEvent)
	void SetCursorArrowDistanceSpeed(float NewZTarget);
	UFUNCTION(BlueprintImplementableEvent)
	void ResetCursorArrowDistance();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// will manually set the value so that the camera can follow the units movement
	void SetCameraCustomMovement_UnitMovement(bool NewcameraCustomMovement_UnitMovement = false);

	void RemoveUnitDetailsFromParent();

	UFUNCTION(BlueprintPure)
	class AUnitCharacterBase* GetSelectedUnit()
	{
		return m_SelectedUnit;
	}

	UFUNCTION(BlueprintPure)
	class AUnitCharacterBase* GetHoveredUnit()
	{
		return m_HoveredUnit;
	}

private:

	// confirm button
	void Confirm(FKey Key);
	// cancel button
	void Cancel(FKey Key);

	// Toggles the Minimap
	void ToggleMinimap(FKey Key);
	bool m_MinimapActive = false;	// boolean responsible for the minimap state

	// Moves the Camera upwards to make the GameplayCameraBase function as a fullscreen minimap
	void OpenMinimap_Fullscreen(FKey Key);
	// void CloseMinimap_Fullscreen(); -> handled by the FullscreenMinimapMenu
	bool m_inFullscreenMinimap = false;

	// for all the input actions
	void UpdateInputMethod(FKey Key)
	{
		Cast<IDynamicInputMethodInterfaceBase>(GetController()->PlayerState)->UpdateKey(Key);
	}
	// direct input method change 
	void SetInputMethod(TEnumAsByte<EInputGameplayMethod> InputGameplayMethod)
	{
		const uint8 conv_toNum = InputGameplayMethod;
		const auto inputMethod_conv = static_cast<EGameplayInputMethod_FORBLUEPRINTS>(conv_toNum);

		Cast<IDynamicInputMethodInterfaceBase>(GetController()->PlayerState)->SetInputMethod(inputMethod_conv);
	}

	// retrieves the cursors current velocity sized
	float GetCursorVelocity() const
	{
		return GetVelocity().Size();
	}

	// traces a sphere to the ground to move the cursor along the z axis to the surface
	FVector StickToGroundWithOffset(float ZOffset, float DeltaSeconds, FHitResult& HitResult);
	FVector StickToGroundWithOffset(float ZOffset, float DeltaSeconds, FHitResult& HitResult, FVector ActorLocation_selected);

	// checks if the character is on a Unit on the field
	void CheckUnitOnField(FHitResult HitResult, FName TagSelected);

	UPROPERTY()
	TObjectPtr<class ATacticGame_StageGamemodeBase> m_GamemodeReference = nullptr;

	UPROPERTY()
	TObjectPtr<class AGameplayGrid> GridInWorld = nullptr;

	// the widget which contains [limited] information of the unit the cursor currently is on
	UPROPERTY()
	TObjectPtr<class UUnitDetails_overlay_SHORT> m_unitDetailsUIOverlay = nullptr;
	UPROPERTY()
	TSubclassOf<class UUnitDetails_overlay_SHORT> m_unitDetailsClassReference_Blueprint = nullptr;

	// the widget containing the more detailed window of details for the player about the unit currently selected
	UPROPERTY()
	TSubclassOf<class UInspectionDetails_Window> m_InspectionUI_class = nullptr;
	UPROPERTY()
	TObjectPtr<class UInspectionDetails_Window> m_InspectionUI_Overlay = nullptr;

	// render target reference
	UPROPERTY()
	TObjectPtr<class AInspectionWindowBG_Renderer> m_InspectionRenderCapture = nullptr;

	// Kurve für Beschleunigung
	TObjectPtr<class UCurveFloat> AccelerationCurve;

	//	//	//
	//	//	//
	//	//	//

	TObjectPtr<class USceneComponent> m_Root = nullptr;

	// Arrow component to indicate the offset direction for the camera
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", DisplayName = "Camera Offset Direction"))
	TObjectPtr<class UArrowComponent> m_CameraOffsetDirection_facing = nullptr;

	// cached camera reference
	UPROPERTY()
	TObjectPtr<class AGameplayCameraBase> m_CachedCamera = nullptr;

	// cached cell selection plane reference
	UPROPERTY()
	TObjectPtr<class ACellSelectionPlane> m_CachedCellSelectionPlane = nullptr;

	// movement system for the cursor
	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UFloatingPawnMovement> m_MovementComponent = nullptr;

	// Camera Movement
	TObjectPtr<class URelativeCameraMovement> m_CameraTilting;

private:

	bool m_canMove = true;

	// will be false if the cursor moved one cell
	bool m_Once__CellMovedOnce = true;
	FVector m_oldPosition = FVector::ZeroVector;

	// Selection

	void SelectCell(FKey Key);
	void DeselectCell(FKey Key);

	UPROPERTY()
	TObjectPtr<class AUnitCharacterBase> m_SelectedUnit = nullptr;
	bool m_successfulSelection = false;

	UPROPERTY()
	TObjectPtr<class AUnitCharacterBase> m_HoveredUnit = nullptr;

	// Inspect Unit

	void Inspect_Unit(FKey Key);

	// Movement Input [Generic]

	void MoveForward(float value);
	void MoveRight(float value);

	// Camera Rotation Tilting [Generic]

	void TiltUp(float Rate);
	void TiltRight(float Rate);

	// Camera Zooming [Generic]

	void ZoomInOut(float value);

	// Toggle FastestPathSelection
	// will not be used by the player [option revoked]
	void TogglePathSelectionType(FKey Key)
	{
		UpdateInputMethod(Key);
		ALevelStateBase::SetPathSelectionType(ALevelStateBase::GetPathSelectionType() == CustomPath ? FastestPath : CustomPath);
	}

	/*
	*
	*	// is responsible to adjust the zoom level of the camera based on the zoom level selected and setup by the user
	*
	*	-1 = Min Zoom Level
	* 	 0 = Default Zoom Level
	* 	 1 = Max Zoom Level
	*/
	void AdjustZoomingLevel(float CurrentZoomLevel);
	
	// checks if both shoulder buttons are pressed to reset the zoom level
	void TryResetZoomLevel(FKey Key)
	{
		UpdateInputMethod(Key);

		if (Key.IsGamepadKey())
		{
			auto playercontroller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

			if (Key.GetFName() == EKeys::Gamepad_RightTrigger.GetFName())
			{
				if (playercontroller->IsInputKeyDown(EKeys::Gamepad_LeftTrigger))
					ResetZoomLevel();
			}
			else if (Key.GetFName() == EKeys::Gamepad_LeftTrigger.GetFName())
			{
				if (playercontroller->IsInputKeyDown(EKeys::Gamepad_RightTrigger))
					ResetZoomLevel();
			}

			return;
		}

		ResetZoomLevel();
	}
	// Resets the zoom level back to default
	void ResetZoomLevel();
};
