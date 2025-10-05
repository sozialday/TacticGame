// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceAttackCameraController.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "TacticGame/UserInput/CursorBase.h"
#include "TacticGame/UserInterface/InStage/Specials/DistanceAttackWidgetBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "E:/UE5_Default/UE_5.6/Engine/Source/Runtime/CinematicCamera/Public/CineCameraComponent.h"


// Sets default values
ADistanceAttackCameraController::ADistanceAttackCameraController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SetRootComponent(SpringArmComponent);
	//
	SpringArmComponent->TargetArmLength = 80.0f;
	SpringArmComponent->SocketOffset = FVector(0.0, 44.0, 80.0f);
	SpringArmComponent->ProbeChannel = ECC_Visibility;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCineCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	//
	FCameraFilmbackSettings FilmbackSettings;
	FilmbackSettings.SensorWidth = 70.410004;
	FilmbackSettings.SensorHeight = 56.630001;

	FCameraLensSettings CameraLensSettings;
	CameraLensSettings.MinFocalLength = 50.0f;
	CameraLensSettings.MaxFocalLength = 50.0f;
	CameraLensSettings.MinFStop = 1.8f;
	CameraLensSettings.MaxFStop = 22.0f;
	CameraLensSettings.SqueezeFactor = 1.0f;

	FCameraFocusSettings CameraFocusSettings;
	CameraFocusSettings.FocusMethod = ECameraFocusMethod::Manual;
	CameraFocusSettings.ManualFocusDistance = 80.0f;

	CameraComponent->Filmback = FilmbackSettings;
	CameraComponent->LensSettings = CameraLensSettings;
	CameraComponent->FocusSettings = CameraFocusSettings;

	CameraComponent->bConstrainAspectRatio = false;
	CameraComponent->CurrentAperture = 5.0f;
}

// Called when the game starts or when spawned
void ADistanceAttackCameraController::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called to bind functionality to input
void ADistanceAttackCameraController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Camera Movement [Rotation]
	PlayerInputComponent->BindAxis("Tilt CameraUp", this, &ADistanceAttackCameraController::LookUp);
	PlayerInputComponent->BindAxis("Tilt CameraRight", this, &ADistanceAttackCameraController::Turn);

	// Fire
	PlayerInputComponent->BindAction("Interact_Confirm", IE_Pressed, this, &ADistanceAttackCameraController::Fire);

	// Cancel
	PlayerInputComponent->BindAction("Interact_Cancel", IE_Pressed, this, &ADistanceAttackCameraController::CancelAttack);
}

void ADistanceAttackCameraController::Fire(FKey Key)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire"));
}

void ADistanceAttackCameraController::CancelAttack(FKey Key)
{
	float blendTime = 0.55f;

	if (const auto& camera = Cast<AGameplayCameraBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameplayCameraBase::StaticClass())))
	{
		const auto& playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		
		playerController->Possess(Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACursorBase::StaticClass())));
		playerController->SetViewTargetWithBlend(camera, blendTime, EViewTargetBlendFunction::VTBlend_Cubic, 2.6);

		TArray<UUserWidget*> foundWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), foundWidgets, UDistanceAttackWidgetBase::StaticClass());
		for (const auto& widget : foundWidgets)
		{
			widget->RemoveFromParent();
		}

		FTimerHandle count;
		GetWorldTimerManager().SetTimer(count, [playerController, this]()
		{
				Destroy();

		}, blendTime, false);
	}
}