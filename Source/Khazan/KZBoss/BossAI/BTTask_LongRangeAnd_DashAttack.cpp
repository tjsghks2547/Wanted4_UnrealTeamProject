// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LongRangeAnd_DashAttack.h"
#include "../KZBossCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"

UBTTask_LongRangeAnd_DashAttack::UBTTask_LongRangeAnd_DashAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_LongRangeAnd_DashAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AKZBossCharacter* Boss = Cast<AKZBossCharacter>(AIController->GetPawn().Get());

	if (Boss)
	{
		// 공격 중임을 블랙보드에 먼저 저장 (대소문자 일치)
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsAttacking"), true);

		int32 RandomIdx = 2; //FMath::RandRange(1, 2);

		if (RandomIdx != 1)
		{
			Boss->PlayJumpAttackMontage();

			return EBTNodeResult::Succeeded;
		}

		// 몽타주 실행
		Boss->PlayDashAttackMontage();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_LongRangeAnd_DashAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// 블랙보드의 IsAttacking 키가 false가 되면 태스크 종료 (몽타주 종료 시 캐릭터에서 false로 변경함)
	bool bIsAttacking = OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("IsAttacking"));
	if (!bIsAttacking)
	{
		// 애니메이션이 끝날 때까지 BT 대기
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_LongRangeAnd_DashAttack::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}
