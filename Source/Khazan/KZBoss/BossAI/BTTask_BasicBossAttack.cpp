// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BasicBossAttack.h"
#include "../KZBossCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_BasicBossAttack::UBTTask_BasicBossAttack()
{
}

EBTNodeResult::Type UBTTask_BasicBossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	AAIController* AIController = OwnerComp.GetAIOwner();
	AKZBossCharacter* BossCharacter = Cast<AKZBossCharacter>(AIController->GetPawn());
	if (BossCharacter)
	{
		// 공격 실행 전에 이동 정지
		AIController->StopMovement();

		// 공격 몽타주 재생
		BossCharacter->PlayAttackMontage();

		// 공격상태가 되었음을 BB에 알림
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("isAttacking"), true);

		// 애니메이션 종료 대기 모드로 진입.
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_BasicBossAttack::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}
