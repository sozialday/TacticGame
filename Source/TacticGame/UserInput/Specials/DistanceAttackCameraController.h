// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "TacticGame/Gameplay/Vision/GameplayCameraBase.h"
#include "DistanceAttackCameraController.generated.h"

UCLASS()
class TACTICGAME_API ADistanceAttackCameraController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADistanceAttackCameraController();

	// passes the reference of the camera that will be attached to the spring arm tip
	UFUNCTION(BlueprintCallable)
	void PassCameraReference(class AGameplayCameraBase* camera)
	{
		CameraComponent = camera;
	}

	// attaches the passed camera to the spring arm tip and sets the player controller to possess this pawn and view through the camera
	UFUNCTION(BlueprintCallable)
	void AttachPassedGameplayCamera()
	{
		if (!CameraComponent)
			return;

		// attaches the camera to the spring arm tip
		CameraComponent->AttachToComponent(GetSpringArmTip(), FAttachmentTransformRules::KeepWorldTransform);

		const auto& playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (playerController)
		{
			playerController->Possess(this);
			playerController->SetViewTarget(CameraComponent);
		}
	}

	// does both: passes the camera reference and attaches it to the spring arm tip
	UFUNCTION(BlueprintCallable)
	void PassAndAttachCamera(class AGameplayCameraBase* camera)
	{
		PassCameraReference(camera);
		AttachPassedGameplayCamera();
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

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

	// Tip of the spring arm where the camera will be placed
	TObjectPtr<class USceneComponent> CameraTip = nullptr;

	// Gameplay Camera Reference -> will be attached to the spring arm and then smoothly moved to the desired location
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AGameplayCameraBase> CameraComponent = nullptr;

	bool m_AllowCameraTransformationCalculation = true;

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

	void TickCameraTransformation(float DeltaSeconds);
	void TickCameraDepthOfField(float DeltaSeconds);
};
