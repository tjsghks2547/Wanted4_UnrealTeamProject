// Fill out your copyright notice in the Description page of Project Settings.


#include "KZAnim/AnimNotify_ChargeCheck.h"
#include "../Interface/KZAnimationAttackInterface.h"

void UAnimNotify_ChargeCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IKZAnimationAttackInterface* AttackPawn = Cast<IKZAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackCheck();
		}
	}
}