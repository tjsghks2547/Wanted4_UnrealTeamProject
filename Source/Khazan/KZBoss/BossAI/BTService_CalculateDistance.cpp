// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CalculateDistance.h"


#include "BTService_CalculateDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTService_CalculateDistance::UBTService_CalculateDistance()
{
	NodeName = TEXT("Calculate Distance");

	// 서비스가 실행되는 주기 (초)
	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UBTService_CalculateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 1. 제어 중인 폰(보스) 가져오기
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* ControllingPawn = AIController->GetPawn();
	if (!ControllingPawn) return;

	// 2. 블랙보드에서 대상(TargetActor) 가져오기
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("PlayerPos")));
	if (!TargetActor) return;

	// 3. 거리 계산
	float Distance = FVector::Dist(ControllingPawn->GetActorLocation(), TargetActor->GetActorLocation());

	// 4. 블랙보드의 "Distance" 키에 결과 저장
	BlackboardComp->SetValueAsFloat(FName("Distance"), Distance);
}
