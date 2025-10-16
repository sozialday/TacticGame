// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TacticGame/UserInput/CursorBase.h"
#include "GenericConfirmCancel.generated.h"


// DO NOT INSTANTIATE THIS CLASS DIRECTLY - INSTEAD CREATE CHILD CLASSES FOR YOUR PURPOSES
UCLASS()
class TACTICGAME_API UGenericConfirmCancel : public UObject
{
	GENERATED_BODY()

public:

	UGenericConfirmCancel() = default;
	void Initialize(TObjectPtr<class ACursorBase> CursorRef)
	{
		m_CursorRef = CursorRef;
	}

	virtual bool ConfirmAction()
	{
		return true;
	}
	virtual bool CancelAction()
	{
		if (!IsValid(m_CursorRef))
			return false;

		// remove the reference of the handler from the cursor
		m_CursorRef->SetConfirmCancelHandler(nullptr);
		return true;
	}

	bool Is(TSubclassOf<class UGenericConfirmCancel> ClassReferenceToCheck)
	{
		return RID.Equals(ClassReferenceToCheck.GetDefaultObject()->RID);
	}

	UFUNCTION(BlueprintCallable)
	void SavePreviousMapping(class UGenericConfirmCancel* NowPreviousMapping)
	{
		m_previousMapping = NowPreviousMapping;
	}
	UFUNCTION(BlueprintPure)
	class UGenericConfirmCancel* GetPreviousMapping() const
	{
		return m_previousMapping;
	}

protected:

	// Reference Identification
	UPROPERTY()
	FString RID;

	UPROPERTY()
	TObjectPtr<class ACursorBase> m_CursorRef = nullptr;

	UPROPERTY()
	TObjectPtr<class UGenericConfirmCancel> m_previousMapping = nullptr;
};


UCLASS()
class TACTICGAME_API UInspectionMenu : public UGenericConfirmCancel
{
	GENERATED_BODY()

public:

	UInspectionMenu()
	{
		RID = "INSPECTION";
	}
	void Initialize(TObjectPtr<class ACursorBase> CursorRef, TObjectPtr<class AGameplayCameraBase> Camera,
		TObjectPtr<class UInspectionDetails_Window> InspectionDetails, TObjectPtr<class AInspectionWindowBG_Renderer> BG_Renderer)
	{
		m_CursorRef = CursorRef;
		m_CachedCamera = Camera;
		m_InspectionUI_Overlay = InspectionDetails;
		m_InspectionRenderCapture = BG_Renderer;
	}

	TObjectPtr<class UInspectionDetails_Window> GetUIReference() const
	{
		return m_InspectionUI_Overlay;
	}

private:

	virtual bool CancelAction() override;

	// Camera Reference
	UPROPERTY()
	TObjectPtr<class AGameplayCameraBase> m_CachedCamera = nullptr;
	
	// Details UI Reference [the bottom left one]
	UPROPERTY()
	TObjectPtr<class UInspectionDetails_Window> m_InspectionUI_Overlay = nullptr;
	
	// the render target actor reference
	UPROPERTY()
	TObjectPtr<class AInspectionWindowBG_Renderer> m_InspectionRenderCapture = nullptr;
};

UCLASS()
class TACTICGAME_API UFullscreenMinimapMenu : public UGenericConfirmCancel
{
	GENERATED_BODY()

public:

	UFullscreenMinimapMenu()
	{
		RID = "FULLSCREENMINIMAP";
	}
	void Initialize(TObjectPtr<class ACursorBase> CursorRef)
	{
		m_CursorRef = CursorRef;
	}

private:

	virtual bool CancelAction() override;
};

UCLASS(Blueprintable)
class TACTICGAME_API UPerkFullscreen_List : public UGenericConfirmCancel
{
	GENERATED_BODY()

public:

	UPerkFullscreen_List()
	{
		RID = "PerFullscreenList";
	}

	UFUNCTION(BlueprintCallable)
	void Initialize(class ACursorBase* CursorRef, class UPerkFullscreenListBase* PerkFullscreenListWidgetReference)
	{
		m_CursorRef = CursorRef;
		m_perkFullscreenWidget = PerkFullscreenListWidgetReference;
	}

private:

	virtual bool CancelAction() override;

	UPROPERTY()
	TObjectPtr<class UPerkFullscreenListBase> m_perkFullscreenWidget;
};