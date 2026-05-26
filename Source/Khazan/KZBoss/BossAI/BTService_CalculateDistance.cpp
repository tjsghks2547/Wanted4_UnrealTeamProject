// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CalculateDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "KZBoss/KZBossCharacter.h"
#include "Perception/AIPerceptionComponent.h"

UBTService_CalculateDistance::UBTService_CalculateDistance()
{
	NodeName = TEXT("Calculate Distance");

	// 서비스가 실행되는 주기 (초)
	Interval = 0.5f;
	RandomDeviation = 0.0f;
}

void UBTService_CalculateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 1. AI 컨트롤러 및 폰 가져오기
	AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* ControllingPawn = AIController->GetPawn();
	if (!ControllingPawn) return;

	// 2. 블랙보드에서 타겟(PlayerPos) 가져오기
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("PlayerPos")));
	
	// 3. 거리 계산 (플레이어와의 거리)
	float Distance;
	if (TargetActor)
	{
		Distance = FVector::Dist(ControllingPawn->GetActorLocation(), TargetActor->GetActorLocation());
		BlackboardComp->SetValueAsFloat(FName("Distance"), Distance);
	}

	// 4. HomePos로부터의 거리 계산
	FVector HomePos = BlackboardComp->GetValueAsVector(FName("HomePos"));
	DistFromHome = FVector::Dist(ControllingPawn->GetActorLocation(), HomePos);
	BlackboardComp->SetValueAsFloat(FName("DistFromHome"), DistFromHome);

	// 5. 활동 범위(PatrolRadius) 가져오기
	// 6. 범위를 벗어났는지 판단
	Radius = BlackboardComp->GetValueAsFloat(FName("PatrolRadius"));

	// 활동범위 밖인 경우
	if (DistFromHome > Radius)
	{
		if (Distance >= 500) {
			BlackboardComp->SetValueAsBool(FName("IsReturning"), true);
		}

		if (!ControllingPawn->IsA(AKZBossCharacter::StaticClass())) // 일반 몬스터인 경우
		{
			// 잊어버리기
			AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("PlayerPos")));
			AIController->GetPerceptionComponent()->ForgetActor(Target);
			BlackboardComp->ClearValue(FName("PlayerPos"));
		}
	}


	// [복귀 완료 판단] 복귀 중이고 집에 충분히 가까워졌다면 상태 해제
	if (BlackboardComp->GetValueAsBool(FName("IsReturning")) && DistFromHome <= 150.0f)
	{
		BlackboardComp->SetValueAsBool(FName("IsReturning"), false);
	}
}
