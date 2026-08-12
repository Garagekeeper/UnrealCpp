

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Maze/MazeData.h"
#include "MazeActor.generated.h"

class ACellActor;
class UHierarchicalInstancedStaticMeshComponent;
struct FCellData;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EAlgorithmType : uint8
{
	None = 0		UMETA(Hidden),
	Wilson = 1 << 0	UMETA(DisplayName = "Wilson"),
	Eller = 1 << 1	UMETA(DisplayName = "Eller"),
};

UCLASS()
class UNREAL_CPP_API AMazeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeActor();

	UFUNCTION(CallInEditor, Category = "Maze")
	void GenerateMaze();

	UFUNCTION(CallInEditor, Category = "Maze")
	void ClearMaze();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnPreMazeGenerate();
	virtual void OnPostMazeGenerate();

private:
	TUniquePtr<FMazeData> MakeMazeData();

	/*--------------------------
	*  CellActor Spawn Normal
	---------------------------*/
	void SpawnCells(FMazeData* InMaze);
	void ClearSpawnedCells();


	/*--------------------------
	* CellActor Spawn by HISM
	---------------------------*/
	void BuildMazeHISM(FMazeData* InMaze);
	void ClearHISMInstances();
	void MakeCellHISM(const FCellData* InCell, const FVector& InLocation);

public:


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze", meta = (ClampMin = "3", ClampMax = "100"))
	uint8 Width = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze", meta = (ClampMin = "3", ClampMax = "100"))
	uint8 Height = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze")
	int32 Seed = FMazeData::RandomSeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze")
	EAlgorithmType AlgoType = EAlgorithmType::Wilson;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|Performance")
	bool bUseHISM = true;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|HISM")
	float CellHalfSize = 1000.0f;


	/*--------------------------
	*  CellActor Spawn
	---------------------------*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maze|Spawn")
	TSubclassOf<ACellActor> CellUnitActor;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category ="Maze|Spawn")
	TArray<TObjectPtr<ACellActor>> SpawnedCellActors;

	/*--------------------------
	* CellActor Spawn by HISM
	---------------------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Components")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> FloorHISM = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Components")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> WallHISM = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Components")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> GateHISM = nullptr;

private:
};
