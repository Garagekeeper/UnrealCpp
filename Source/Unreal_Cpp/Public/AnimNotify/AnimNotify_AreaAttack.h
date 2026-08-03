

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AreaAttack.generated.h"


class APlayerCameraManager;
//class UCameraShakeBase;
/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UAnimNotify_AreaAttack : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCameraShakeBase> ShakeEffect = nullptr;

private:
	TWeakObjectPtr<APlayerCameraManager> CameraManager = nullptr;
	
};
