// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NiagaraSystem.h"
#include "AnimNotifyState_BlackHole.generated.h"

UCLASS()
class KHAZAN_API UAnimNotifyState_BlackHole : public UAnimNotifyState
{
	GENERATED_BODY()
	
	UAnimNotifyState_BlackHole();

	// 태스크가 실행될 때 호출되는 함수
	//virtual EBTNodeResult::Type ExecuteTask(
	//	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const
		FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category = "BlackHole|Effect")
	UNiagaraSystem* SpiralEffect;

	UPROPERTY(EditAnywhere, Category = "BlackHole|Effect")
	FName SocketName = TEXT("Root");

protected:

	// 몽타주가 끝났을 때 호출되는 함수 (델리게이트로 연동)
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:

	float PullRadius = 1500.0f; // 흡입 범위
	float PullStrength = 500.0f; // 흡입 세기
	float StopDistance = 160.0f; // 끼임 방지용 흡입 중단 거리
	float DamageRadius = 400.0f; // 대미지 입힐 범위
	float DamageInterval = 0.5f; // 0.5초마다 대미지

	// LastDamageTime을 저장하기 위한 map 선언
	TMap<AActor*, float> LastDamageTimeMap;

};
