


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
		FCommandResult Res;
		TargetInventory->ExecuteCommand(FInventoryCommand::MakeMove(Index, DragOp->Index), Res);
		TargetInventory->ExecuteCommand(FInventoryCommand::MakeMove(TargetInventory->GetSize(), Index), Res);
	}

	return true;
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Log, TEXT("바닥에서 드래그 종료"));

	if (APlayerController* PC = GetOwningPlayer())
	{
		UE_LOG(LogTemp, Log, TEXT("플레이어 컨트롤러 확인"));
		FHitResult HitResult;
		//if (PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))	// UI에서 관리하는 마우스 좌표와 PC가 관리하는 마우스 좌표가 다름
		//{
		//	UE_LOG(LogTemp, Log, TEXT("바닥 히트 성공"));
		//	FCommandResult Result;
		//	TargetInventory->ExecuteCommand(
		//		FInventoryCommand::MakeDrop(TargetInventory->GetSize(), HitResult.Location),
		//		Result);
		//}

		FVector2D AbsolutePosition = InDragDropEvent.GetScreenSpacePosition();
		FVector2D PixelPosion;
		FVector2D ViewportPosition;
		USlateBlueprintLibrary::AbsoluteToViewport(this, AbsolutePosition, PixelPosion, ViewportPosition);

		FVector WorldLocation;
		FVector WorldDirection;
		if (PC->DeprojectScreenPositionToWorld(
			PixelPosion.X, PixelPosion.Y,
			WorldLocation, WorldDirection))
		{
			FVector Start = WorldLocation;
			FVector End = Start + WorldDirection * 10000.0f;

			FVector SpawnLocation;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
			{
				SpawnLocation = HitResult.Location;
			}
			else
			{
				SpawnLocation = End;
			}

			float MaxBound = 500.0f;
			FVector PlayerLoc = GetOwningPlayerPawn()->GetActorLocation();
			FVector Dist = SpawnLocation - PlayerLoc;
			if ((Dist.SquaredLength()) >= (MaxBound * MaxBound))
			{
				SpawnLocation = PlayerLoc + Dist.GetSafeNormal() * MaxBound;
				FVector DownStart = SpawnLocation + FVector::UpVector * 10000.0f;
				FVector EndStart = SpawnLocation + FVector::DownVector * 10000.0f;
				FHitResult GroundHit;
				if (GetWorld()->LineTraceSingleByChannel(GroundHit, DownStart, EndStart, ECollisionChannel::ECC_Visibility))
				{
					SpawnLocation = GroundHit.Location;
				}
			}

			FCommandResult Result;
			TargetInventory->ExecuteCommand(
				FInventoryCommand::MakeDrop(TargetInventory->GetSize(), SpawnLocation),
				Result);
		}
	}

	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
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
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		if (FInventorySlot* InvenSlot = TargetInventory->GetSlot(Index))
		{
			if (!InvenSlot->IsEmpty())
			{
				FCommandResult Res;
				TargetInventory->ExecuteCommand(FInventoryCommand::MakeUse(Index), Res);
				TargetInventory->ExecuteCommand(FInventoryCommand::MakeEquip(Index), Res);
			}

		}
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);


}
