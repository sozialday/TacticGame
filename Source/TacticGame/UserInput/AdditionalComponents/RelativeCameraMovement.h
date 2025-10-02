// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RelativeCameraMovement.generated.h"

enum EAxisSelection
{
	Pitch, 
	Yaw, 
	Roll
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICGAME_API URelativeCameraMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URelativeCameraMovement();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// camera actor setup
	void SetCameraComponent(TObjectPtr<class AGameplayCameraBase> Camera)
	{
		m_cam = Camera;
		m_canTick = Camera ? true : false;
	}
		
	void SetRotationAxis(EAxisSelection AxisSelection, float Value)
	{
		switch (AxisSelection)
		{
		case Pitch:
			m_currentRotation.Pitch = Value;
			return;
		case Yaw:
			m_currentRotation.Yaw = Value;
			return;
		case Roll:
			m_currentRotation.Roll = Value;
			return;
		}
	}

private:

	bool m_canTick = false;

	UPROPERTY()
	TObjectPtr<class AGameplayCameraBase> m_cam = nullptr;

	// current camera rotation value
	FRotator m_currentRotation;
};
