

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AttackEnable.generated.h"


class AActionCharacter;
class IWeaponUserInterface;
/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UAnimNotifyState_AttackEnable : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, 
		float TotalDuration, 
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference) override;
protected:


private:
	//IWeaponUserInterface* WeaponOwner = nullptr;
	//TWeakObjectPtr<IWeaponUserInterface> Owner = nullptr;

	
};
