// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BasicBossAttack.generated.h"

UCLASS()
class KHAZAN_API UBTTask_BasicBossAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_BasicBossAttack();

	// 태스크가 실행될 때 호출되는 함수
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	
	// 몽타주가 끝났을 때 호출되는 함수 (델리게이트로 연동)
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 나중에 태스크를 종료하기 위해 저장해두는 컴포넌트
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;
};
