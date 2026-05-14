// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EBossPhase.h"
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

protected:

	// 보스가 스폰되어 이 컨트롤러가 빙의했을 때 실행
	virtual void OnPossess(APawn* InPawn) override;

	void RetrySetTarget();

	void UpdatePhase(EBossPhase NewPhase);

public:

	// 에디터에서 드래그 앤 드롭으로 할당할 비헤이비어 트리 애셋
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BTBoss;

	// 블랙보드 컴포넌트 (생성자에서 초기화)
	UPROPERTY()
	class UBlackboardComponent* BlackboardComp;
};
