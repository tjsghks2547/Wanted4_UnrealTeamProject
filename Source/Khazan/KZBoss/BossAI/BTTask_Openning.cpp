// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Openning.h"
#include "../KZBossCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_Openning::UBTTask_Openning()
{
}

EBTNodeResult::Type UBTTask_Openning::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AKZBossCharacter* Boss = Cast<AKZBossCharacter>(AIController->GetPawn().Get());

	if (Boss)
	{
		// 시작되지 않은 경우
		if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("IsOpenning")))
		{

			// 풍차돌리기 공격 몽타주 실행
			Boss->PlayOpenningMontage();

		}

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_Openning::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
