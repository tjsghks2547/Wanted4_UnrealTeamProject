// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ChargeCheck.generated.h"

/**
 * 
 */
UCLASS()
class KHAZAN_API UAnimNotify_ChargeCheck : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	// 애니메이션 노티파이가 발생할 때 호출되는 함수.
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;


};
