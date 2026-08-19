


#include "Widget/InventoryWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Widget/MoneyPanelWidget.h"
#include "Widget/InventorySlotWidget.h"
#include "Component/InventoryComponent.h"
#include "Interface/InventoryUserInterface.h"

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
	SlotSize = 0;
	TargetInventory = nullptr;

	if (!TargetInventory.IsValid()) return;
	TargetInventory->OnSlotChanged.Unbind();
	TargetInventory->OnMoneyChaneged.Clear();
}

void UInventoryWidget::OpenInventoryWidget()
{
	
}

void UInventoryWidget::CloseInventoryWidget()
{

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
}

void UInventoryWidget::OnCloseBtnClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Close Btn Clicked"));
	CloseInventoryWidget();
}
