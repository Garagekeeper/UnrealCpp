


#include "Component/InventoryComponent.h"
#include "Framework/SubSystem/PickupFactorySubsystem.h"
#include "Widget/TemporaryWidget.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	Slots.SetNum(InventorySize + 1);
}

bool UInventoryComponent::ExecuteCommand(const FInventoryCommand& Command, FCommandResult& OutResult)
{
	switch (Command.Type)
	{
		case EInventoryCommandType::Add:
			HandleAddCommand(Command.ItemData, Command.Count, OutResult);
			break;
		case EInventoryCommandType::Move:
			HandleMoveCommand(Command.SourceIndex, Command.TargetIndex, OutResult);
			break;
		case EInventoryCommandType::Use:
			HandleUseCommand(Command.TargetIndex, OutResult);
			break;
		case EInventoryCommandType::Drop:
			HandleDropCommand(Command.TargetIndex, Command.Target3DPos, OutResult);
			break;
		case EInventoryCommandType::Money:
			HandleMoneyCommand(Command.Count, OutResult);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Invalid InventoryCommad"));
			break;
	}
	return OutResult.bSuccess;
}

void UInventoryComponent::AddMoney(int32 InIncome)
{
	Money += InIncome;
	OnMoneyChaneged.Broadcast(Money);
}

int32 UInventoryComponent::AddItem(const UItemDataAsset* InItemData, int32 InCount)
{
	if (!InItemData)
	{
		UE_LOG(LogTemp, Log, TEXT("[UInventoryComponent::AddItem] : InItemData was nullptr"));
		return InCount;
	}
	else if (InCount < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[UInventoryComponent::AddItem] : InCount was below zero"));
		return InCount;
	}
	int32 RemainCnt = InCount;

	int32 StartIndex = 0;

	while (RemainCnt > 0)
	{
		int32 FoundIndex = FindSlotWithItem(InItemData, StartIndex);
		if (FoundIndex == InValidSlot) break;

		FInventorySlot& Slot = Slots[FoundIndex];
		int32 AmountToAdd = FMath::Min(Slot.GetRemainCnt(), RemainCnt);

		UpdateSlotCount(FoundIndex, AmountToAdd);
		RemainCnt -= AmountToAdd;
		StartIndex = FoundIndex + 1;

	}

	// 빈칸 찾기
	while (RemainCnt > 0)
	{
		int32 FoundIndex = FindEmptySlot();
		if (FoundIndex == InValidSlot) break;

		FInventorySlot& Slot = Slots[FoundIndex];
		int32 AmountToAdd = FMath::Min(InItemData->MaxStackCnt, RemainCnt);

		PlaceItem2Slot(FoundIndex, InItemData, AmountToAdd);
		RemainCnt -= AmountToAdd;
		StartIndex = FoundIndex + 1;

	}

	return RemainCnt;
}

void UInventoryComponent::UseItem(int32 InIndex)
{

}

FInventorySlot* UInventoryComponent::GetSlot(int InSlotIndex)
{
	check(IsValidIndex(InSlotIndex) && "[UInventoryComponent::GetSlot] : invalid array index");
	//if (!IsValidIndex(InSlotIndex)) return nullptr;
	return &Slots[InSlotIndex];
}

FInventorySlot* UInventoryComponent::GetTempSlot()
{
	// 마지막 원소가 임시 배열임
	return &Slots[InventorySize];
}

int32 UInventoryComponent::FindSlotWithItem(const UItemDataAsset* InItemData, int32 InStartIndex)
{
	int32 Res = InValidSlot;

	for (int32 i = InStartIndex; i < InventorySize; i++)
	{
		if (Slots[i].ItemData == InItemData && !Slots[i].IsFull())
		{
			Res = i;
			break;
		}
	}
	return Res;
}

int32 UInventoryComponent::FindEmptySlot()
{
	int32 Res = InValidSlot;

	for (int32 i = 0; i < InventorySize; i++)
	{
		if (Slots[i].IsEmpty())
		{
			Res = i;
			break;
		}
	}

	return Res;
}

void UInventoryComponent::PlaceItem2Slot(int32 InSlotIndex, const UItemDataAsset* InItemData, int32 InCount)
{
	if (!IsValidIndex(InSlotIndex)) return;
	if (InItemData)
	{
		if (!InItemData->IsLoaded())
		{
			InItemData->RequestDataLoad(
				FStreamableDelegate::CreateWeakLambda(
					this,
					[this, InSlotIndex]()
					{
						UE_LOG(LogTemp, Log, TEXT("Set Slot: Asyc load compelete"));
						OnSlotChanged.ExecuteIfBound(InSlotIndex);
					}
				)
			);
		}
	}

	FInventorySlot& Slot = Slots[InSlotIndex];
	Slot.ItemData = InItemData;
	Slot.SetCnt(InCount);

	//TODO Delegate
	OnSlotChanged.ExecuteIfBound(InSlotIndex);
}

void UInventoryComponent::UpdateSlotCount(int32 InSlotIndex, int32 InDeltaCount)
{
	check(IsValidIndex(InSlotIndex) && "[UInventoryComponent::UpdateSlotCount] : Accessing with invalid array index");
	FInventorySlot& Slot = Slots[InSlotIndex];

	if (Slot.IsEmpty())return;

	int32 NewCount = Slot.GetCnt() + InDeltaCount;
	PlaceItem2Slot(InSlotIndex, Slot.ItemData, NewCount);
}

void UInventoryComponent::ClearSlot(int32 InSlotIndex)
{
	PlaceItem2Slot(InSlotIndex, nullptr, 0);
}

bool UInventoryComponent::HandleAddCommand(const UItemDataAsset* InItemData, int32 InCount, FCommandResult& OutResult)
{

	int32 Res = AddItem(InItemData, InCount);
	if (Res > 0)
	{
		OutResult.bSuccess = false;
		OutResult.RemainingCnt = Res;
	}
	else
	{
		OutResult.bSuccess = true;
		OutResult.RemainingCnt = 0;
	}

	if (OutResult.bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("add completed"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("add Incompleted %d remain"), OutResult.RemainingCnt);
	}

	return OutResult.bSuccess;
}

bool UInventoryComponent::HandleMoveCommand(const int32 From, const int32 To, FCommandResult& OutResult)
{
	if (!IsValidIndex(From)) return OutResult.bSuccess = false;
	if (!IsValidIndex(To)) return OutResult.bSuccess = false;
	if (Slots[From].IsEmpty()) return OutResult.bSuccess = false;

	if (Slots[From].ItemData == Slots[To].ItemData)
	{
		// From에서 TO로 옮길 수 있는 개수
		int32 AmountToAdd = FMath::Min(Slots[To].GetRemainCnt(), Slots[From].GetCnt());
		if (AmountToAdd > 0)
		{
			UpdateSlotCount(To, AmountToAdd);
			UpdateSlotCount(From, -AmountToAdd);
			return OutResult.bSuccess = true;
		}
	}

	Swap(Slots[From], Slots[To]);
	OnSlotChanged.ExecuteIfBound(From);
	OnSlotChanged.ExecuteIfBound(To);

	return OutResult.bSuccess = true;
}

bool UInventoryComponent::HandleDropCommand(const int32 InSlot, FVector InPos, FCommandResult& OutResult)
{
	if (!IsValidIndex(InSlot)) return OutResult.bSuccess = false;

	FInventorySlot* slot = GetSlot(InSlot);
	if (!slot) return OutResult.bSuccess = false;

	if (!GetWorld()) return OutResult.bSuccess = false;
	
	UPickupFactorySubsystem* FactorySubSystem = GetWorld()->GetSubsystem<UPickupFactorySubsystem>();
	if (!FactorySubSystem) return OutResult.bSuccess = false;

	if (slot->GetCnt() >= 1)
	{
		for (int i=0; i< slot->GetCnt(); i++)
			FactorySubSystem->SpawnPickupAsync(slot->ItemData, FTransform(FRotator::ZeroRotator, InPos + FVector(FMath::RandPointInCircle(100.0f),0.0f)));

		ClearSlot(InSlot);
		OutResult.bSuccess = true;

	}
	else
	{
		OutResult.bSuccess = false;
	}


	return OutResult.bSuccess;
}

bool UInventoryComponent::HandleUseCommand(const int32 InSlot, FCommandResult& OutResult)
{
	if (!IsValidIndex(InSlot)) return  OutResult.bSuccess = false;
	FInventorySlot* slot = GetSlot(InSlot);
	if (!slot) return OutResult.bSuccess = false;

	UE_LOG(LogTemp, Log, TEXT("%s를 사용했습니다!"), *slot->ItemData->DisplayName.ToString());

	return OutResult.bSuccess = true;
}

bool UInventoryComponent::HandleMoneyCommand(const int32 InDelta, FCommandResult& OutResult)
{
	AddMoney(InDelta);
	return OutResult.bSuccess = true;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}