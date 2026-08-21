


#include "Data/Item/Action/ItemAction_Money.h"
#include "Interface/InventoryUserInterface.h"
#include "Component/InventoryComponent.h"

void UItemAction_Money::ExecuteAction_Implementation(AActor* InInstigator, AActor* InTarget)
{
	if (IInventoryUserInterface* InvenUser = Cast<IInventoryUserInterface>(InTarget))
	{
		FCommandResult res;
		InvenUser->ExecuteInventoryCommand(FInventoryCommand::MakeMoney(Money), res);
	}
	UE_LOG(LogTemp, Display, TEXT("돈 %d 추가하기"), Money);
}
