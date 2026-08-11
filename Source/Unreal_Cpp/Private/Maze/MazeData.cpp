


#include "Maze/MazeData.h"
#include "Maze/CellData.h"
#include "Maze/MazeActor.h"

const FIntPoint  FMazeData::Direction[DirectionCnt] =
{
	FIntPoint(0,1),
	FIntPoint(1,0),
	FIntPoint(0,-1),
	FIntPoint(-1,0),
};

FMazeData::~FMazeData()
{
	// 안전장치
	ClearMaze();
}

void FMazeData::MakeMaze(uint8 InWidth, uint8 InHeight, int32 InSeed)
{


	if (InSeed == RandomSeed)
	{
		// 랜덤하게 시드값 설정
		RandomStream.GenerateNewSeed();
	}
	else
	{
		// 정해진 값으로 시드값 설정
		RandomStream.Initialize(InSeed);
	}

	ClearMaze();
	//용량 확보만
	//Cells.Reserve(Width * Height);

	Width = InWidth;
	Height = InHeight;
	
	//용향 확보하고 실제 만들기까지
	Cells.SetNum(Width * Height);

	WillsonAlgorithmExecute();
}

void FMazeData::ClearMaze()
{
	Width = 0;
	Height = 0;
	//Empty와 같지만 allocation을 지우지는 않는다(메모리는 남겨둠)
	//Cells.Reset();
	Cells.Empty();
}

void FMazeData::WillsonAlgorithmExecute()
{
	// 기본 초기화
	TArray<FCellData*> NotInMazeCells;
	NotInMazeCells.Reserve(Width * Height);
	for (int32 y = 0; y < Height; y++)
	{
		for (int32 x = 0; x < Width; x++)
		{
			// 셀 초기화랑
			// 미로에 포함되지 않은 셀을 추가하는 걸 같이 수행
			FCellData* Cell = GetCell(x, y);
			Cell->PosX = x;
			Cell->PosY = y;
			NotInMazeCells.Add(Cell);
		}
	}

	// 미로에 포함되지 않은 셀들을 섞기
	ShuffleArray(NotInMazeCells);

	
	// 윌슨 알고리즘 시작
	// 미로가 아닌 셀 중 하나를 초기 셀로 지정하고 셀 목록에서 제거
	FCellData* InitCell = NotInMazeCells.Pop(); 
	InitCell->bInMaze = true;

	while (NotInMazeCells.Num() > 0)
	{
		// 미로가 아닌 셀 중 하나를 랜덤으로 선택
		FCellData* StartCell = NotInMazeCells.Pop();
		// 이미 미로에 있는 경우 건너뛰기
		if (StartCell->bInMaze) continue;

		FCellData* CurrentCell = StartCell;
		do
		{
			// 랜덤 워크 진행
			FCellData* NeighborCell = GetRandomNeighborCell(*CurrentCell);
			// 이전에 방문했던 경로를 재방문하면 값이 덮어써지기 때문에 경로가 바뀜
			CurrentCell->NextCell = NeighborCell;
			CurrentCell = NeighborCell;
		} while (!CurrentCell -> bInMaze);

		// 경로를 미로에 포함시키기
		FCellData* Path = StartCell;
		while (Path != CurrentCell)
		{
			Path->bInMaze = true;
			ConnectCells(Path, Path->NextCell);
			Path = Path->NextCell;
		}
	}
}

FCellData* FMazeData::GetCell(uint8 InX, uint8 InY)
{
	if(!IsValidLocation(InX, InY))	return nullptr;

	return &Cells[Locatoin2Index(InX, InY)];
}

void FMazeData::ConnectCells(FCellData* InFrom, FCellData* InTo)
{
	if (!InFrom || !InTo || (InFrom == InTo)) return;

	if (InFrom->PosX < InTo->PosX)
	{
		// To가 From의 R방향
		InFrom->AddPath(EDirType::East);
		InTo->AddPath(EDirType::West);
	}
	else if (InFrom->PosX > InTo->PosX)
	{
		InFrom->AddPath(EDirType::West);
		InTo->AddPath(EDirType::East);
	}
	else if (InFrom->PosY < InTo->PosY)
	{
		InFrom->AddPath(EDirType::South);
		InTo->AddPath(EDirType::North);
	}
	else if (InFrom->PosY > InTo->PosY)
	{
		InFrom->AddPath(EDirType::North);
		InTo->AddPath(EDirType::South);
	}
}

FCellData* FMazeData::GetRandomNeighborCell(const FCellData& InCell)
{
	FIntPoint NeighborLocation;
	do
	{
		int32 Index = RandomStream.RandRange(0, DirectionCnt - 1);
		NeighborLocation = InCell.GetLocation() + Direction[Index];
	} while (!IsValidLocation(NeighborLocation.X, NeighborLocation.Y));

	return GetCell(static_cast<uint8>(NeighborLocation.X), static_cast<uint8>(NeighborLocation.Y));
}

void FMazeData::ShuffleArray(TArray<FCellData*>& InOutArray)
{
	for (int i = InOutArray.Num() - 1; i > 0; i--)
	{
		int32 Index = RandomStream.RandRange(0, i);
		InOutArray.Swap(i, Index);
	}
}
