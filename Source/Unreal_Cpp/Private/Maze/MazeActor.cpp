


#include "Maze/MazeActor.h"
#include "Maze/CellActor.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
AMazeActor::AMazeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(Root);

	FloorHISM = CreateDefaultSubobject< UHierarchicalInstancedStaticMeshComponent>(TEXT("FloorHISM"));
	FloorHISM->SetupAttachment(Root);
	FloorHISM->SetCollisionProfileName(TEXT("BlockAll"));

	WallHISM = CreateDefaultSubobject< UHierarchicalInstancedStaticMeshComponent>(TEXT("WallHISM"));
	WallHISM->SetupAttachment(Root);
	WallHISM->SetCollisionProfileName(TEXT("BlockAll"));

	GateHISM = CreateDefaultSubobject< UHierarchicalInstancedStaticMeshComponent>(TEXT("GateHISM"));
	GateHISM->SetupAttachment(Root);
	GateHISM->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

// Called when the game starts or when spawned
void AMazeActor::BeginPlay()
{
	Super::BeginPlay();
	GenerateMaze();

}

// Called every frame
void AMazeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeActor::OnPreMazeGenerate()
{

	//월드상에 있는 네비게이션 시스템을 받아옴
	// NavMesh 자동 업데이트 막기
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSystem)
	{
		UNavigationSystemV1::SetNavigationAutoUpdateEnabled(false, NavSystem);
	}
}

void AMazeActor::OnPostMazeGenerate()
{

	//월드상에 있는 네비게이션 시스템을 받아옴
	// NavMesh 자동 업데이트 재개
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSystem)
	{
		NavSystem->Build();
		UNavigationSystemV1::SetNavigationAutoUpdateEnabled(true, NavSystem);
	}
}

TUniquePtr<FMazeData> AMazeActor::MakeMazeData()
{
	TUniquePtr<FMazeData> Maze = MakeUnique<FMazeData>();
	Maze->MakeMaze(Width, Height, Seed, AlgoType);

	return Maze;
}

void AMazeActor::SpawnCells(FMazeData* InMaze)
{
	if (!CellUnitActor) return;
	SpawnedCellActors.Reserve(Width * Height);

	// 클래스의 Default값 받아옥;
	const ACellActor* DefaultCell = CellUnitActor->GetDefaultObject<ACellActor>();

	//float CellHalfSize = DefaultCell->GEtCellHalfSize();
	float CellSize = CellHalfSize * 2.0f;

	FVector StartLoc = FVector(Height * CellHalfSize, -Width * CellHalfSize, 0)
		+ FVector((Height % 2) * -CellHalfSize, (Width % 2) * CellHalfSize, 0)
		+ GetTransform().GetLocation();

	UWorld* World = GetWorld();
	if (!World) return;

	//배열 기준 
	for (uint8 y = 0; y < Height; y++)
	{
		for (uint8 x = 0; x < Width; x++)
		{
			if (FCellData* Cell = InMaze->GetCell(x, y))
			{
				FVector CellLocation = StartLoc + FVector(-y * CellSize, x * CellSize, 0.0f);
				ACellActor* CellActor = World->SpawnActor<ACellActor>(
					CellUnitActor,
					CellLocation,
					FRotator::ZeroRotator);

				if (CellActor)
				{
					CellActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					CellActor->InitializeCell(Cell);
					SpawnedCellActors.Add(CellActor);
				}
			}
		}
	}
}

void AMazeActor::ClearSpawnedCells()
{
	for (int i = SpawnedCellActors.Num() - 1; i >= 0; i--)
	{
		if (IsValid(SpawnedCellActors[i]))
			SpawnedCellActors[i]->Destroy();
	}
	SpawnedCellActors.Empty();
}

void AMazeActor::BuildMazeHISM(FMazeData* InMaze)
{
	if (!InMaze) return;
	if (!FloorHISM->GetStaticMesh() || !WallHISM->GetStaticMesh() || !GateHISM->GetStaticMesh())
	{
		UE_LOG(LogTemp, Log, TEXT("there is static mesh in HISM component"));
		return;
	}

	ClearHISMInstances();

	float CellSize = CellHalfSize * 2.0f;
	FVector StartLoc = FVector(Height * CellHalfSize, -Width * CellHalfSize, 0)
		+ FVector((Height % 2) * -CellHalfSize, (Width % 2) * CellHalfSize, 0)
		+ GetTransform().GetLocation();

	for (uint8 y = 0; y < Height; y++)
	{
		for (uint8 x = 0; x < Width; x++)
		{
			FCellData* Cell = InMaze->GetCell(x, y);
			if (!Cell) continue;

			FVector CellLocation = StartLoc + FVector(-y * CellSize, x * CellSize, 0.0f);
			MakeCellHISM(Cell, CellLocation);
		}
	}
}

void AMazeActor::ClearHISMInstances()
{
	if (FloorHISM)
	{
		FloorHISM->ClearInstances();
	}

	if (WallHISM)
	{
		WallHISM->ClearInstances();
	}

	if (GateHISM)
	{
		GateHISM->ClearInstances();
	}

	//디버그 박스 지우기
	FlushPersistentDebugLines(GetWorld());

}

void AMazeActor::MakeCellHISM(const FCellData* InCell, const FVector& InLocation)
{
	if (!InCell) return;


	// 바닥 추가
	FloorHISM->AddInstance(FTransform(FRotator::ZeroRotator, InLocation));

	//벽 추가
	FVector Locattion;
	FRotator Rot;

	Locattion = InLocation + FVector::ForwardVector * CellHalfSize;
	Rot = FRotator::ZeroRotator;
	WallHISM->AddInstance(FTransform(Rot, Locattion));
	// 문 추가
	if (InCell->IsWall(EDirType::North))
	{
		GateHISM->AddInstance(FTransform(Rot, Locattion));
	}

	Locattion = InLocation + FVector::LeftVector * CellHalfSize;
	Rot = FRotator(0.0f, -90.0f, 0.0f);
	WallHISM->AddInstance(FTransform(Rot, Locattion));
	// 문 추가
	if (InCell->IsWall(EDirType::West))
	{
		GateHISM->AddInstance(FTransform(Rot, Locattion));
	}

	if (InCell->PosX == Width - 1)
	{
		Locattion = InLocation + FVector::RightVector * CellHalfSize;
		Rot = FRotator(0.0f, 90.0f, 0.0f);
		WallHISM->AddInstance(FTransform(Rot, Locattion));
		GateHISM->AddInstance(FTransform(Rot, Locattion));	// 동쪽 끝은 막혀 있는게 확정
	}
	if (InCell->PosY == Height - 1)
	{
		Locattion = InLocation + FVector::BackwardVector * CellHalfSize;
		Rot = FRotator(0.0f, 180.0f, 0.0f);
		WallHISM->AddInstance(FTransform(Rot, Locattion));
		GateHISM->AddInstance(FTransform(Rot, Locattion)); // 남쪽 끝은 막혀 있는게 확정
	}

#if WITH_EDITOR
	DrawDebugBox(
		GetWorld(),
		InLocation,
		FVector(CellHalfSize, CellHalfSize, 400.0f),
		FColor::Green,
		true
	);
#endif

}


void AMazeActor::GenerateMaze()
{
	OnPreMazeGenerate();

	ClearMaze();

	TUniquePtr<FMazeData> Maze = MakeMazeData();
	if (bUseHISM)
	{
		BuildMazeHISM(Maze.Get());
	}
	else
	{
		SpawnCells(Maze.Get());
	}

	OnPostMazeGenerate();

	//MyMaze.MakeMaze(Width, Height, Seed);
	//TArray<FCellData> Maze = MyMaze.GetMaze();
	//int32 YOffset = -(Width-1) * 2000 / 2;
	//int32 XOffset = (Height-1) * 2000 / 2;
	//for (auto Cell : Maze)
	//{
	//
	//	FActorSpawnParameters SpawnParam;
	//	SpawnParam.Owner = nullptr;
	//	SpawnParam.ObjectFlags = RF_Transient;

	//	FTransform NewTransform = GetTransform();
	//	FVector NewLocation = NewTransform.GetLocation();
	//	NewLocation.X += Cell.PosY * -2000 + XOffset;
	//	NewLocation.Y += Cell.PosX * 2000 + YOffset;
	//	NewTransform.SetLocation(NewLocation);

	//	ACellActor* Spawned = GetWorld()->SpawnActor<ACellActor>(CellUnitActor, NewTransform,SpawnParam);
	//	Spawned->InitializeCell(&Cell);
	//	SpawnedCellActors.Emplace(Spawned);
	//	//GetWorld()->SpawnActor(;
	//}
}


void AMazeActor::ClearMaze()
{
	if (bUseHISM)
	{
		ClearHISMInstances();
	}
	else
	{
		ClearSpawnedCells();
	}
}

