


#include "Widget/InventorySlotWidget.h"
#include "Component/InventoryComponent.h"
#include "Component/InventoryCommandTypes.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Widget/InventoryDragDropOperation.h"
#include "Widget/TemporaryWidget.h"
#include "Player/ActionCharacter.h"

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
	//UE_LOG(LogTemp, Log, TEXT("OnMouseEnter : %d 슬롯"), Index);
	OnSlotEnter.Broadcast(Index);
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	//UE_LOG(LogTemp, Log, TEXT("OnMouseLeave : %d 슬롯"), Index);
	OnSlotLeave.Broadcast();
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Log, TEXT("드래그 감지"));

	//UInventoryDragDropOperation* DragOp = NewObject<UInventoryDragDropOperation>();
	////DragOp->ItemData
	//
	//UTemporaryWidget* DragTemp = CreateWidget<UTemporaryWidget>(
	//	this,
	//	TargetInventory->GetTempSlotWidgetClass()
	//);
	//// 드래그중에 보일 위젯
	//DragOp->DefaultDragVisual = DragTemp;
	//DragOp->Index = Index;
	//OutOperation = DragOp;
}


bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Log, TEXT("드롭 감지"));
	/*if (UInventoryDragDropOperation* DragOp = Cast<UInventoryDragDropOperation>(InOperation))
	{
		
		if (DragOp->Index == Index) return false;

		if (AActionCharacter* Player = Cast<AActionCharacter>(GetOwningPlayerPawn()))
		{
			FCommandResult Res;
			Player->GetInventoryComponent()->ExecuteCommand(FInventoryCommand::MakeMove(DragOp->Index, Index), Res);
		}
	}*/
	return true;
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

	UE_LOG(LogTemp, Log, TEXT("드래그 실패"));
	return Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (FInventorySlot* InvenSlot = TargetInventory->GetSlot(Index))
		{
			if (InvenSlot->IsEmpty()) 
				return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

			return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
