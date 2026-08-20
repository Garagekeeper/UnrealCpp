


#include "Widget/DetailInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "Data/Item/ItemDataAsset.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UDetailInfoWidget::DetailOpen(const UItemDataAsset* InItemData)
{
	if (!InItemData) return;

	ItemIcon->SetBrushFromTexture(InItemData->ItemIcon.Get());
	ItemNameText->SetText(InItemData->DisplayName);
	ItemPriceText->SetText(FText::AsNumber(InItemData->Price));
	ItemDescriptionText->SetText(InItemData->Description);
	SetVisibility(ESlateVisibility::HitTestInvisible);

	UpdateLocation();
}

void UDetailInfoWidget::DetailClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UDetailInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	DetailClose();
}

void UDetailInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateLocation();
}

void UDetailInfoWidget::UpdateLocation()
{
	if (!CanvasSlot.IsValid())
	{
		CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	}

	if (CanvasSlot.IsValid())
	{
		const FVector2D MouseAbsolute = UWidgetLayoutLibrary::GetMousePositionOnPlatform();
		if (UPanelWidget* ParentPanel = GetParent())
		{
			const FGeometry& ParentGeometry = ParentPanel->GetTickSpaceGeometry();
			const FVector2D LocalPos = ParentGeometry.AbsoluteToLocal(MouseAbsolute);
			CanvasSlot->SetPosition(FVector2D(LocalPos));
		}
	}
}
