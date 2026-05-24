// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LongRangeAttack.h"
#include "../KZBossCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "KZMonster/KZMonsterCharacter.h"
#include "BehaviorTree/BTTaskNode.h"

UBTTask_LongRangeAttack::UBTTask_LongRangeAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_LongRangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AKZMonsterCharacter* Monster = Cast<AKZMonsterCharacter>(AIController->GetPawn().Get());

	if (Monster)
	{
		AIController->StopMovement();

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("PlayerPos")));
		if (TargetActor)
		{
			AIController->SetFocus(TargetActor);
		}

		// 공격 중임을 블랙보드에 먼저 저장 (대소문자 일치)
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsAttacking"), true);

		// 몽타주 실행
		Monster->PlayLongRangeAttackMontage();

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_LongRangeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}

void UBTTask_LongRangeAttack::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}
