

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Item/ItemDataAsset.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
	TObjectPtr<UItemDataAsset> ItemData;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
	int32 StackCnt = 0;

public:
	inline bool			IsEmpty() const { return ItemData == nullptr; }
	inline bool			IsFull() const{ return ItemData && StackCnt >= ItemData->MaxStackCnt; }
	inline void			Clear() { ItemData = nullptr; StackCnt = 0; }
		
	inline int32		GetRemainCnt() const { return ItemData ? ItemData->MaxStackCnt - StackCnt : 0; }
	inline int32		GetCnt() const { return StackCnt; }
	inline void			SetCnt(int32 InCount) 
	{
		if (ItemData && InCount > 0)
		{
			StackCnt = FMath::Clamp(InCount, 0, ItemData->MaxStackCnt);
		}
		else
		{
			Clear();
		}
	}

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_CPP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
						UInventoryComponent();	


	void				AddMoney(int32 InIncome);
	void				AddItem(UItemDataAsset* InItemData, int32 InCount);
	void				UseItem(int32 InIndex);

	/*-----------------------------
	* Getter
	-----------------------------*/
	inline int32		GetMoney() const { return Money; }
	FInventorySlot*		GetSlot(int InSlotIndex);
	FInventorySlot*		GetTempSlot(int InSlotIndex);


protected:

	void				UpdateSlotCount(int32 InSlotIndex, int32 InDeltaCount);
	void				SetItemSlot(int32 InSlotIndex, UItemDataAsset* InItemData, int32 InCount);
	void				ClearSlot(int32 InSlotIndex);
	inline bool			IsValidIndex(int32 InSlotIndex) const
	{
		return InSlotIndex < InventorySize 
			&& InSlotIndex > 0;
	}

	// Called when the game starts
	virtual void		BeginPlay() override;
	// Called every frame
	virtual void		TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	int32				FindSlotWitdhItem(const UItemDataAsset* InItemData, int32 InStartIndex = 0);
	int32				FindEmptySlot();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|Money")
	int32 Money = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|Slot")
	TArray<FInventorySlot> Slots;

private:
	static constexpr int32 InventorySize = 10;
	// 슬롯 검색등에서 실패했을 때 반환할 정수
	static constexpr int32 InValidSlot = INT32_MIN;
	
};
