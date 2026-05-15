// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "../KZBossCharacter.h"
#include "Kismet/GameplayStatics.h" // �÷��̾� ���� �������� ����

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
			// 블랙보드 거리 변수 초기화 (처음 0으로 시작하여 공격하고 시작함을 방지)
			BlackboardComp->SetValueAsFloat(FName("Distance"), 9999.0f);

			// BT 실행
			RunBehaviorTree(BTBoss);

			AKZBossCharacter* Viper = Cast<AKZBossCharacter>(InPawn);
			if (Viper)
			{
				// 보스의 현재 페이즈를 블랙보드에 저장
				BlackboardComp->SetValueAsEnum(FName("CurrentPhase"), (uint8)Viper->CurrentPhase);
			}

			// 플레이어 타겟 설정 시도
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
		SetFocus(PlayerPawn);
	}
	else
	{
		// 플레이어를 못 찾았을 경우 0.2초 후에 다시 시도
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
