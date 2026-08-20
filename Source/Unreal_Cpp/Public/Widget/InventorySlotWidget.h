

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"


class UInventoryComponent;
class UTextBlock;
class UImage;
class UHorizontalBox;
struct FInventorySlot;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnSlotEnter, int32);
DECLARE_MULTICAST_DELEGATE(FOnSlotLeave);
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
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
public:
	FOnSlotEnter OnSlotEnter;
	FOnSlotLeave OnSlotLeave;

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
};
