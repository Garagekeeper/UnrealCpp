

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"


class UInventoryComponent;
class UTextBlock;
class UImage;
class UHorizontalBox;
struct FInventorySlot;

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitSlot(UInventoryComponent* InInven, int32 InIndex);
	void RefreshSlot() const;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UHorizontalBox> CountBox;

	// 현재 stack 수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UTextBlock> CountText;

	// 최대 stack 수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UTextBlock> MaxStackText;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowprivateAccess = "true"))
	TWeakObjectPtr<UInventoryComponent> TargetInventory = nullptr;

	static constexpr int32 InvalidIndex = -1;
	int32 Index = InvalidIndex;

	// 이slot widget과 대응되는 인벤토리의 주소
	const FInventorySlot* Slot = nullptr;
};
