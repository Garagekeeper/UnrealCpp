


#include "Maze/MazeData.h"
#include "Maze/CellData.h"
#include "Maze/MazeActor.h"
#include <type_traits>

const FIntPoint  FMazeData::Direction[DirectionCnt] =
{
	FIntPoint(0,1),
	FIntPoint(1,0),
	FIntPoint(0,-1),
	FIntPoint(-1,0),
};

FMazeData::FMazeData()
{
	MazeGenAlgoTable.Add(EAlgorithmType::Wilson, &FMazeData::WilsonAlgorithmExecute);
	MazeGenAlgoTable.Add(EAlgorithmType::Eller, &FMazeData::EllerAlgorithmExecute);

	CellInstanceFuncTable.Add(EAlgorithmType::Wilson, [](){return new FCellData(); });
	CellInstanceFuncTable.Add(EAlgorithmType::Eller,  []()->FCellData* {return new FEllerCellData(); }); // 리턴타입 지정하고 싶을 때

}

FMazeData::~FMazeData()
{
	// 안전장치
	ClearMaze();
}

void FMazeData::MakeMaze(uint8 InWidth, uint8 InHeight, int32 InSeed, EAlgorithmType InType)
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


	Width = InWidth;
	Height = InHeight;


	GenerateEmptyMaze(InType);


	if (MazeGenAlgoTable.Find(InType))
	{
		// 해당 함수포인터를 
		// 이 객체(메모리)위에서 실행
		(this->*MazeGenAlgoTable[InType])();
	}
}

void FMazeData::GenerateEmptyMaze(EAlgorithmType InType)
{
	Cells.Reserve(Width * Height);
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			Cells.Add(CellInstanceFuncTable[InType]());
		}
	}
}

void FMazeData::ClearMaze()
{
	Width = 0;
	Height = 0;
	//Empty와 같지만 allocation을 지우지는 않는다(메모리는 남겨둠)
	//Cells.Reset();
	for (FCellData* Cell : Cells)
	{
		if (Cell != nullptr)
		{
			delete Cell;
			Cell = nullptr;
		}
	}
	Cells.Empty();
}

void FMazeData::WilsonAlgorithmExecute()
{
	//용향 확보하고 실제 만들기까지
	Cells.SetNum(Width * Height);

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

void FMazeData::EllerAlgorithmExecute()
{
	int32 Id = 1;

	for (int32 y = 0; y < Height; y++)
	{	
		FEllerCellData* CurrentCell;
		FEllerCellData* NextCell;

		// 0. 한 행의 셀들이 소속된 집합이 없으면 집합 ID를 부여
		for (int x = 0; x < Width; x++)
		{
			CurrentCell = static_cast<FEllerCellData*>(GetCell(x, y));
			CurrentCell->PosX = x;
			CurrentCell->PosY = y;
			if (CurrentCell->SetID == 0)
			{
				CurrentCell->SetID = Id++;
			}
		}

		// 1. 행의 첫칸부터 마지막 칸까지 수평 연결 탐색
		for (int x = 0; x < Width - 1; x++)
		{
			CurrentCell = static_cast<FEllerCellData*>(GetCell(x, y));
			NextCell = static_cast<FEllerCellData*>(GetCell(x + 1, y));

			// 1-1. 인접한 두 셀의 집합이 다르면 벽을 제거할지 말지 선택
			if (CurrentCell->SetID != NextCell->SetID)
			{
				// 약 50% 확률로 연결
				if (RandomStream.FRand() < 0.50f)
				{
					int32 OldSetID = NextCell->SetID;
					int32 NewSetID = CurrentCell->SetID;

					ConnectCells(CurrentCell, NextCell);

					// [수정 포인트] NextCell 하나만 바꾸는 게 아니라, 
					// 기존 NextCell과 같은 집합이었던 모든 셀의 ID를 NewSetID로 통합해 줍니다.
					for (int i = 0; i < Width; i++)
					{
						FEllerCellData* CheckCell = static_cast<FEllerCellData*>(GetCell(i, y));
						if (CheckCell->SetID == OldSetID)
						{
							CheckCell->SetID = NewSetID;
						}
					}
				}
			}
		}

		// 2. 현재 행과 다음 행 사이의 수직 경로 생성 (마지막 행이 아닌 경우)
		if (y < Height - 1)
		{
			TMap<int32, TArray<FEllerCellData*>> CellSetMap;
			for (int x = 0; x < Width; x++)
			{
				FEllerCellData* Cell = static_cast<FEllerCellData*>(GetCell(x, y));
				CellSetMap.FindOrAdd(Cell->SetID).Add(Cell);
			}

			// 2-1. 각 집합에서 최소 1개 이상을 아래쪽으로 무조건 연결
			for (auto& [_, Val] : CellSetMap)
			{
				CurrentCell = Val[RandomStream.RandRange(0, Val.Num() - 1)];
				NextCell = static_cast<FEllerCellData*>(GetCell(CurrentCell->PosX, CurrentCell->PosY + 1));
				NextCell->SetID = CurrentCell->SetID;
				NextCell->PosX = CurrentCell->PosX;
				NextCell->PosY = CurrentCell->PosY + 1;

				ConnectCells(CurrentCell, NextCell);
			}

			// 2-2. 나머지 Cell들도 임의로 수직 경로 생성
			for (int x = 0; x < Width; x++)
			{
				CurrentCell = static_cast<FEllerCellData*>(GetCell(x, y));
				NextCell = static_cast<FEllerCellData*>(GetCell(CurrentCell->PosX, CurrentCell->PosY + 1));
				if (CurrentCell->SetID != NextCell->SetID)
				{
					if (RandomStream.FRand() < 0.33f)
					{
						NextCell->SetID = CurrentCell->SetID;
						NextCell->PosX = CurrentCell->PosX;
						NextCell->PosY = CurrentCell->PosY + 1;
						ConnectCells(CurrentCell, NextCell);
					}
				}
			}
		}

		// 3. 마지막 행 처리: 서로 다른 집합 사이의 모든 벽을 제거하여 하나로 연결
		if (y == Height - 1)
		{
			for (int x = 0; x < Width - 1; x++)
			{
				CurrentCell = static_cast<FEllerCellData*>(GetCell(x, y));
				NextCell = static_cast<FEllerCellData*>(GetCell(x + 1, y));

				if (CurrentCell->SetID != NextCell->SetID)
				{
					int32 OldSetID = NextCell->SetID;
					int32 NewSetID = CurrentCell->SetID;

					ConnectCells(CurrentCell, NextCell);

					// 마지막 행에서도 완전히 집합을 병합해 주어야 사이클이 안 생깁니다.
					for (int i = 0; i < Width; i++)
					{
						FEllerCellData* CheckCell = static_cast<FEllerCellData*>(GetCell(i, y));
						if (CheckCell->SetID == OldSetID)
						{
							CheckCell->SetID = NewSetID;
						}
					}
				}
			}
		}
	}
}

FCellData* FMazeData::GetCell(uint8 InX, uint8 InY)
{
	if(!IsValidLocation(InX, InY))	return nullptr;

	return Cells[Locatoin2Index(InX, InY)];
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
