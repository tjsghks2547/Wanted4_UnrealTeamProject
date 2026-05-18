// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../KZCharacter/KZCharacterPlayer.h"
#include "AnimNotifyState_Invincibility.generated.h"

/**
 * 
 */
UCLASS()
class KHAZAN_API UAnimNotifyState_Invincibility : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;


	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
