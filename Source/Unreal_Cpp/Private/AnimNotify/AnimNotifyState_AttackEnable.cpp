


#include "AnimNotify/AnimNotifyState_AttackEnable.h"
#include "Interface/WeaponUserInterface.h"

void UAnimNotifyState_AttackEnable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	WeaponOwner = Cast<IWeaponUserInterface>(MeshComp->GetOwner());
	if (WeaponOwner)
	{
		WeaponOwner->OnWeaponAttackState(true);
	}
}

void UAnimNotifyState_AttackEnable::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference & EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (WeaponOwner)
	{
		WeaponOwner->OnWeaponAttackState(false);
		WeaponOwner = nullptr;
	}
}
