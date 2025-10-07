// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DistanceAttackCameraController.generated.h"

UCLASS()
class TACTICGAME_API ADistanceAttackCameraController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADistanceAttackCameraController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	class USceneComponent* GetSpringArmTip() const
	{
		return CameraTip;
	}

private:

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCineCameraComponent> CameraComponent;

	TObjectPtr<class USceneComponent> CameraTip = nullptr;

private:

	//		// Simple Camera Movement Functions

	void LookUp(float Rate)
	{
		AddControllerPitchInput(Rate * -1);
	}
	void Turn(float Rate)
	{
		AddControllerYawInput(Rate);
	}

	void Fire(FKey Key);
	void CancelAttack(FKey Key);
};
