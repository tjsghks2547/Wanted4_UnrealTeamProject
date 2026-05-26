// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BasicBossAttack.h"
#include "../KZBossCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "KZMonster/KZMonsterCharacter.h"
#include "BehaviorTree/BTTaskNode.h"

UBTTask_BasicBossAttack::UBTTask_BasicBossAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BasicBossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AKZMonsterCharacter* Monster = Cast<AKZMonsterCharacter>(AIController->GetPawn().Get());

	if (Monster)
	{
		//AIController->StopMovement();

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("PlayerPos")));
		if (TargetActor)
		{
			AIController->SetFocus(TargetActor);
		}

		// 공격 중임을 블랙보드에 먼저 저장 (대소문자 일치)
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsAttacking"), true);

		// 몽타주 실행
		Monster->PlayAttackMontage();

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}


void UBTTask_BasicBossAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
