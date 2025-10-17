// Fill out your copyright notice in the Description page of Project Settings.


#include "InspectionWindowBG_Renderer.h"

#include "Kismet/KismetMathLibrary.h"

#include "Engine/SceneCapture.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"

#include "TacticGame/Gameplay/Units/UnitCharacterBase.h"

AInspectionWindowBG_Renderer::AInspectionWindowBG_Renderer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCaptureComponent2D()->FOVAngle = 20.0f;
	GetCaptureComponent2D()->bCaptureEveryFrame = true;
	
	{
		static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderTexture
		(TEXT("/Game/Miscellaneous/FX/Textures/RenderTextures/RT_UnitRender_DetailedInspection.RT_UnitRender_DetailedInspection"));

		if (RenderTexture.Succeeded())
			GetCaptureComponent2D()->TextureTarget = RenderTexture.Object;
	}
}

void AInspectionWindowBG_Renderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!EnableTicking)
		return;

	const FVector FwdDir = UnitReference->GetActorForwardVector() * 260;
	SetActorLocation((FwdDir + (UnitReference->GetActorLocation() + FVector(0, 0, 40))));
	
	auto Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UnitReference->GetActorLocation());
	Rot.Pitch = 0;
	Rot.Roll = 0;

	SetActorRotation(Rot);
}