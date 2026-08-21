


#include "Data/Item/Action/ItemAction_Heal.h"

void UItemAction_Heal::ExecuteAction_Implementation(AActor* InInstigator, AActor* InTarget)
{
	UE_LOG(LogTemp, Display, TEXT("체력 %d 추가하기"), Heal);
}
