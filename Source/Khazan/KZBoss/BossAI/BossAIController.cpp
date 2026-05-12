// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "../KZBossCharacter.h"
#include "Kismet/GameplayStatics.h" // 플레이어 폰을 가져오기 위함

ABossAIController::ABossAIController()
{
	// 블랙보드 컴포넌트 생성
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BTBoss && BTBoss->BlackboardAsset)
	{
		if (UseBlackboard(BTBoss->BlackboardAsset, BlackboardComp))
		{
			// 거리값 초기화 (실행 시 자동으로 0으로 초기화되는 것 방지)
			BlackboardComp->SetValueAsFloat(FName("Distance"), 9999.0f);

			// BT 실행
			RunBehaviorTree(BTBoss);

			AKZBossCharacter* Viper = Cast<AKZBossCharacter>(InPawn);
			if (Viper)
			{
				// 보스의 현재 페이즈를 블랙보드에 저장(갱신)
				BlackboardComp->SetValueAsEnum(FName("CurrentPhase"), (uint8)Viper->CurrentPhase);
			}

			// 플레이어 타겟 설정 재시도 함수
			RetrySetTarget();

		}
	}

}

void ABossAIController::RetrySetTarget()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn)
	{
		BlackboardComp->SetValueAsObject(FName("PlayerPos"), PlayerPawn);
	}
	else
	{
		// 플레이어를 못 찾았다면 0.2초 뒤에 다시 시도
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABossAIController::RetrySetTarget, 0.2f, false);

	}
}

void ABossAIController::UpdatePhase(EBossPhase NewPhase)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(FName("CurrentPhase"), static_cast<uint8>(NewPhase));
	}
}
