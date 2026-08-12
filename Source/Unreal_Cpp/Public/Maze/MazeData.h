

#pragma once

#include "CoreMinimal.h"
#include "Maze/CellData.h"

struct FCellData;
enum class EAlgorithmType : uint8;
/**
 * 
 */
class UNREAL_CPP_API FMazeData
{
public:
	FMazeData();
	~FMazeData();

	//미로 생성
	void MakeMaze(uint8 InWidth, uint8 InHeight, int32 InSeed, EAlgorithmType InType);

	void GenerateEmptyMaze(EAlgorithmType InType);

	//미로 제거
	void ClearMaze();

	inline const TArray <FCellData*>& GetMaze() { return Cells; }

	// 특정 위치 셀 가져오기
	FCellData* GetCell(uint8 InX, uint8 InY);

private:
	// 윌슨 알고리즘을 실행
	void WilsonAlgorithmExecute();
	void EllerAlgorithmExecute();


	// 벽 부수기
	void ConnectCells(FCellData* InFrom, FCellData* InTo);

	// Direction에서 정의된 방향중 랜덤하게 하나를 선택해서 해당 방향의 Cell을 가져오기
	FCellData* GetRandomNeighborCell(const FCellData& InCell);

	// 배열 섞기
	void ShuffleArray(TArray<FCellData*>& InOutArray);

	inline uint16 Locatoin2Index(uint8 InX, uint8 InY) const { return InX + InY * Width; }
	inline bool IsValidLocation(uint8 InX, uint8 InY) const { return InX < Width && InY < Height; }

public:
	static constexpr int32 RandomSeed = -1;

private:
	// Rand walk가 가능한 방향의 수
	static constexpr uint8 DirectionCnt = 4;

	uint8 Width = 0;
	uint8 Height = 0;

	//TArray of All Cells in Maze
	TArray<FCellData*> Cells;

	// Stream for Randon Number
	FRandomStream RandomStream;
	//(U R D L)
	static const FIntPoint Direction[DirectionCnt];

	typedef void (FMazeData::* FMazeGenPtr)();
	TMap<EAlgorithmType, FMazeGenPtr> MazeGenAlgoTable;

	typedef FCellData* (*FCellCreatorPtr)();
	TMap<EAlgorithmType, FCellCreatorPtr> CellInstanceFuncTable;

};
