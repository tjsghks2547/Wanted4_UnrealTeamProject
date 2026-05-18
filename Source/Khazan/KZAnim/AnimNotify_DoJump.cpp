// Fill out your copyright notice in the Description page of Project Settings.


#include "KZAnim/AnimNotify_DoJump.h"
#include "../KZCharacter/KZCharacterPlayer.h"

void UAnimNotify_DoJump::Notify(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		AKZCharacterPlayer* Player = Cast<AKZCharacterPlayer>(MeshComp->GetOwner());
		if (Player)
		{
			Player->ExcutePhysicsJump();
		}
	}
}
