

#pragma once

#include "CoreMinimal.h"
#include "InventoryCommandTypes.generated.h"

class UItemDataAsset;

UENUM(BlueprintType)
enum class EInventoryCommandType :uint8
{
	None,
	Add,
	Move,
	Use,
	Drop,
	Money
};

USTRUCT(BlueprintType)
struct UNREAL_CPP_API FInventoryCommand
{

	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	EInventoryCommandType Type = EInventoryCommandType::Add;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	int32 Count = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	TObjectPtr<const UItemDataAsset> ItemData;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	int32 SourceIndex = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	int32 TargetIndex = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	FVector2D ScreenPostion = FVector2D::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	FVector Target3DPos = FVector::ZeroVector;

public:
	// 헬퍼 함수
	static FInventoryCommand MakeAdd(const UItemDataAsset* InItemData, int32 InCount = 1)
	{
		FInventoryCommand Command;
		Command.Type = EInventoryCommandType::Add;
		Command.ItemData = InItemData;
		Command.Count = InCount;
		return Command;
	}

	static FInventoryCommand MakeMove(const int32 From, const int32 To)
	{
		FInventoryCommand Command;
		Command.Type = EInventoryCommandType::Move;
		Command.SourceIndex = From;
		Command.TargetIndex = To;
		return Command;
	}

	static FInventoryCommand MakeDrop(const int32 InSlot, FVector TargetPos)
	{
		FInventoryCommand Command;
		Command.Type = EInventoryCommandType::Drop;
		Command.TargetIndex = InSlot;
		Command.Target3DPos = TargetPos;
		return Command;
	}

	static FInventoryCommand MakeUse(const int32 InSlot)
	{
		FInventoryCommand Command;
		Command.Type = EInventoryCommandType::Use;
		Command.TargetIndex = InSlot;
		return Command;
	}

	static FInventoryCommand MakeMoney(const int32 InDelta)
	{
		FInventoryCommand Command;
		Command.Type = EInventoryCommandType::Money;
		Command.Count = InDelta;
		return Command;
	}
};

USTRUCT(BlueprintType)
struct UNREAL_CPP_API FCommandResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bSuccess = false;

	UPROPERTY(BlueprintReadOnly)
	int32 RemainingCnt = 0;
};
