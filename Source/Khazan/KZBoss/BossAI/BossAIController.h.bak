// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EBossPhase.h"
//#include "Perception/AIPerceptionTypes.h"
#include "BossAIController.generated.h"

/**
 *
 */
UCLASS()
class KHAZAN_API ABossAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABossAIController();

	// Perception 컴포넌트 전방선언
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAIPerceptionComponent* AIPerceptionComponent;

protected:

	// 감지 업데이트 콜백
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// 시야 범위 설정 가능하도록 변수화
	UPROPERTY(EditAnywhere, Category = "AI")
	float SightRadius = 1500.0f;

public:

	// 보스가 스폰되어 이 컨트롤러가 빙의했을 때 실행
	virtual void OnPossess(APawn* InPawn) override;

	//void RetrySetTarget();

	void UpdatePhase(EBossPhase NewPhase);

public:

	// 에디터에서 드래그 앤 드롭으로 할당할 비헤이비어 트리 애셋
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BTBoss;

	// 블랙보드 컴포넌트 (생성자에서 초기화)
	UPROPERTY()
	class UBlackboardComponent* BlackboardComp;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	//TObjectPtr<class UBehaviorTree> BT;

	// 시야 설정 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAISenseConfig_Sight* SightConfig;
};