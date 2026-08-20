


#include "Widget/InventorySlotWidget.h"
#include "Component/InventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void UInventorySlotWidget::InitSlot(UInventoryComponent* InInven, int32 InIndex)
{
	if (!InInven) return;

	TargetInventory = InInven;
	Index = InIndex;
	RefreshSlot();
}

void UInventorySlotWidget::RefreshSlot() const
{


	// 이slot widget과 대응되는 인벤토리의 주소
	if (!TargetInventory.IsValid()) return;

	const FInventorySlot* TargetSlot = TargetInventory.Get()->GetSlot(Index);

	if (!TargetSlot)
	{
		UE_LOG(LogTemp, Log, TEXT("[Slot : %d] was Invalid"), Index);
		return;
	}

	if (TargetSlot->IsEmpty())
	{
		Icon->SetBrushFromSoftTexture(nullptr);
		Icon->SetBrushTintColor(FLinearColor::Transparent);
		CountBox->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Icon->SetBrushFromSoftTexture(TargetSlot->ItemData->ItemIcon.Get());
		Icon->SetBrushTintColor(FLinearColor(1,1,1,1));
		CountText->SetText(FText::AsNumber(TargetSlot->GetCnt()));
		MaxStackText->SetText(FText::AsNumber(TargetSlot->ItemData->MaxStackCnt));
		CountBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	UE_LOG(LogTemp, Log, TEXT("OnMouseEnter : %d 슬롯"), Index);
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Log, TEXT("OnMouseLeave : %d 슬롯"), Index);
	Super::NativeOnMouseLeave(InMouseEvent);
}
