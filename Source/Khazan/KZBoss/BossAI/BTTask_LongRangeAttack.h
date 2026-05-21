// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LongRangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class KHAZAN_API UBTTask_LongRangeAttack : public UBTTaskNode
{
	GENERATED_BODY()


	UBTTask_LongRangeAttack();

	// 태스크가 실행될 때 호출되는 함수
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:

	// 몽타주가 끝났을 때 호출되는 함수 (델리게이트로 연동)
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
};
