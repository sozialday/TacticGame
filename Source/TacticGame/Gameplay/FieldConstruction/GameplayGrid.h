// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayGrid.generated.h"

UCLASS()
class TACTICGAME_API AGameplayGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameplayGrid();

    // adjusts the color of a specific cell in the grid
    void SetCellColor(int32 InstanceIndex, const FLinearColor& Color);
    // adjusts the colour of a specific list of cells in the grid
    void SetCellColor(TArray<int> InstanceIndicies, const FLinearColor& Color);

    void ClearAllCells();
    void TryClearCellsPreviouslySelected();

    // returns an integer containing all Instance Indicies using the tracing method of the input
    TArray<int> GetInstancesIndiciesFromTracing(FVector Origin, float SphereRadius, FVector BoxExtend = FVector::ZeroVector);

    // gets the Instanced Mesh
    UFUNCTION(BlueprintPure)
    class UInstancedStaticMeshComponent* GetInstancedMesh() const
    {
        return GridInstancedMesh;
    }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

    float CellSize; // Größe jeder Zelle im Grid
    UPROPERTY(EditAnywhere, Category = "Grid", meta = (AllowPrivateAccess = "true"))
    int32 GridWidth = 20;

    UPROPERTY(EditAnywhere, Category = "Grid", meta = (AllowPrivateAccess = "true"))
    int32 GridHeight = 20;

    UPROPERTY(EditAnywhere, Category = "Grid", meta = (AllowPrivateAccess = "true"))
    float ZOffset = 5.0f; // leicht über dem Boden, damit es nicht flackert

    UPROPERTY(EditAnywhere, Category = "Grid", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UStaticMesh> CellMesh;

    UPROPERTY(EditAnywhere, Category = "Grid", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UMaterialInterface> CellMaterial;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UInstancedStaticMeshComponent> GridInstancedMesh;

    // hold the information of the selected cells 
    // if the character is moving you can easily remove the selected cells without looping over the entire field again

    TArray<int> m_SelectedIndicies_tmp;
    int m_SelectedIndex_tmp = -1;
};
