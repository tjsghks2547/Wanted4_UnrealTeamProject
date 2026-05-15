// Fill out your copyright notice in the Description page of Project Settings.


#include "KZMonster/AI/BTTaskNode_Idle.h"
#include "BTTaskNode_Idle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "KZMonster/KZMonsterCharacter.h"

UBTTaskNode_Idle::UBTTaskNode_Idle()
{
	NodeName = TEXT("Find Random Pos");
	//bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const AIC = OwnerComp.GetAIOwner();
	auto const Pawn = AIC->GetPawn();
	auto const BB = OwnerComp.GetBlackboardComponent();
	
	// 1. 스폰 위치(HomePos) 가져오기
	FVector Origin = BB->GetValueAsVector(FName("HomePos"));

	// 2. 주변 랜덤 좌표 찾기
	UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation NextLocation;

	// HomePos 기준 500 반경 내에서 이동 가능한 좌표 탐색
	if (NavSys->GetRandomPointInNavigableRadius(Origin, 500.0f, NextLocation))
	{
		// 3. 찾은 좌표를 블랙보드에 저장
		BB->SetValueAsVector(FName("TargetLocation"), NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;

	//AAIController* AIController = OwnerComp.GetAIOwner();
	//AKZMonsterCharacter* Monster = Cast<AKZMonsterCharacter>(AIController->GetPawn());

}

//void UBTTaskNode_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//}
