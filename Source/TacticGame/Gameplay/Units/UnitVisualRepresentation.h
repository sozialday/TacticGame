// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "UnitVisualRepresentation.generated.h"

UCLASS()
class TACTICGAME_API AUnitVisualRepresentation : public AActor
{
	GENERATED_BODY()

private:

	FVector GetPawnVelocity()
	{
		return UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetVelocity();
	}

	bool IsMoving()
	{
		return GetPawnVelocity().Size2D() > 50;
	}

public:	
	// Sets default values for this actor's properties
	AUnitVisualRepresentation();

	void SetSkeletalMesh(TObjectPtr<USkeletalMesh> NewMesh)
	{
		if (m_visualMesh && NewMesh)
			m_visualMesh->SetSkeletalMesh(NewMesh);
	}

	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> m_visualMesh;
};
