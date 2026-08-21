


#include "Widget/TemporaryWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTemporaryWidget::SetVisual(UTexture2D* InIcon, int32 InCount)
{
	Icon->SetBrushFromTexture(InIcon);
	Icon->SetBrushTintColor(FLinearColor(1, 1, 1, 1));
	CountText->SetText(FText::AsNumber(InCount));
}
