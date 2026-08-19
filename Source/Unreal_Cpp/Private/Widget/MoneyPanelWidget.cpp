


#include "Widget/MoneyPanelWidget.h"
#include "Components/TextBlock.h"

void UMoneyPanelWidget::SetMoney(int32 InMoney)
{
	MoneyText->SetText(FText::AsNumber(InMoney));
}
