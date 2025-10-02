// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementPath.h"
#include "Components/SplineComponent.h"

// Sets default values
AMovementPath::AMovementPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SetRootComponent(SplineComponent);
}

//void AMovementPath::DEBUG_ONLY_DrawSpline()
//{
//	const int32 numPoints = SplineComponent->GetNumberOfSplinePoints();
//	for (int32 i = 0; i < numPoints - 1; i++)
//	{
//		const FVector start = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
//		const FVector end = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World);
//		DrawDebugLine(GetWorld(), start, end, FColor::Red, true, 10.0f, 0, 5.0f);
//
//		UE_LOG(LogTemp, Warning, TEXT("Spline Point %d: %s"), i, *start.ToString());
//	}
//}

void AMovementPath::AddPoint(const FVector& Point)
{
	SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::World, true);
}
void AMovementPath::AddPoint(const FVector& Point, const FVector& Tangent)
{
	// until we have a way to set tangents directly, just ignore the tangent for now

	AddPoint(Point);
	// SplineComponent->AddSplinePoint()
}
