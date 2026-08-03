


#include "AnimNotify/AnimNotify_AreaAttack.h"

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

	MeshComp->GetOwner();;
}
