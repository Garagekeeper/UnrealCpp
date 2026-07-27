// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AttackState.h"


void UAnimNotifyState_AttackState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	OwnerCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->UpdateAttackState(true);
	}
}



void UAnimNotifyState_AttackState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->UpdateAttackState(false);
		OwnerCharacter = nullptr;
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}


