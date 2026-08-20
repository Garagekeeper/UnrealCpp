


#include "Widget/InventoryWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Widget/MoneyPanelWidget.h"
#include "Widget/InventorySlotWidget.h"
#include "Component/InventoryComponent.h"
#include "Interface/InventoryUserInterface.h"
#include "Player/ActionPlayerController.h"
#include "Widget/InventorySlotWidget.h"
#include "Widget/DetailInfoWidget.h"
#include "Components/CanvasPanelSlot.h"


void UInventoryWidget::InitInventoryWidget(UInventoryComponent* InventoryComponent)
{
	ClearInventoryWidget();
	TargetInventory = InventoryComponent;

	if (TargetInventory.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("TargetInventory was InValid Pointer"));
	}

	if (SlotGridPannel)
	{
		int32 ChildCount = SlotGridPannel->GetChildrenCount();
		int32 InvenSize = TargetInventory.Get()->GetSize();
		SlotSize = FMath::Min(ChildCount, InvenSize);

		SlotWidgets.Empty(SlotSize);
		for (int i = 0; i < SlotSize; ++i)
		{
			if (UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(SlotGridPannel->GetChildAt(i)))
			{
				SlotWidget->InitSlot(TargetInventory.Get(), i);
				SlotWidget->OnSlotEnter.AddWeakLambda(
					this,
					[this](int32 InIndex)
					{
						DetailInfo->DetailOpen(TargetInventory.Get()->GetSlot(InIndex)->ItemData);
					});
				SlotWidget->OnSlotLeave.AddWeakLambda(
					this,
					[this]()
					{
						DetailInfo->DetailClose();
					});
				SlotWidgets.Add(SlotWidget);
			}
		}
	}

	TargetInventory->OnMoneyChaneged.AddUObject(this, &UInventoryWidget::RefreshMoneyPannel);
	TargetInventory->OnSlotChanged.BindUObject(this, &UInventoryWidget::RefreshSlotWidget);

	RefreshInventoryWidget();
}

void UInventoryWidget::ClearInventoryWidget()
{

	if (TargetInventory.IsValid())
	{
		for (UInventorySlotWidget* SlotWidget : SlotWidgets)
		{
			SlotWidget->OnSlotEnter.RemoveAll(this);
			SlotWidget->OnSlotLeave.RemoveAll(this);
		}

		TargetInventory->OnSlotChanged.Unbind();
		TargetInventory->OnMoneyChaneged.RemoveAll(this);
		TargetInventory = nullptr;
	}

	SlotWidgets.Empty();
	SlotSize = 0;
}

void UInventoryWidget::OpenInventoryWidget()
{
	SetVisibility(ESlateVisibility::Visible);

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	DetailInfo->SetParentPosition(CanvasSlot->GetPosition());

	//사실 여기다 델리게이트를 넣는게 맞음
	if (AActionPlayerController* PC = Cast<AActionPlayerController>(GetOwningPlayer()))
	{
		PC->OnInventoryOpenClose(true, this);
	}
}

void UInventoryWidget::CloseInventoryWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
	if (AActionPlayerController* PC = Cast<AActionPlayerController>(GetOwningPlayer()))
	{
		PC->OnInventoryOpenClose(false, this);
	}
}

void UInventoryWidget::ToggleInventoryWidget()
{
	IsInventoryOpen() ? CloseInventoryWidget() : OpenInventoryWidget();
}

void UInventoryWidget::TestRefresh()
{
	RefreshInventoryWidget();
}

void UInventoryWidget::RefreshInventoryWidget() const
{
	if (!TargetInventory.IsValid()) return;

	RefreshMoneyPannel(TargetInventory.Get()->GetMoney());

	for (const UInventorySlotWidget* SlotWidget : SlotWidgets)
	{
		if (SlotWidget)
		{
			SlotWidget->RefreshSlot();
		}
	}
}

void UInventoryWidget::RefreshSlotWidget(int32 InSlotIndex) const
{
	if (IsValidIndex(InSlotIndex) && SlotWidgets[InSlotIndex])
	{
		SlotWidgets[InSlotIndex]->RefreshSlot();
	}
}

void UInventoryWidget::RefreshMoneyPannel(int32 InCurrentMoney) const
{
	if (MoneyPanel)
	{
		MoneyPanel->SetMoney(InCurrentMoney);
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);

	if (CloseBtn)
	{
		CloseBtn->OnClicked.AddDynamic(this, &UInventoryWidget::OnCloseBtnClicked);
	}

	//InitInventoryWidget()
	if (IInventoryUserInterface* InvenUser = Cast<IInventoryUserInterface>(GetOwningPlayerPawn()))
	{
		if (UInventoryComponent* InvenComp = InvenUser->GetInventoryComponent())
		{
			InitInventoryWidget(InvenComp);
		}
	}

	CloseInventoryWidget();
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// 일단 어떤키든 눌러지면 실행

	// 눌려진 키가 I인 경우 처리
	if (InKeyEvent.GetKey() == EKeys::I)
	{
		CloseInventoryWidget();

		// 이 입력이 여기서 끝났다고 처리 (Consume)
		//FReply::UnHandled()의 경우 처리는 했지만 하위 위젯에게 입력을 전달 할 수 있음
		return FReply::Handled();
	}

	// 이 위젯이 처리하지 않은 입력은 부모에서 처리
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UInventoryWidget::OnCloseBtnClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Close Btn Clicked"));
	CloseInventoryWidget();
}
