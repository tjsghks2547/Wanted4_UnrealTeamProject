// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_LaunchCharacter.h"
#include "../Interface/KZAnimationAttackInterface.h"
#include "../Interface/KZDamageInterface.h"

void UAnimNotify_LaunchCharacter::Notify(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IKZAnimationAttackInterface* AttackPawn = Cast<IKZAnimationAttackInterface>(MeshComp->GetOwner());
		IKZDamageInterface* DamageInterface = Cast<IKZDamageInterface>(MeshComp->GetOwner());

		if (AttackPawn)
		{
			float FinalLaunchForce = LaunchForce;

			if (DamageInterface && DamageInterface->IsLastDamageDot())
			{
				FinalLaunchForce = 10.0f;
			}
			else
			{
				FinalLaunchForce = 500.0f;
			}

			AttackPawn->LaunchCharacterNotify(FinalLaunchForce);
		}
	}
}
