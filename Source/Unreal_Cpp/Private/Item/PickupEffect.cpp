


#include "Item/PickupEffect.h"
#include "Interface/StatHolder.h"
#include "Component/StatComponent.h"


void APickupEffect::OnPickUp(AActor* OtherActor)
{
	//1. 인터페이스로 캐스트
	//IStaminaInterface* Target =  Cast<IStaminaInterface>(OtherActor);

	//2. 권장하는 방법(Bp에서 상속 받은 것도 체크 가능)
	//bool bImplements = OtherActor->Implements<UStaminaInterface>();

	//if (OtherActor && OtherActor->Implements<UStatHolder>())
	IStatHolder* Stat = Cast<IStatHolder>(OtherActor);
	if (OtherActor && Stat)
	{
		if (UStatComponent* Component = Stat->GetStatComponent())
		{
			//UStatComponent* Component = IStatHolder::Execute_GetStatComponent(OtherActor);
			if (Stamina > 0)
			{
				IStaminaInterface::Execute_RecoveryStamina(Component, Stamina);
			}
			else if (Stamina < 0)
			{
				IStaminaInterface::Execute_ConsumeStamina(Component, -Stamina);
			}

			if (Health > 0)
			{
				IHealthInterface::Execute_ApplyHeal(Component, Health);
			}
			else if (Health < 0)
			{
				IHealthInterface::Execute_ApplyDamage(Component, -Health);
			}
		}

	}
}

UMeshComponent* APickupEffect::GetMesh() const
{
	return nullptr;
}
