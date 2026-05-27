// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PhaseChange.h"
#include "../KZBossCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorTree/BTTaskNode.h"


UBTTask_PhaseChange::UBTTask_PhaseChange()
{
	bNotifyTick = true;
}


EBTNodeResult::Type UBTTask_PhaseChange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AKZBossCharacter* Boss = Cast<AKZBossCharacter>(AIController->GetPawn().Get());
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (Boss)
	{

		//Boss->ChangePhase();

		EBossPhase BossPhase = Boss->CurrentPhase;
		bool bIsChanging = BlackboardComp->GetValueAsBool(FName("IsPhaseChanging"));

		// 페이즈에 맞는 몽타주 실행
		UE_LOG(LogTemp, Error, TEXT("%d"), (int32)Boss->CurrentPhase);
		
		if (BossPhase == EBossPhase::Phase1A && bIsChanging)
		{	
			// 공격 중임을 블랙보드에 먼저 저장 (대소문자 일치)
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsAttacking"), true);

			// 풍차돌리기 공격 몽타주 실행
			Boss->PlayPhaseChangingAttackMontage();

		}
		else if (BossPhase == EBossPhase::Phase1B)
		{
			// Todo: 확장할 시 외형 변경이나 새로운 객체(외형 다른 보스) 생성 관련 로직
		}

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_PhaseChange::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
}

void UBTTask_PhaseChange::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

}