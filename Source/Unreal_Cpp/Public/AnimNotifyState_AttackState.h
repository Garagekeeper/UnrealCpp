// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AttackState.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UAnimNotifyState_AttackState : public UAnimNotifyState
{
	GENERATED_BODY()

public:

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
protected:


private:
	TWeakObjectPtr<AActionCharacter> OwnerCharacter = nullptr;

	
	
	
};
