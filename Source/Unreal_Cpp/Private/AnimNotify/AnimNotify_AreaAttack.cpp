


#include "AnimNotify/AnimNotify_AreaAttack.h"
#include "Interface/WeaponUserInterface.h"
#include "Weapon/WeaponActor.h"
#include "Player/ActionCharacter.h"

void UAnimNotify_AreaAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!CameraManager.IsValid())
	{
		if (UWorld* World = MeshComp->GetWorld())
		{
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				CameraManager = PC->PlayerCameraManager;
			}
		}
	}

	if (CameraManager.IsValid())
	{
		CameraManager->StartCameraShake(ShakeEffect);
	}

	AActionCharacter* OwnerCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->OnAreaAttack();
	}

	//if (IWeaponUserInterface* WeaponUser = Cast<IWeaponUserInterface>(MeshComp->GetOwner()))
	//{
	//	if (UWeaponComponent* WeaponComp = WeaponUser->GetWeaponComponent())
	//	{
	//		WeaponComp->AreaAttack();
	//	}
	//}
}
