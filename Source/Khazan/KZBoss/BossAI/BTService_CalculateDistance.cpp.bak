// Fill out your copyright notice in the Description page of Project Settings.


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

	// 1. AI 컨트롤러 및 폰 가져오기
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* ControllingPawn = AIController->GetPawn();
	if (!ControllingPawn) return;

	// 2. 블랙보드에서 타겟(PlayerPos) 가져오기
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("PlayerPos")));
	
	// 3. 거리 계산 (플레이어와의 거리)
	float Distance = 99999.0f;
	if (TargetActor)
	{
		Distance = FVector::Dist(ControllingPawn->GetActorLocation(), TargetActor->GetActorLocation());
		BlackboardComp->SetValueAsFloat(FName("Distance"), Distance);
	}

	// 4. HomePos로부터의 거리 계산
	FVector HomePos = BlackboardComp->GetValueAsVector(FName("HomePos"));
	DistFromHome = FVector::Dist(ControllingPawn->GetActorLocation(), HomePos);

	// 5. 활동 범위(PatrolRadius) 가져오기
	Radius = BlackboardComp->GetValueAsFloat(FName("PatrolRadius"));

	// 6. 범위를 벗어났는지 판단
	bool bIsFarFromHome = DistFromHome > Radius;
	BlackboardComp->SetValueAsBool(FName("IsFarFromHome"), bIsFarFromHome);

	float LeachThreshold = 500.0f; // 추격 유지 임계값

	if (bIsFarFromHome)
	{
		// 범위를 벗어났을 때
		if (TargetActor && Distance > LeachThreshold)
		{
			// 플레이어와 거리가 멀어지면 복귀 상태로 전환
			BlackboardComp->SetValueAsBool(FName("IsReturning"), true);
			BlackboardComp->ClearValue(FName("PlayerPos"));
		}
		else if (!TargetActor)
		{
			// 타겟이 없는데 범위를 벗어나 있다면 복귀
			BlackboardComp->SetValueAsBool(FName("IsReturning"), true);
		}
	}
	else
	{
		// 범위 안으로 들어오면 복귀 상태 해제
		BlackboardComp->SetValueAsBool(FName("IsReturning"), false);
	}

	// [복귀 완료 판단] 복귀 중이고 집에 충분히 가까워졌다면 상태 해제
	if (BlackboardComp->GetValueAsBool(FName("IsReturning")) && DistFromHome <= 150.0f)
	{
		BlackboardComp->SetValueAsBool(FName("IsReturning"), false);
	}
}
