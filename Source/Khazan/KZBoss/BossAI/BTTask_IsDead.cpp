// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IsDead.h"
#include "AIController.h"
#include "KZMonster/KZMonsterCharacter.h"

UBTTask_IsDead::UBTTask_IsDead()
{
	NodeName = TEXT("Death");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_IsDead::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	AKZMonsterCharacter* Monster = Cast<AKZMonsterCharacter>(AIC->GetPawn());

	if (Monster)
	{
		Monster->PlayDeathMontage();
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_IsDead::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	AKZMonsterCharacter* Monster = Cast<AKZMonsterCharacter>(AIC->GetPawn().Get());

	// 캐릭터에서 애니메이션이 끝났다고 신호를 주면
	if (Monster && Monster->IsDeathAnimationDone())
	{
		// 비헤이비어 트리 테스크 완료 보고
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		// 액터 파괴
		Monster->Destroy();
	}

}

void UBTTask_IsDead::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}
