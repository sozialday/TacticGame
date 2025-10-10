// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinimapCameraTransformPointBase.generated.h"

UCLASS()
class TACTICGAME_API AMinimapCameraTransformPointBase : public AActor
{
	GENERATED_BODY()
	
private:

	TObjectPtr<class USceneComponent> m_Root = nullptr;

public:	
	// Sets default values for this actor's properties
	AMinimapCameraTransformPointBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	void PassCameraReference(TObjectPtr<class AGameplayCameraBase> CameraReference)
	{
		m_CameraReference = CameraReference;
	}
	void EnableCameraFollowing()
	{
		TraceToGroundForFocalDistance();
		m_enableCameraFollowing = true;
	}

private:

	void TraceToGroundForFocalDistance();

private:

	float m_cameraFocalDistance;

	bool m_enableCameraFollowing = false;

	UPROPERTY()
	TObjectPtr<class AGameplayCameraBase> m_CameraReference = nullptr;
};
