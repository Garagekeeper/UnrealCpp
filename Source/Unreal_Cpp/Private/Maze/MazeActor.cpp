


#include "Maze/MazeActor.h"
#include "Maze/CellActor.h"


// Sets default values
AMazeActor::AMazeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMazeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMazeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMazeActor::GenerateMaze()
{
	MyMaze.MakeMaze(Width, Height, Seed);
	TArray<FCellData> Maze = MyMaze.GetMaze();
	int32 YOffset = -(Width-1) * 2000 / 2;
	int32 XOffset = (Height-1) * 2000 / 2;
	for (auto Cell : Maze)
	{
	
		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = nullptr;
		SpawnParam.ObjectFlags = RF_Transient;

		FTransform NewTransform = GetTransform();
		FVector NewLocation = NewTransform.GetLocation();
		NewLocation.X += Cell.PosY * -2000 + XOffset;
		NewLocation.Y += Cell.PosX * 2000 + YOffset;
		NewTransform.SetLocation(NewLocation);

		ACellActor* Spawned = GetWorld()->SpawnActor<ACellActor>(CellUnitActor, NewTransform,SpawnParam);
		Spawned->InitializeCell(&Cell);
		SpawnedCellActors.Emplace(Spawned);
		//GetWorld()->SpawnActor(;
	}
}


void AMazeActor::ClearMaze()
{
	for (int i = SpawnedCellActors.Num() - 1; i >= 0; i--)
	{
		if(SpawnedCellActors[i])
			GetWorld()->DestroyActor(SpawnedCellActors[i]);
	}
	
	MyMaze.ClearMaze();
}

