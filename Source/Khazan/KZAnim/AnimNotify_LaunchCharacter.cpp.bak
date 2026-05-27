// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_LaunchCharacter.h"
#include "../Interface/KZAnimationAttackInterface.h"

void UAnimNotify_LaunchCharacter::Notify(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IKZAnimationAttackInterface* AttackPawn = Cast<IKZAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->LaunchCharacterNotify(LaunchForce);
		}
	}
}
