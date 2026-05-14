// Fill out your copyright notice in the Description page of Project Settings.


#include "KZAnim/AnimNotifyState_WeaponCollision.h"
#include "AnimNotifyState_WeaponCollision.h"


void UAnimNotifyState_WeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (AKZCharacterBase* Character = Cast<AKZCharacterBase>(MeshComp->GetOwner()))
	{
		Character->AlreadyHitActor.Empty();
		Character->EnableWeaponCollision();
	
	}
}

void UAnimNotifyState_WeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AKZCharacterBase* Character = Cast<AKZCharacterBase>(MeshComp->GetOwner()))
	{
		Character->DisableWeaponCollision();
		
	}
}
