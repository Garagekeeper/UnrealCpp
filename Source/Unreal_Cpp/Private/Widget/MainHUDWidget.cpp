


#include "Widget/MainHUDWidget.h"
#include "Widget/InventoryWidget.h"
#include "Widget/StatBarWidget.h"

void UMainHUDWidget::TestInventoryRefresh() const
{
#if WITH_EDITOR
	if(Inventory)
		Inventory->TestRefresh();
#endif
}
