// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "Components/ArrowComponent.h"
#include "GameplayCameraBase.generated.h"


UCLASS()
class TACTICGAME_API AGameplayCameraBase : public ACineCameraActor
{
	GENERATED_BODY()
	
	// if true, enables ticking for this actor
	bool m_enableTicking = false;

	bool m_enableTicking__Inspection = false;

public:

	AGameplayCameraBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// retrieves the current camera offset [will be added to the actor location after the direction calculation]
	FVector GetCameraOffset() const
	{
		return m_CameraOffset;
	}
	// retrieves the current distance (xy depending on the direction) from the cursor to the camera
	float GetCameraDistanceToCursor() const
	{
		return m_CameraDistanceToCursor;
	}

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UArrowComponent> m_ForwardFacing_ArrowComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", DisplayName = "Camera Offset"), Category = "Camera Parameters")
	FVector m_CameraOffset = FVector(0, 0, 600.0);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", DisplayName = "Distance to Cursor"), Category = "Camera Parameters")
	float m_CameraDistanceToCursor = 350.0f;


	// Blueprint Exposable Parameters to set in Editor


	/*
	*	// sets the minimum zoom offset for the camera
	*
	*
	*/
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Min Zoom Offset"), Category = "Zooming Offset|Translation Offset")
	FVector MinZoomOffset = FVector(100, 0, -200.0f);


	/*
	*	// sets the maximum zoom offset for the camera
	*
	*
	*/
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Min Zoom Offset"), Category = "Zooming Offset|Translation Offset")
	FVector MaxZoomOffset = FVector(-100, 0, 200.0f);


	/*
	*	// sets the minimum pitch offset for the camera
	*
	*
	*/
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Min Pitch Offset"), Category = "Zooming Offset|Rotation Offset")
	float MinPitchOffset = -5.0f;


	/*
	*	// sets the maximum pitch offset for the camera
	*
	*
	*/
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Max Pitch Offset"), Category = "Zooming Offset|Rotation Offset")
	float MaxPitchOffset = 5.0f;


	/*
	*	// sets the minimum distance offset for the camera
	*
	*
	*/
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Min Pitch Offset"), Category = "Zooming Offset|Camera Distance Offset")
	float MinCameraDistanceOffset = -200.0f;

	/*
	*	// sets the maximum distance offset for the camera
	*
	*
	*/
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Max Pitch Offset"), Category = "Zooming Offset|Camera Distance Offset")
	float MaxCameraDistanceOffset = 200.0f;



	//	//	//	//	//	//	//	//	//	//	//	//	//	//
	//	//	//	//	//	//	//	//	//	//	//	//	//	//

	// sets the Offset values to equalable values by adding them to the current zoom level
	void SetRelativeOffsetsToGlobalOffset()
	{
		// makes the offsets relative to the current camera offset and rotation

		MinZoomOffset = m_CameraOffset + MinZoomOffset;
		MaxZoomOffset = m_CameraOffset + MaxZoomOffset;
		//	//
		MinPitchOffset = GetActorRotation().Pitch - MinPitchOffset;
		MaxPitchOffset = GetActorRotation().Pitch - MaxPitchOffset;

		MinCameraDistanceOffset = m_CameraDistanceToCursor + MinCameraDistanceOffset;
		MaxCameraDistanceOffset = m_CameraDistanceToCursor + MaxCameraDistanceOffset;
	}

	void InspectionTicking(float DeltaSeconds);

public:

	// retrieves the root component of this actor
	TObjectPtr<class USceneComponent> GetRootComponent() const
	{
		return Super::GetRootComponent();
	}

	// adjusts the zooming offset added to the camera location every frame
	void SetCameraZoomingOffset(FVector NewOffset) 
	{
		m_CameraOffset = NewOffset;
	}
	// adjusts the rotation offset added to the camera rotation every frame
	void SetCameraRotationOffset(FRotator NewRotationOffset) 
	{
		auto actorrot = GetActorRotation();
		actorrot.Pitch = NewRotationOffset.Pitch;

		m_TargetCameraRotation = actorrot;
	}
	// adjusts the distance offset added to the camera location every frame
	void SetCameraDistanceOffset(float NewDistanceOffset) 
	{
		m_CameraDistanceToCursor = NewDistanceOffset;
	}

	// retrieves the forward facing arrow component
	TObjectPtr<class UArrowComponent> GetForwardFacingArrow() const { return m_ForwardFacing_ArrowComponent; }

	// retrieves the forward facing rotation
	FRotator GetMovementRotation() const { return m_ForwardFacing_ArrowComponent->GetComponentRotation(); }

	// Getter for MinZoomOffset
	FVector GetMinZoomOffset() const { return MinZoomOffset; }
	// Getter for MaxZoomOffset
	FVector GetMaxZoomOffset() const { return MaxZoomOffset; }
	// Getter for MinPitchOffset
	float GetMinPitchOffset() const { return MinPitchOffset; }
	// Getter for MaxPitchOffset
	float GetMaxPitchOffset() const { return MaxPitchOffset; }
	// Getter for MinCameraDistanceOffset
	float GetMinCameraDistanceOffset() const { return MinCameraDistanceOffset; }
	// Getter for MaxCameraDistanceOffset
	float GetMaxCameraDistanceOffset() const { return MaxCameraDistanceOffset; }

	bool GetCustomCameraMovement() const
	{
		return m_customCameraMovement;
	}

	UFUNCTION(BlueprintCallable)
	void SetTickingEnabled(bool EnableTicking = false)
	{
		m_enableTicking = EnableTicking;
	}

	UFUNCTION(BlueprintCallable)
	void SetCustomCameraMovement(bool EnableUseCustomCameraMovement = false)
	{
		m_customCameraMovement = EnableUseCustomCameraMovement;
	}
	
	UFUNCTION(BlueprintCallable)
	void SetCustomCameraLocation(FVector CameraLocation)
	{
		m_CameraLocation = CameraLocation;
	}
	UFUNCTION(BlueprintCallable)
	void SetCustomCameraRotation(FRotator CameraRotation)
	{
		m_CameraRotation = CameraRotation;
	}

	void SetEnableInspectionMovement(bool Enable = false)
	{
		m_enableTicking__Inspection = Enable;
	}

	void SetLocationAndRotationOfInspection(FVector Location, FRotator Rotation)
	{
		m_InspectionCameraLocation = Location;
		m_InspectionCameraRotation = Rotation;
	}

private:

	// cursor reference
	UPROPERTY()
	TObjectPtr<class ACursorBase> m_Cursor = nullptr;

	// Smooth Camera Rotation
	FRotator m_TargetCameraRotation;

	// custom Camera Location
	FVector m_CameraLocation;
	FRotator m_CameraRotation;

	bool m_customCameraMovement = false;

	// custom Camera Transformation : for inspection
	FVector m_InspectionCameraLocation;
	FRotator m_InspectionCameraRotation;

	// Depth of Field Values

};
