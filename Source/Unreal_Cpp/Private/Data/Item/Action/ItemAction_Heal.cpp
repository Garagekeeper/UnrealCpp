


#include "Data/Item/Action/ItemAction_Heal.h"
#include "Component/StatComponent.h"
#include "Interface/HealthInterface.h"
#include "Interface/StatHolder.h"

void UItemAction_Heal::ExecuteAction_Implementation(AActor* InInstigator, AActor* InTarget)
{
	UE_LOG(LogTemp, Display, TEXT("체력 %d 추가하기"), Heal);


	if (IStatHolder* Stat = Cast<IStatHolder>(InTarget))
	{
		if (UStatComponent* Comp = Stat->GetStatComponent())
		{
			if (Heal > 0)
				IHealthInterface::Execute_ApplyHeal(Comp, Heal);
			else 
				IHealthInterface::Execute_ApplyDamage(Comp, -Heal);
		}
	}
}
