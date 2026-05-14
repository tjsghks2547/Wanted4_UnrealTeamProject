// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../KZCharacter/KZCharacterBase.h"
#include "AnimNotifyState_WeaponCollision.generated.h"

/**
 * 
 */
UCLASS()
class KHAZAN_API UAnimNotifyState_WeaponCollision : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;


	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
