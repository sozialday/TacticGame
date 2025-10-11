
// Fill out your copyright notice in the Description page of Project Settings.


#include "CursorBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Components/ArrowComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TacticGame/Gameplay/Units/UnitCharacterBase.h"

#include "TacticGame/UserInterface/InStage/PlayerScreenBase.h"
#include "TacticGame/UserInterface/InStage/Details/UnitDetails_overlay_SHORT.h"

#include "TacticGame/TacticGameInstanceBase.h"
#include "TacticGame/UserInput/CursorPlayerController.h"
#include "TacticGame/UserInput/Visuals/CellSelectionPlane.h"
#include "TacticGame/Gameplay/FieldConstruction/GameplayGrid.h"

#include "TacticGame/Functionality/TracingLibrary/TracingLibrary.h"
#include "TacticGame/UserInput/AdditionalComponents/RelativeCameraMovement.h"

// for better confirm and cancel handling
#include "TacticGame/UserInput/AdditionalComponents/GenericConfirmCancel.h"

// Player Input Bindings [Gameplay Movement alone]
void ACursorBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Basic Movement [Keyboard]
	PlayerInputComponent->BindAxis("Movement_Fwd_Camera", this, &ACursorBase::MoveForward);
	PlayerInputComponent->BindAxis("Movement_Right_Camera", this, &ACursorBase::MoveRight);
	
	// Basic Camera Tilting [Keyboard]
	PlayerInputComponent->BindAxis("Tilt CameraUp", this, &ACursorBase::TiltUp);
	PlayerInputComponent->BindAxis("Tilt CameraRight", this, &ACursorBase::TiltRight);
	
	// Inspect Unit
	PlayerInputComponent->BindAction("Inspect Unit", IE_Pressed, this, &ACursorBase::Inspect_Unit);

	// Interact Selection
	PlayerInputComponent->BindAction("Interact_Confirm", IE_Pressed, this, &ACursorBase::Confirm);
	// Interact Cancel
	PlayerInputComponent->BindAction("Interact_Cancel", IE_Pressed, this, &ACursorBase::Cancel);

	// Minimap Toggle
	PlayerInputComponent->BindAction("ToggleMinimapVisiblity", IE_Pressed, this, &ACursorBase::ToggleMinimap);
	// Opens the Minimap in Fullscreen
	PlayerInputComponent->BindAction("OpenMinimap_Fullscreen", IE_Pressed, this, &ACursorBase::OpenMinimap_Fullscreen);

	// Camera Zoom [Keyboard]
	PlayerInputComponent->BindAxis("Zoom", this, &ACursorBase::ZoomInOut);

	// Zoom Reset
	PlayerInputComponent->BindAction("ResetCameraZoom", IE_Pressed, this, &ACursorBase::TryResetZoomLevel);
}

// Confirm Action [Select / Confirm Movement etc.]
void ACursorBase::Confirm(FKey Key)
{
	UpdateInputMethod(Key);

	if (HasConfirmCancelHandler())
	{

		return;
	}

	SelectCell(Key);
}
// Cancel Action [Cancel Movement / Close Menus etc.]
void ACursorBase::Cancel(FKey Key)
{
	UpdateInputMethod(Key);

	// if successful then do not do anything else
	if (m_confirmcancelHandler && m_confirmcancelHandler->CancelAction())
		return;

	DeselectCell(Key);
}


// set the Confirm / Cancel Handler
void ACursorBase::SetConfirmCancelHandler(TObjectPtr<class UGenericConfirmCancel> NewConfirmCancelHandler)
{
	m_confirmcancelHandler = NewConfirmCancelHandler;
}
// checks if the handler is active
bool ACursorBase::HasConfirmCancelHandler() const
{
	return IsValid(m_confirmcancelHandler);
}
// checks if the handler has the class you are looking for
bool ACursorBase::IsHandlerSameClass(TSubclassOf<class UGenericConfirmCancel> HandlerClass) const
{
	// if there is no handler, return true so that the player can open the minimap or whatever
	return IsValid(m_confirmcancelHandler) && IsValid(HandlerClass) && m_confirmcancelHandler->IsA(HandlerClass);
}


// will manually set the value so that the camera can follow the units movement
void ACursorBase::SetCameraCustomMovement_UnitMovement(bool NewcameraCustomMovement_UnitMovement)
{
	if (!m_CachedCamera)
		return;

	const auto& PlayerController = Cast<class APlayerController>(GetController());
	if (PlayerController)
	{
		if (NewcameraCustomMovement_UnitMovement)
		{
			// DisableInput(PlayerController);
		}
		else
		{
			// EnableInput(PlayerController);
			m_SelectedUnit = nullptr;
		}
	}

	m_CachedCamera->SetCustomCameraMovement(NewcameraCustomMovement_UnitMovement);
}

// tries to remove the unit details UI Element from the PlayerScreen
void ACursorBase::RemoveUnitDetailsFromParent()
{
	if (IsValid(m_unitDetailsUIOverlay))
		m_unitDetailsUIOverlay->RemoveFromParent();

	if (const auto& gamemode = Cast<ATacticGame_StageGamemodeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		gamemode->GetPlayerScreen()->RemoveUnitDetailsFromContainer();
}

// does a trace from the players position + ZOffset down to the ground
FHitResult TraceDownFromOffset(UWorld* World, FVector ActorLocation, FVector ActorUpVector, float ZOffset, TObjectPtr<class ACursorBase> self)
{
	TArray<FHitResult> Hits;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> obj_type;
	TArray<AActor*> ignore;

	{
		obj_type.Add(EObjectTypeQuery::ObjectTypeQuery1);
		obj_type.Add(EObjectTypeQuery::ObjectTypeQuery7);
		ignore.Add(self);
	}

	const FVector EndTrace = (ActorUpVector * -2500) + (ActorLocation + FVector(0, 0, ZOffset));
	UKismetSystemLibrary::SphereTraceMultiForObjects(World, ActorLocation, EndTrace, 25, obj_type, false, ignore, 
		ALevelStateBase::GetTraceVisibility(), Hits, true);

	for (const auto& hitsingle : Hits)
	{
		if (const auto& actorref = hitsingle.GetActor())
		{
			if (actorref->ActorHasTag(ALevelStateBase::GetTag_Gameplay_Ground()))
				return hitsingle;
		}
	}

	return FHitResult();
}
// does a box trace to the currently selected cell
FHitResult TraceCellAsBox(UWorld* World, TObjectPtr<class AActor> self, FName& TagHit)
{
	FHitResult Hit;
	const bool unitHit = UTracingLibrary::TraceUnit(World, self, TagHit, Hit);

	return Hit;
}

// Sets default values
ACursorBase::ACursorBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(m_Root);

	m_CameraTilting = CreateDefaultSubobject<URelativeCameraMovement>(TEXT("Camera Relative Movement"));

	m_MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("CursorMovementComponent"));
	m_MovementComponent->UpdatedComponent = RootComponent;
	// //
	m_MovementComponent->MaxSpeed = 1200.0;
	m_MovementComponent->Acceleration = 30000.0f;
	m_MovementComponent->Deceleration = 13000.0f;
	m_MovementComponent->TurningBoost = 5.0f;

	m_CameraOffsetDirection_facing = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraOffsetDirection_ArrowComponent"));
	m_CameraOffsetDirection_facing->SetupAttachment(RootComponent);
	m_CameraOffsetDirection_facing->SetAbsolute(false, true, false);

	// Acceleration Curve
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> tmp_AccelerationCurve
		(TEXT("/Game/Miscellaneous/Curves/FloatCurves/C_CursorMovementAccelerationCurve.C_CursorMovementAccelerationCurve"));

		if(tmp_AccelerationCurve.Succeeded())
			AccelerationCurve = tmp_AccelerationCurve.Object;
	}

	// Unit Details [bottom left] [Blueprint Class]
	{
		static ConstructorHelpers::FClassFinder<UUnitDetails_overlay_SHORT> unitDetails_overlay
		(TEXT("/Game/UserInterface/InStage/Overlays/WB_UnitDetails"));

		if (unitDetails_overlay.Succeeded())
			m_unitDetailsClassReference_Blueprint = unitDetails_overlay.Class;
	}

	// Inspection User Interface Class [Blueprint Class]
	{
		static ConstructorHelpers::FClassFinder<UInspectionDetails_Window> inspectionDetails
		(TEXT("/Game/UserInterface/InStage/WB_DetailedInspection"));

		if (inspectionDetails.Succeeded())
			m_InspectionUI_class = inspectionDetails.Class;
	}

}

// does a trace downwards (using a function) to then select the cells that the unit can move on
void ACursorBase::TraceDownForPlayerGridSelection()
{
	FName SelectedTag;
	const FHitResult HitSingle = TraceCellAsBox(GetWorld(), this, SelectedTag);

	if (!GridInWorld)
		GridInWorld = Cast<AGameplayGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameplayGrid::StaticClass()));


	CheckUnitOnField(HitSingle, SelectedTag);
}

// tries to find a cell selection plane in the world, if not then spawn and return a new one
TObjectPtr<class ACellSelectionPlane> GetCellSelectionPlane(UWorld* World, TObjectPtr<class AActor> self)
{
	const auto& foundPlane = UGameplayStatics::GetActorOfClass(World, ACellSelectionPlane::StaticClass());
	if (foundPlane)
		return Cast<ACellSelectionPlane>(foundPlane);

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return World->SpawnActor<ACellSelectionPlane>(ACellSelectionPlane::StaticClass(), self->GetActorLocation(), FRotator::ZeroRotator, spawnParams);
}

// Called when the game starts or when spawned
void ACursorBase::BeginPlay()
{
	Super::BeginPlay();

	// gets the grid size from the GameManager to store it for further use and to initially snap the Cursor the the closest Cell
	GridSize = ALevelStateBase::GetLevelGridSize();
	SetActorLocation(UKismetMathLibrary::Vector_SnappedToGrid(GetActorLocation(), GridSize));

	// retrieves the Grid from the World [if not successful then it will get pulled in the code later]
	GridInWorld = Cast<AGameplayGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameplayGrid::StaticClass()));

	// retrieves the gameplay camera from the world [only once]
	m_CachedCamera = Cast<AGameplayCameraBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameplayCameraBase::StaticClass()));

	// passes the gameplay camera to the camera rotation component for mouse / gamepad camera movement
	if (m_CameraTilting)
		m_CameraTilting->SetCameraComponent(m_CachedCamera);

	// retrieves the cell selection plane from the world [only once]
	m_CachedCellSelectionPlane = GetCellSelectionPlane(GetWorld(), this);
	if (m_CachedCellSelectionPlane)
		m_CachedCellSelectionPlane->SetCursorReference(this);
}

// to store the input values for acceleration calculation
float InputValues[2] = { 0,0 };

// Called every frame
void ACursorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// detection variable for the grid
	bool bHitBorder = false;

	// detects the next grid cell [based on the velocity of the cursor]
	// if nothing gets hit then stop the movement completely
	if (GridInWorld)
	{
		if (GetCursorVelocity() > 50.0f)
		{
			FVector Origin_box = GetActorLocation();
			{
				Origin_box.Z += 100.0f;

				FVector RoundedVelocity = GetVelocity().GetSafeNormal();
				for (int i = 0; i < 3; i++)
				{
					RoundedVelocity[i] = FMath::RoundToInt(RoundedVelocity[i]);
				}
				RoundedVelocity *= 90.0f;

				Origin_box += RoundedVelocity;
			}

			const FVector Start = UKismetMathLibrary::Vector_SnappedToGrid(Origin_box, ALevelStateBase::GetLevelGridSize());


			const auto& Box = UKismetMathLibrary::MakeBoxWithOrigin(Start, FVector(ALevelStateBase::GetCellDimension(), 800));
			const TArray<int> InstancedMeshIndicies = GridInWorld->GetInstancedMesh()->GetInstancesOverlappingBox(Box);

			if (InstancedMeshIndicies.IsEmpty())
				bHitBorder = true;
			else
				bHitBorder = false;
		}
	}

	// Berechne die Zielposition basierend auf der aktuellen Actor-Location
	FVector FinalLocation = GetActorLocation();

	// combined size of the input values
	const float SizeInput = FMath::Sqrt(FMath::Square(InputValues[0]) + FMath::Square(InputValues[1]));

	// accelerates the cursor movement based on the acceleration curve when the player is moving the cursor
	if ((GetCursorVelocity() > 10 || SizeInput != 0.0f) && !bHitBorder)
	{
		// removes the currently hovered unit
		m_HoveredUnit = nullptr;

		if (!m_DoOnce_SnappingToCharacter)
		{
			m_DoOnce_SnappingToCharacter = true;

			// everything below will only be executed once when the player is moving the cursor

			if (!m_successfulSelection)
			{
				// remove the ui element [unit details] from the screen
				RemoveUnitDetailsFromParent();

				if (!GridInWorld)
					GridInWorld = Cast<AGameplayGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameplayGrid::StaticClass()));

				GridInWorld->ClearAllCells();


				// notify the cell selection plane to continue moving 
				if (m_CachedCellSelectionPlane)
					m_CachedCellSelectionPlane->StartMovement();

				ResetCursorArrowDistance();
			}
		}

		m_currentVelocity = GetVelocity();

		if (AccelerationCurve)
		{
			AccelerationTime += (DeltaTime * 1.1);

			float MinTime, MaxTime;
			AccelerationCurve->GetTimeRange(MinTime, MaxTime);

			AccelerationTime = FMath::Clamp(AccelerationTime, 0.f, AccelerationCurve->GetFloatValue(MaxTime));
			AccelerationValue = AccelerationCurve->GetFloatValue(AccelerationTime);
		}
		else
		{
			AccelerationValue = 1.0f;
		}	
	}
	else if ((GetCursorVelocity() < 10 || SizeInput == 0.0f) || bHitBorder)	 // snap to grid when the player stops moving or when he lets go of the input
	{
		AccelerationTime = 0.0f;
		AccelerationValue = 0.0f;

		if (!m_successfulSelection)
		{
			// if tracing is successful snap the cursor to the traced character on the field
			if (m_DoOnce_SnappingToCharacter)
			{
				m_DoOnce_SnappingToCharacter = false;

				// everything below will only be executed once when the player stops moving the cursor

				// trace to find the next cell 
				// if it contains a character snap to it

				// if false it will trace again to get the character data [if available]
				// if true it won't trace again
				bool successful = false;
				FHitResult HitResult;

				const FVector EndTrace = (m_currentVelocity.GetSafeNormal() * 100.0f) + GetActorLocation();
				if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), GetActorLocation(), EndTrace, 60,
					{ EObjectTypeQuery::ObjectTypeQuery3 }, false, { this }, ALevelStateBase::GetTraceVisibility(), HitResult, true))
				{
					FName SelectedTag;
					if (UTracingLibrary::HasTags(HitResult.GetActor(), { ALevelStateBase::GetTag_Characters_Allies(), ALevelStateBase::GetTag_Characters_Enemies() }, SelectedTag))
					{
						// if here the actor hit is a character on the field
						m_HoveredUnit = Cast<AUnitCharacterBase>(HitResult.GetActor());

						FVector TargetCharacterLocation = HitResult.GetActor()->GetActorLocation();
						TargetCharacterLocation.Z = FinalLocation.Z;
						
						SetActorLocation(TargetCharacterLocation);	// snap immediately to the character
						FinalLocation = TargetCharacterLocation;
						successful = true; // do not trace again for the same data

						CheckUnitOnField(HitResult, SelectedTag);
					}
					else
					{
						m_HoveredUnit = nullptr;
					}
				}

				if (successful == false)
				{
					// Trace to find a character on the field currently on
					// if successful then retrieve the data for the movement and pass it to the grid class for further processing

					TraceDownForPlayerGridSelection();
				}
			}
		}
	
		// snaps the cursor the the closest point without considering the Z axis when the player stops moving
		{
			const FVector SnappedLocation = UKismetMathLibrary::Vector_SnappedToGrid(FinalLocation, GridSize);

			//	//

			const FVector Result = FMath::VInterpTo(GetActorLocation(), SnappedLocation, DeltaTime, 20.0f);

			FinalLocation.X = Result.X;
			FinalLocation.Y = Result.Y;
		}
	}

	//	//
	FHitResult HitResult_Sticking;
	// stick to ground with offset
	const FVector NewActorLocation_SnappedToGround = StickToGroundWithOffset(20.0f, DeltaTime, HitResult_Sticking, FinalLocation);
	SetActorLocation(NewActorLocation_SnappedToGround);

	// passes the current acceleration value to the cell selection plane to adjust its interpolation speed
	m_CachedCellSelectionPlane->SetAccelerationValue(AccelerationValue);
	// passes the hit result of the ground to the cell selection plane to align itself to the ground
	// + to adjust its Z position
	m_CachedCellSelectionPlane->PassHitResultOfGround(HitResult_Sticking);

	// movement calculation when the unit is moving
	if (m_CachedCamera && m_CachedCamera->GetCustomCameraMovement() && m_SelectedUnit)
	{
		const FRotator CursorRotation = GetCameraOffsetDirection()->GetComponentRotation();
		const FVector CameraLocation = (CursorRotation.Vector() * (-m_CachedCamera->GetCameraDistanceToCursor())) + 
			(m_SelectedUnit->GetActorLocation() + m_CachedCamera->GetCameraOffset());

		m_CachedCamera->SetCustomCameraLocation(FMath::VInterpTo(m_CachedCamera->GetActorLocation(), CameraLocation, DeltaTime, 2.5));
	}

	auto snappedActorLocation = UKismetMathLibrary::Vector_SnappedToGrid(GetActorLocation(), GridSize);

	if (snappedActorLocation.Equals(m_oldPosition, 10.0f))
	{
		if (m_Once__CellMovedOnce)
		{
			m_Once__CellMovedOnce = false;
			// everything here will be called once

			if (const auto& gameInstance = Cast<UTacticGameInstanceBase>(UGameplayStatics::GetGameInstance(GetWorld())))
			{
				// tries to play the force feedback when moving from one cell to another
				gameInstance->TryPlayForceFeedback(Cast<ACursorPlayerController>(GetController()),
					LoadObject<UForceFeedbackEffect>(nullptr, TEXT("/Game/Miscellaneous/Input/ForceFeedbacks/FFE_MoveCell.FFE_MoveCell")));
			}
		}	
	}
	else
	{
		m_Once__CellMovedOnce = true;
	}

	m_oldPosition = snappedActorLocation;
}

// Selects the current cell
void ACursorBase::SelectCell(FKey Key)
{
	if (m_successfulSelection)
	{
		if (!m_SelectedUnit)
			return;

		m_SelectedUnit->ReleaseUnit();
		m_successfulSelection = false;

		auto* gameManager = ALevelStateBase::GetGameManager(GetWorld());
		if (!gameManager)
			return;

		// applies the movement of the unit [request to the gamemanager class]
		// makes it so that the unit cannot be selected again this turn
		gameManager->TryTurn(m_SelectedUnit, false);

		if (m_CachedCellSelectionPlane)
			m_CachedCellSelectionPlane->SetPlaneColour();

		if (!m_CachedCamera)
			return;

		m_CachedCamera->SetCustomCameraMovement(true);
	}
	else
	{
		FName SelectedHit;
		const FHitResult HitSingle = TraceCellAsBox(GetWorld(), this, SelectedHit);
		if (const auto& Unit = Cast<AUnitCharacterBase>(HitSingle.GetActor()))
		{
			// if the actor is an ally unit
			if (!Unit->ActorHasTag(ALevelStateBase::GetTag_Characters_Allies()))
				return;

			auto* gameManager = ALevelStateBase::GetGameManager(GetWorld());
			if (!gameManager)
				return;

			// check if the unit can even be selected [request to the gamemanager class]
			if (!gameManager->VerifyIfUnitCanTakeTurn(Unit, false))
				return;

			// the character is an ally unit
			m_SelectedUnit = Unit;
			m_SelectedUnit->PickUnit();
			m_successfulSelection = true;

			if (m_CachedCellSelectionPlane)
				m_CachedCellSelectionPlane->SetPlaneColour(FColor::FromHex("FFD04BFF"));
		}
	}
}
// Deselect Cell [Cancel Movement]
void ACursorBase::DeselectCell(FKey Key)
{
	if (!m_successfulSelection)
		return;

	if (!m_SelectedUnit)
		return;

	if (GridInWorld)
		GridInWorld->ClearAllCells();

	m_SelectedUnit->Cancel();
	m_SelectedUnit->MovementProcessFinished(false);

	m_SelectedUnit = nullptr;
	m_successfulSelection = false;

	if (m_CachedCellSelectionPlane)
		m_CachedCellSelectionPlane->SetPlaneColour();
}

// inspects the currently selected unit
void ACursorBase::Inspect_Unit(FKey Key)
{
	UpdateInputMethod(Key);

	if (HasConfirmCancelHandler())
		return;

	FName HitTag;
	FHitResult Hit;
	if (!UTracingLibrary::TraceUnit(GetWorld(), this, HitTag, Hit))
		return;

	// if here the unit selected is a UnitCharacterBase at least

	const auto& Unit = Cast<AUnitCharacterBase>(Hit.GetActor());
	if (!IsValid(Unit))
		return;

	ResetZoomLevel();

	if (!m_CachedCamera)
		return;

	FVector Location;
	FRotator Rotation;

	m_CachedCamera->SetEnableInspectionMovement(true);

	{
		Rotation.Pitch = -10.0f;

		FVector ToCamDirection = GetCameraOffsetDirection()->GetComponentRotation().Vector();
		ToCamDirection = ToCamDirection * (-677) + GetActorLocation();
		ToCamDirection.Z += 210.0;

		Location = ToCamDirection;
	}

	m_CachedCamera->SetLocationAndRotationOfInspection(Location, Rotation);

	m_GamemodeReference = Cast<ATacticGame_StageGamemodeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (const auto& playerscreen = m_GamemodeReference->GetPlayerScreen())
	{
		playerscreen->HidePlayerScreen();
		// playerscreen->SetVisibilityStatus(false);
	}

	// render capturer spawning + setup
	m_InspectionRenderCapture = GetWorld()->SpawnActor<AInspectionWindowBG_Renderer>(AInspectionWindowBG_Renderer::StaticClass());
	if (m_InspectionRenderCapture)
	{
		// lets only the currently selected unit be visible on the capture
		m_InspectionRenderCapture->GetCaptureComponent2D()->ShowOnlyActorComponents(Unit);

		m_InspectionRenderCapture->AttachToComponent(Unit->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("pelvis"));
		m_InspectionRenderCapture->SetActorRelativeLocation(FVector(85, 260, 1));
		m_InspectionRenderCapture->SetActorRelativeRotation(FRotator(0.0, -101.0, 90.0));
	}

	m_InspectionUI_Overlay = CreateWidget<UInspectionDetails_Window>(GetWorld(), m_InspectionUI_class);
	if (m_InspectionUI_Overlay)
	{
		// pass the parameters to this ui for display [..]

		m_InspectionUI_Overlay->PassUnitCharacterDetails(Unit->GetUnitConfigurationData());
		m_InspectionUI_Overlay->AddToViewport();
	}

	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//

	{
		// should change after everything is setup
		// create and pass the confirm/cancel handler component
		TObjectPtr<UInspectionMenu> newHandler = NewObject<UInspectionMenu>(this);
		newHandler->Initialize(this, m_CachedCamera, m_InspectionUI_Overlay, m_InspectionRenderCapture);

		SetConfirmCancelHandler(newHandler);
	}
}

// Movement Forward
void ACursorBase::MoveForward(float value)
{
	if (!m_canMove)
		return;

	if (HasConfirmCancelHandler())
	{
		// do not allow movement when inspecting a unit
		if (IsHandlerSameClass(UInspectionMenu::StaticClass()))
			return;
	}

	InputValues[0] = value;

	if (!m_CachedCamera)
		return;

	// get the camera forward arrow to determine the movement direction
	const FVector MovementDirection_Fwd = m_CachedCamera->GetMovementRotation().Vector();

	AddMovementInput(MovementDirection_Fwd * AccelerationValue, value);
}
// Movement Right
void ACursorBase::MoveRight(float value)
{
	if (!m_canMove)
		return;

	if (HasConfirmCancelHandler())
	{
		// do not allow movement when inspecting a unit
		if (IsHandlerSameClass(UInspectionMenu::StaticClass()))
			return;
	}

	InputValues[1] = value;

	if (!m_CachedCamera)
		return;

	// get the camera right arrow to determine the movement direction
	const FVector MovementDirection_Right = FRotationMatrix(m_CachedCamera->GetMovementRotation()).GetScaledAxis(EAxis::Y);

	AddMovementInput(MovementDirection_Right * AccelerationValue, value);
}

// slight camera tilting [for more personality : totally optional does not need to be used in gameplay]
void ACursorBase::TiltUp(float Rate)
{
	bool CanTiltIntensive = false;
	if (HasConfirmCancelHandler())
	{
		CanTiltIntensive = IsHandlerSameClass(UInspectionMenu::StaticClass());
	}

	const float TiltingMultiplier = CanTiltIntensive ? 0.25 : 1.0;
	m_CameraTilting->SetRotationAxis(Pitch, (Rate * 3.0) * TiltingMultiplier);
}
void ACursorBase::TiltRight(float Rate)
{
	bool CanTiltIntensive = false;
	if (HasConfirmCancelHandler())
	{
		CanTiltIntensive = IsHandlerSameClass(UInspectionMenu::StaticClass());
	}

	const float TiltingMultiplier = CanTiltIntensive ? 0.25 : 1.0;
	m_CameraTilting->SetRotationAxis(Yaw, (Rate * 3.0) * TiltingMultiplier);
}

// Shows / Hides the Minimap from the PlayerScreen
void ACursorBase::ToggleMinimap(FKey Key)
{
	UpdateInputMethod(Key);

	const auto& gamemode = Cast<ATacticGame_StageGamemodeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (m_MinimapActive)
	{
		m_MinimapActive = false;
		gamemode->HideMinimapOnPlayerScreen();
	}
	else
	{
		m_MinimapActive = true;
		gamemode->ShowMinimapOnPlayerScreen();
	}
}

// Opens the Minimap in Fullscreen
// uses the GameplayCameraBase to do so
void ACursorBase::OpenMinimap_Fullscreen(FKey Key)
{
	UpdateInputMethod(Key);

	if (HasConfirmCancelHandler())
		return;

	const auto& gamemode = Cast<ATacticGame_StageGamemodeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(gamemode))
		return;

	// disable movement of the cursor
	this->DisableMovement();

	// request the gamemode to open the minimap overlay on the playerscreen
	gamemode->RequestMinimap_Fullscreen();

	// move the camera upwards to make it look like a fullscreen minimap
	{
		TObjectPtr<class AGameplayCameraBase> camera = nullptr;

		if (!IsValid(m_CachedCamera))
			camera = Cast<AGameplayCameraBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameplayCameraBase::StaticClass()));
		else
			camera = m_CachedCamera;

		// move the camera upwards to make it look like a fullscreen minimap
		gamemode->CreateAndFillInMinimapCameraManager(this, camera);
	}
	
	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//

	{
		// should change after everything is setup
		// create and pass the confirm/cancel handler component
		TObjectPtr<class UFullscreenMinimapMenu> newHandler = NewObject<UFullscreenMinimapMenu>(this);
		newHandler->Initialize(this);

		SetConfirmCancelHandler(newHandler);
	}
}

// tries to stay close to the ground (with offset) without interupting the XY Movement
FVector ACursorBase::StickToGroundWithOffset(float ZOffset, float DeltaSeconds, FHitResult& HitResult)
{
	// down facing trace
	FHitResult Ground = TraceDownFromOffset(GetWorld(), GetActorLocation(), GetActorUpVector(), 950, this);
	HitResult = Ground;

	// make it smoothly interpolate
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = FMath::FInterpTo(GetActorLocation().Z, (Ground.Location.Z + ZOffset), DeltaSeconds, 70.0f);

	return NewLocation;
}
FVector ACursorBase::StickToGroundWithOffset(float ZOffset, float DeltaSeconds, FHitResult& HitResult, FVector ActorLocation_selected)
{
	// down facing trace
	FHitResult Ground = TraceDownFromOffset(GetWorld(), ActorLocation_selected, GetActorUpVector(), 950, this);
	HitResult = Ground;

	// make it smoothly interpolate
	FVector NewLocation = ActorLocation_selected;
	NewLocation.Z = FMath::FInterpTo(ActorLocation_selected.Z, (Ground.Location.Z + ZOffset), DeltaSeconds, 70.0f);

	return NewLocation;
}

// checks if there are any units on the field you are currently on
void ACursorBase::CheckUnitOnField(FHitResult HitResult, FName TagSelected)
{
	if (!HitResult.bBlockingHit)
	{
		// removes all the unit detail ui's in case there are any duplicates

		TArray<UUserWidget*> AllWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), AllWidgets, UUnitDetails_overlay_SHORT::StaticClass(), false);
		for (const auto& widgetSingle : AllWidgets)
		{
			if (widgetSingle)
				widgetSingle->RemoveFromParent();
		}

		return;
	}

	const auto& UnitReference = Cast<AUnitCharacterBase>(HitResult.GetActor());
	if (UnitReference)
	{
		// notify the cell selection plane to stop moving 
		if (m_CachedCellSelectionPlane)
			m_CachedCellSelectionPlane->StopMovement();

		SetCursorArrowDistanceSpeed(-20);

		float sphere_radius = UnitReference->GetSphereRadius();
		FVector box_extend = UnitReference->GetBoxExtend();

		const auto indicies = GridInWorld->GetInstancesIndiciesFromTracing(UnitReference->GetActorLocation(), sphere_radius, box_extend);
		
		UnitReference->SetLegalFields(indicies);

		GridInWorld->ClearAllCells();

		const bool isAlly_orIllegalField = TagSelected == ALevelStateBase::GetTag_Characters_Allies();
		GridInWorld->SetCellColor(indicies, isAlly_orIllegalField ? ALevelStateBase::GetMovableCellColor() : ALevelStateBase::GetUnMovableCellColor());
	
		// the unit details [name, etc.]
		const auto& unit_config = UnitReference->GetUnitConfigurationData();

		m_unitDetailsUIOverlay = CreateWidget<UUnitDetails_overlay_SHORT>(GetWorld(), m_unitDetailsClassReference_Blueprint);
		if (m_unitDetailsUIOverlay)
		{
			m_unitDetailsUIOverlay->PassUnitDetails(unit_config);
			
			// get the gamemode which contains the playerscreen to add this ui to the Overlay Container [which has the position]
			if (const auto& Gamemode = Cast<ATacticGame_StageGamemodeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				Gamemode->GetPlayerScreen()->AddUnitDetailsToContainer(m_unitDetailsUIOverlay);
			}
		}
	}
}

// Zoom In / Out
void ACursorBase::ZoomInOut(float value)
{
	if (HasConfirmCancelHandler())
	{
		if (IsHandlerSameClass(UInspectionMenu::StaticClass()))
			return;
	}

	if (value != 0.0f && m_CachedCamera)
	{
		m_ZoomLevel = m_ZoomLevel + (-value * 0.05);
		m_ZoomLevel = FMath::Clamp(m_ZoomLevel, -1.0f, 1.0f);

		AdjustZoomingLevel(m_ZoomLevel);
	}
}

/*
*
* // is responsible to adjust the zoom level of the camera based on the zoom level selected and setup by the user
*
*	-1 = Min Zoom Level
* 	 0 = Default Zoom Level
* 	 1 = Max Zoom Level
*/
void ACursorBase::AdjustZoomingLevel(float CurrentZoomLevel)
{
	FVector NewZoomOffset;
	NewZoomOffset.X = UKismetMathLibrary::MapRangeClamped(CurrentZoomLevel, -1.0f, 1.0f, m_CachedCamera->GetMinZoomOffset().X, m_CachedCamera->GetMaxZoomOffset().X);
	NewZoomOffset.Y = UKismetMathLibrary::MapRangeClamped(CurrentZoomLevel, -1.0f, 1.0f, m_CachedCamera->GetMinZoomOffset().Y, m_CachedCamera->GetMaxZoomOffset().Y);
	NewZoomOffset.Z = UKismetMathLibrary::MapRangeClamped(CurrentZoomLevel, -1.0f, 1.0f, m_CachedCamera->GetMinZoomOffset().Z, m_CachedCamera->GetMaxZoomOffset().Z);

	m_CachedCamera->SetCameraZoomingOffset(NewZoomOffset);

	FRotator PitchRotationOffset;
	PitchRotationOffset.Pitch = UKismetMathLibrary::MapRangeClamped(CurrentZoomLevel, -1.0f, 1.0f, m_CachedCamera->GetMinPitchOffset(), m_CachedCamera->GetMaxPitchOffset());

	m_CachedCamera->SetCameraRotationOffset(PitchRotationOffset);

	const float DistanceOffset = UKismetMathLibrary::MapRangeClamped(CurrentZoomLevel, -1.0f, 1.0f, m_CachedCamera->GetMinCameraDistanceOffset(), m_CachedCamera->GetMaxCameraDistanceOffset());
	m_CachedCamera->SetCameraDistanceOffset(DistanceOffset);
}

// resets the zoom level back to its default value [smoothly]
void ACursorBase::ResetZoomLevel()
{
	if (HasConfirmCancelHandler())
	{
		if (IsHandlerSameClass(UInspectionMenu::StaticClass()))
			return;
	}
	

	m_ZoomLevel = 0.0f;
	AdjustZoomingLevel(0.0f);
}