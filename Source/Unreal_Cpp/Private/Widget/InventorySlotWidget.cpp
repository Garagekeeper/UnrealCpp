


#include "Widget/InventorySlotWidget.h"
#include "Component/InventoryComponent.h"
#include "Component/InventoryCommandTypes.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Widget/InventoryDragDropOperation.h"
#include "Widget/TemporaryWidget.h"
#include "Player/ActionCharacter.h"
#include "Blueprint/SlateBlueprintLibrary.h"

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
		Icon->SetBrushFromTexture(nullptr);
		Icon->SetBrushTintColor(FLinearColor::Transparent);
		CountBox->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Icon->SetBrushFromSoftTexture(TargetSlot->ItemData->ItemIcon.Get());
		Icon->SetBrushTintColor(FLinearColor(1, 1, 1, 1));
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

	FInventorySlot* InvenSlot = TargetInventory->GetSlot(Index);
	if (!InvenSlot || !InvenSlot->ItemData) return;

	UInventoryDragDropOperation* DragOp = NewObject<UInventoryDragDropOperation>();
	DragOp->ItemData = InvenSlot->ItemData;
	DragOp->CurrentCnt = InvenSlot->GetCnt();;

	UTemporaryWidget* DragTemp = CreateWidget<UTemporaryWidget>(
		this,
		TargetInventory->GetTempSlotWidgetClass()
	);
	// 드래그중에 보일 위젯
	DragOp->DefaultDragVisual = DragTemp;
	DragOp->Index = Index;
	DragTemp->SetVisual(InvenSlot->ItemData->ItemIcon.Get(), InvenSlot->GetCnt());
	OutOperation = DragOp;

	FCommandResult Res;
	TargetInventory->ExecuteCommand(FInventoryCommand::MakeMove(Index, TargetInventory->GetSize()), Res);
}


bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UInventoryDragDropOperation* DragOp = Cast<UInventoryDragDropOperation>(InOperation))
	{

		if (DragOp->Index == Index) return false;


		FCommandResult Res;
		TargetInventory->ExecuteCommand(FInventoryCommand::MakeMove(Index, DragOp->Index), Res);
		TargetInventory->ExecuteCommand(FInventoryCommand::MakeMove(TargetInventory->GetSize(), Index), Res);
	}

	return true;
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

	if (UInventoryDragDropOperation* DragOp = Cast<UInventoryDragDropOperation>(InOperation))
	{
		if (APlayerController* PC = GetOwningPlayer())
		{
			//FHitResult HitRes;
			//if (PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitRes))
			//{
			//	//FVector Loc = GetOwningPlayerPawn()->GetActorLocation();
			//	FCommandResult Res;
			//	TargetInventory->ExecuteCommand(FInventoryCommand::MakeDrop(TargetInventory->GetSize(), HitRes.Location), Res);
			//}
			
			FVector2D AbsPos = InDragDropEvent.GetScreenSpacePosition();
			FVector2D PixelPos;
			FVector2D ViewPortPos;
			USlateBlueprintLibrary::AbsoluteToViewport(this, AbsPos, PixelPos, ViewPortPos);

			FVector WorldPos;
			FVector WorldDir;
			if (PC->DeprojectScreenPositionToWorld(PixelPos.X, PixelPos.Y, WorldPos, WorldDir))
			{
				FVector Start = WorldPos;
				FVector End = Start + WorldDir * 10000.0f;

				FVector SpawnLocation;
				FHitResult HitRes;
				if (GetWorld()->LineTraceSingleByChannel(HitRes, Start, End, ECollisionChannel::ECC_Visibility))
				{
					SpawnLocation = HitRes.Location;
				}
				else
				{
					SpawnLocation = End;
				}

				FCommandResult Res;
				TargetInventory->ExecuteCommand(FInventoryCommand::MakeDrop(TargetInventory->GetSize(), SpawnLocation), Res);
			}
		}

		
	}
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
