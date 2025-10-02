// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayGrid.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TacticGame/UserInput/CursorBase.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TacticGame/Gameplay/LevelPrerequisits/LevelStateBase.h"
#include "TacticGame/Functionality/TransformationLibrary/TransformationMatrixLibrary.h"


// Sets default values
AGameplayGrid::AGameplayGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    GridInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GridInstancedMesh"));
    SetRootComponent(GridInstancedMesh);

    GridInstancedMesh->SetStaticMesh(CellMesh);
    GridInstancedMesh->SetMaterial(0, CellMaterial);
    GridInstancedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// CHANGE THIS CODE BELOW FOR IT TO ACUALLY WORK


// Called when the game starts or when spawned
void AGameplayGrid::BeginPlay()
{
	Super::BeginPlay();
	
    if (!CellMesh || !CellMaterial) return;

	CellSize = ALevelStateBase::GetLevelGridSize();

    GridInstancedMesh->SetStaticMesh(CellMesh);
    GridInstancedMesh->SetMaterial(0, CellMaterial);
    GridInstancedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    /*
    * 0 = Grid Selection Colour R
	* 1 = Grid Selection Colour G
	* 2 = Grid Selection Colour B
	* 3 = Grid Selection Colour A for Grid Cells
	* 4 = Grid Seleciton Colour A for Grid Cell Outlines
    */
    GridInstancedMesh->SetNumCustomDataFloats(5);

    UWorld* World = GetWorld();
    if (!World) return;

	FVector Origin = UKismetMathLibrary::Vector_SnappedToGrid(GetActorLocation(), CellSize);

    for (size_t x = 0; x < GridWidth; x++)
    {
        for (size_t y = 0; y < GridHeight; y++)
        {
            FHitResult Hit;

            const FVector CellOffset = FVector(x * CellSize, y * CellSize, 0.0f);
			const FVector CellSpawnPos = Origin + CellOffset;

            const FVector Start = CellSpawnPos + FVector(0, 0, 3500);
            const FVector End = (GetActorUpVector() * -7500) + Start;

            const bool TraceHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End,
                { EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery7 }, false, { this },
                ALevelStateBase::GetTraceVisibility(), Hit, true);

            if (TraceHit)
            {
				// if there are any actors with this tag, ignore them for the grid generation
                if (Hit.GetActor()->ActorHasTag(ALevelStateBase::GetTag_IgnoreGameplayGridGeneration()))
					continue;

                FTransform InstanceTransform;
                //  //
				InstanceTransform.SetRotation(UTransformationMatrixLibrary::RotateAlongHitSurface(Hit, GetActorForwardVector()).Quaternion());
                InstanceTransform.SetLocation(FVector(CellSpawnPos.X, CellSpawnPos.Y, Hit.ImpactPoint.Z) + FVector(0, 0, ZOffset));
                InstanceTransform.SetScale3D(FVector(CellSize / 100.0f, CellSize / 100.0f, 1.0f));

                const int index = GridInstancedMesh->AddInstance(InstanceTransform);

                GridInstancedMesh->SetCustomDataValue(index, 0, 0.0);
                GridInstancedMesh->SetCustomDataValue(index, 1, 0.0);
                GridInstancedMesh->SetCustomDataValue(index, 2, 0.0);
                GridInstancedMesh->SetCustomDataValue(index, 3, 0.0);
                GridInstancedMesh->SetCustomDataValue(index, 4, 0.25);
            }
        }
    }
}

// Adjusts the color of a specific cell in the grid
void AGameplayGrid::SetCellColor(int32 InstanceIndex, const FLinearColor& Color)
{
    m_SelectedIndex_tmp = InstanceIndex;

    GridInstancedMesh->SetCustomDataValue(InstanceIndex, 0, Color.R);
    GridInstancedMesh->SetCustomDataValue(InstanceIndex, 1, Color.G);
    GridInstancedMesh->SetCustomDataValue(InstanceIndex, 2, Color.B);
    GridInstancedMesh->SetCustomDataValue(InstanceIndex, 3, Color.A);
}
// Adjusts the colour of a specific list of cells in the grid
void AGameplayGrid::SetCellColor(TArray<int> InstanceIndicies, const FLinearColor& Color)
{
    m_SelectedIndicies_tmp = InstanceIndicies;

    const auto& GridInWorld = Cast<AGameplayGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameplayGrid::StaticClass()));

    for (const auto& Index : InstanceIndicies)
    {
        FTransform Transform;
        GridInWorld->GetInstancedMesh()->GetInstanceTransform(Index, Transform);

        FHitResult Hit;

        // character on field [illegal field]
        bool IllegalField;
        FLinearColor TargetIllegalColour;

        IllegalField = UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(), Transform.GetLocation(), Transform.GetLocation(), FVector(90, 90, 200),
            FRotator::ZeroRotator, { EObjectTypeQuery::ObjectTypeQuery3 }, false, { this },
            ALevelStateBase::GetTraceVisibility(), Hit, true);
        
        if (IllegalField)
        {
            if (Hit.GetActor()->ActorHasTag(ALevelStateBase::GetTag_Characters_Allies()))
            {
                // it is an ally
                TargetIllegalColour = FLinearColor(1.0, 1.0, 1.0, 0.16);

            }
            else if (Hit.GetActor()->ActorHasTag(ALevelStateBase::GetTag_Characters_Enemies()))
            {
                // it is an enemy
                TargetIllegalColour = ALevelStateBase::GetUnMovableCellColor();
            }
        }


        SetCellColor(Index, IllegalField ? TargetIllegalColour : Color);
    }
}

TArray<int> AGameplayGrid::GetInstancesIndiciesFromTracing(FVector Origin, float SphereRadius, FVector BoxExtend)
{
    if (!GridInstancedMesh)
        return {};

    auto sphere_array = GridInstancedMesh->GetInstancesOverlappingSphere(Origin, SphereRadius, true);
    
    if (BoxExtend == FVector::ZeroVector)
        return sphere_array;

    auto box = UKismetMathLibrary::MakeBoxWithOrigin(Origin, BoxExtend);
    auto box_array = GridInstancedMesh->GetInstancesOverlappingBox(box, true);

    // to remove duplicates

    TSet<int> set_int = TSet<int>(sphere_array);
    set_int.Append(box_array);

    return set_int.Array();
}


void AGameplayGrid::ClearAllCells()
{
    int instances = GridInstancedMesh->GetInstanceCount();
    for (int i = 0; i < instances; i++)
    {
        SetCellColor(i, FLinearColor(0, 0, 0, 0));
    }

    m_SelectedIndex_tmp = -1;
    m_SelectedIndicies_tmp.Empty();
}
void AGameplayGrid::TryClearCellsPreviouslySelected()
{
    if (m_SelectedIndicies_tmp.IsEmpty())
    {
        if (m_SelectedIndex_tmp >= 0)
        {
            SetCellColor(m_SelectedIndex_tmp, FLinearColor(0, 0, 0, 0));
        }
    }
    else
    {
        for (const auto& indexSingle : m_SelectedIndicies_tmp)
        {
            SetCellColor(indexSingle, FLinearColor(0, 0, 0, 0));
        }
    }

    m_SelectedIndex_tmp = -1;
    m_SelectedIndicies_tmp.Empty();
}