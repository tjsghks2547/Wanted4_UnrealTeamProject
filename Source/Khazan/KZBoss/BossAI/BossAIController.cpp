// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "../KZBossCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h" // UAISenseConfig_Sight 정의 포함

ABossAIController::ABossAIController()
{
	// Perception 컴포넌트 생성
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// 시야(Sight) 설정
	SightConfig->SightRadius = 2000.0f; // 감지 범위
	SightConfig->LoseSightRadius = 2000.0f; // 감지 해제 범위
	SightConfig->PeripheralVisionAngleDegrees = 180.0f; // 시야각(양옆 45도 씩 총 360도)
	SightConfig->SetMaxAge(1.0f); // 타겟을 놓친 후 기억하는 시간

	// 감지 대상 설정 (기본적으로 모두 감지하도록 설정)
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;


	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

}

void ABossAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// 감지 성공 여부 확인
	if (Stimulus.WasSuccessfullySensed())
	{
		// 플레이어인지 확인 (태그 또는 Cast 사용가능)
		APawn* SensedPawn = Cast<APawn>(Actor);
		if (SensedPawn && SensedPawn->IsPlayerControlled())
		{
			BlackboardComp = GetBlackboardComponent();
			if (BlackboardComp)
			{
				BlackboardComp->SetValueAsObject(FName("PlayerPos"), Actor);
			}
			//// 블랙보드 값 업데이트 (기본 PlayerPos 키 사용)
			//BlackboardComp->SetValueAsObject(FName("PlayerPos"), Actor);
			//SetFocus(Actor); // 필요 시 타겟 따라보기
		}
	}
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 델리게이트 바인딩
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABossAIController::OnTargetPerceptionUpdated);

	if (BTBoss)// && BTBoss->BlackboardAsset
	{
		// BT 실행
		if (RunBehaviorTree(BTBoss))
		{
			// 엔진이 생성한 블랙보드 컴포넌트를 멤버 변수에 할당
			BlackboardComp = GetBlackboardComponent();

			if (BlackboardComp)
			{
				// 블랙보드 거리 변수 초기화 (처음 0으로 시작하여 공격하고 시작함을 방지)
				BlackboardComp->SetValueAsFloat(FName("Distance"), 9999.0f);

				// 블랙보드 스폰위치를 초기화(저장)
				BlackboardComp->SetValueAsVector(FName("HomePos"), InPawn->GetActorLocation());

				// 블랙보드 범위 지정
				BlackboardComp->SetValueAsFloat(FName("PatrolRadius"), 2000.0f);



				AKZBossCharacter* Viper = Cast<AKZBossCharacter>(InPawn);
				if (Viper)
				{
					// 보스의 현재 페이즈를 블랙보드에 저장
					BlackboardComp->SetValueAsEnum(FName("CurrentPhase"), (uint8)Viper->CurrentPhase);
				}

				// 플레이어 타겟 설정 시도
				//RetrySetTarget();

			}

		}

	}

}
//
//void ABossAIController::RetrySetTarget()
//{
//	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
//
//	if (PlayerPawn)
//	{
//		BlackboardComp->SetValueAsObject(FName("PlayerPos"), PlayerPawn);
//		SetFocus(PlayerPawn);
//	}
//	else
//	{
//		// 플레이어를 못 찾았을 경우 0.2초 후에 다시 시도
//		FTimerHandle TimerHandle;
//		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABossAIController::RetrySetTarget, 0.2f, false);
//
//	}
//}

void ABossAIController::UpdatePhase(EBossPhase NewPhase)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(FName("CurrentPhase"), static_cast<uint8>(NewPhase));
	}
}
