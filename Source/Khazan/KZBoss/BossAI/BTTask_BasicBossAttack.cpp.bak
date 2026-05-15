// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BasicBossAttack.h"
#include "../KZBossCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "KZMonster/KZMonsterCharacter.h"

UBTTask_BasicBossAttack::UBTTask_BasicBossAttack()
{
}

EBTNodeResult::Type UBTTask_BasicBossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	AAIController* AIController = OwnerComp.GetAIOwner();
	// AI가 빙의한 액터를 KZMonsterCharacter로 캐스팅
	AKZMonsterCharacter* Monster = Cast<AKZMonsterCharacter>(AIController->GetPawn());

	//AKZBossCharacter* BossCharacter = Cast<AKZBossCharacter>(AIController->GetPawn());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Monster->GetName());

	if (Monster)
	{
		// 몽타주 실행 전 이동 중지
		AIController->StopMovement();

		// 플레이어 타겟 설정 (StopMovement 후 Focus)
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("PlayerPos")));
		if (TargetActor)
		{
			AIController->SetFocus(TargetActor);
		}

		// 몽타주 실행
		Monster->PlayAttackMontage();

		// 공격 중임을 블랙보드에 저장
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("isAttacking"), true);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_BasicBossAttack::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}
