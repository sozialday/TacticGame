// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceAttackCameraController.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "TacticGame/UserInput/CursorBase.h"
#include "TacticGame/UserInterface/InStage/PlayerScreenBase.h"
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


	CameraTip = CreateDefaultSubobject<USceneComponent>(TEXT("CameraTip"));
	CameraTip->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);


	//CameraComponent = CreateDefaultSubobject<UCineCameraComponent>(TEXT("CameraComponent"));
	//CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	////
	//FCameraFilmbackSettings FilmbackSettings;
	//FilmbackSettings.SensorWidth = 70.410004;
	//FilmbackSettings.SensorHeight = 56.630001;

	//FCameraLensSettings CameraLensSettings;
	//CameraLensSettings.MinFocalLength = 50.0f;
	//CameraLensSettings.MaxFocalLength = 50.0f;
	//CameraLensSettings.MinFStop = 1.8f;
	//CameraLensSettings.MaxFStop = 22.0f;
	//CameraLensSettings.SqueezeFactor = 1.0f;

	//FCameraFocusSettings CameraFocusSettings;
	//CameraFocusSettings.FocusMethod = ECameraFocusMethod::Manual;
	//CameraFocusSettings.ManualFocusDistance = 80.0f;

	//CameraComponent->Filmback = FilmbackSettings;
	//CameraComponent->LensSettings = CameraLensSettings;
	//CameraComponent->FocusSettings = CameraFocusSettings;

	//CameraComponent->bConstrainAspectRatio = false;
	//CameraComponent->CurrentAperture = 5.0f;
}

// Called when the game starts or when spawned
void ADistanceAttackCameraController::BeginPlay()
{
	Super::BeginPlay();
	
	m_AllowCameraTransformationCalculation = true;
}

// smoothly interpolates the camera to the desired location and rotation
void ADistanceAttackCameraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(CameraComponent))
		return;

	if (m_AllowCameraTransformationCalculation)
	{
		TickCameraTransformation(DeltaTime);
	}

	// traces the distance from the camera to the a unit and sets the depth of field accordingly
	TickCameraDepthOfField(DeltaTime);
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

// fires the distance attack
void ADistanceAttackCameraController::Fire(FKey Key)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire"));
}

// cancels the distance attack and returns to the cursor
void ADistanceAttackCameraController::CancelAttack(FKey Key)
{
	float blendTime = 0.55f;

	if (const auto& camera = Cast<AGameplayCameraBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameplayCameraBase::StaticClass())))
	{
		const auto& playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		playerController->Possess(Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACursorBase::StaticClass())));
		playerController->SetViewTarget(camera);
		
		camera->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		camera->SetTickingEnabled(true);

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

				// re show the ui [if on the cell of a character]
				const auto& gamemode = Cast<ATacticGame_StageGamemodeBase>(UGameplayStatics::GetGameMode(GetWorld()));
				if (gamemode)
				{
					gamemode->GetPlayerScreen()->ShowPlayerScreen();
				}

		}, blendTime, false);
	}
}

// calculates the camera transformation each tick [smooth interpolation to its origin and rotation]
void ADistanceAttackCameraController::TickCameraTransformation(float DeltaSeconds)
{
	const auto& playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(playerController))
		return;

	FTransform CameraTransform = CameraComponent->GetRootComponent()->GetRelativeTransform();
	FTransform targetTransform;

	targetTransform.SetLocation(FMath::VInterpTo(CameraTransform.GetLocation(), FVector::ZeroVector, DeltaSeconds, 12));
	targetTransform.SetRotation(FMath::RInterpTo(CameraTransform.GetRotation().Rotator(), FRotator::ZeroRotator, DeltaSeconds, 8).Quaternion());

	CameraComponent->SetActorRelativeTransform(targetTransform);

	UE_LOG(LogTemp, Warning, TEXT("Camera Location: %s"), *CameraTransform.GetLocation().ToString());

	// check if the camera is close enough to the target location and rotation to then snap it to it

	if (FVector::Distance(CameraTransform.GetLocation(), FVector::ZeroVector) < 0.05f &&
		CameraTransform.GetRotation().Equals(FQuat::Identity, 0.01f))
	{
		CameraComponent->SetActorRelativeLocation(FVector::ZeroVector);
		CameraComponent->SetActorRelativeRotation(FRotator::ZeroRotator);

		m_AllowCameraTransformationCalculation = false;	// stop ticking -> without removing the camera reference, as it is needed for the depth of field calculations
	}
}

// adjusts the depth of field of the camera based on the distance to the target unit
void ADistanceAttackCameraController::TickCameraDepthOfField(float DeltaSeconds)
{
	FHitResult HitResult;
	const FVector End = CameraComponent->GetActorLocation() + (CameraComponent->GetActorForwardVector() * 10000.0f);

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		CameraComponent->GetActorLocation(), End,
		{ EObjectTypeQuery::ObjectTypeQuery8 }, false, {}, EDrawDebugTrace::None, HitResult, true))
	{
		if (IsValid(HitResult.GetActor()))
		{
			// Camera Focus Distance

			FCameraFocusSettings focusSettings = CameraComponent->GetCineCameraComponent()->FocusSettings;
			focusSettings.ManualFocusDistance = FMath::FInterpTo(focusSettings.ManualFocusDistance, HitResult.Distance, DeltaSeconds, 13.0);

			CameraComponent->GetCineCameraComponent()->FocusSettings = focusSettings;

		}
	}

	// Camera Aperture

	const float Aperture = FMath::FInterpTo(CameraComponent->GetCineCameraComponent()->CurrentAperture, 1.6, DeltaSeconds, 4.0);
	CameraComponent->GetCineCameraComponent()->CurrentAperture = Aperture;
}