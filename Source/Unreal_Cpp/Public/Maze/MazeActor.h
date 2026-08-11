

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Maze/MazeData.h"
#include "MazeActor.generated.h"

class ACellActor;

UCLASS()
class UNREAL_CPP_API AMazeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Path 설정에 따라 문이 제대로 열리는지 테스트하기 위한 함수
	UFUNCTION(CallInEditor, Category = "Maze")
	void GenerateMaze();

	UFUNCTION(CallInEditor, Category = "Maze")
	void ClearMaze();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze")
	uint8 Width;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze")
	uint8 Height;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze")
	int32 Seed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze")
	TSubclassOf<ACellActor> CellUnitActor;

private:
	FMazeData MyMaze;

	UPROPERTY()
	TArray<ACellActor*> SpawnedCellActors;

	
	
};
