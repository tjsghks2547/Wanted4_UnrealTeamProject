// Fill out your copyright notice in the Description page of Project Settings.


#include "KZAnim/AnimNotifyState_Invincibility.h"
#include "../KZCharacter/KZCharacterPlayer.h"


void UAnimNotifyState_Invincibility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (AKZCharacterPlayer* Character = Cast<AKZCharacterPlayer>(MeshComp->GetOwner()))
	{
		Character->SetInvincible(true);

	}
}

void UAnimNotifyState_Invincibility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AKZCharacterPlayer* Character = Cast<AKZCharacterPlayer>(MeshComp->GetOwner()))
	{
		Character->SetInvincible(false);

	}
}
