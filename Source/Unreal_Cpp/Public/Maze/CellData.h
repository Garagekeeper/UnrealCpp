

#pragma once

#include "CoreMinimal.h"
#include "Math/MathFwd.h"

// 방향을 나타내는 Enum
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDirType : uint8
{
	None = 0		UMETA(Hidden),
	North = 1 << 0	UMETA(DisplayName = "North"),
	East = 1 << 1	UMETA(DisplayName = "East"),
	South = 1 << 2	UMETA(DisplayName = "South"),
	West = 1 << 3	UMETA(DisplayName = "West"),
};


//Enum을 bitflags로 사용하려면 비트연산을 오버로딩 해야하는데
// 매크로를 통해서 쉽게 구현 가능
ENUM_CLASS_FLAGS(EDirType);

/**
 * 
 */
struct UNREAL_CPP_API FCellData
{
public:
	virtual ~FCellData() = default;

public:
	// 셀의 좌표를 가져오는 함수
	inline FIntPoint GetLocation() const { return FIntPoint(PosX, PosY); }
	inline void AddPath(EDirType InAddedPath) { PathCanGo |= InAddedPath; }
	inline bool IsPath(EDirType InCheck) const { return (PathCanGo & InCheck) == InCheck; }
	//inline bool IsPath(EDirType InCheck) { return EnumHasAnyFlags(PathCanGo, InCheck); }
	inline bool IsWall(EDirType InCheck) const { return !IsPath(InCheck); }

public:
	uint8 PosX = 0;
	uint8 PosY = 0;

	// 이 셀에서 진행 가능한 방향 (Bitflag)
	EDirType PathCanGo = EDirType::None;

	// 미로 생성 과정에서 이 셀이 미로에 포함되어 있는지 
	bool bInMaze = false;

	// 미로 생성 과정에서 다음셀을 기록하기 위한 변수
	FCellData* NextCell = nullptr;
};

struct UNREAL_CPP_API FEllerCellData : public FCellData
{
public:
	int SetID = 0;

public:
	virtual ~FEllerCellData() override = default;
};