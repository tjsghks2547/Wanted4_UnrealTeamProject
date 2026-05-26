// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../KZMonster/KZMonsterCharacter.h"
#include "BossAI/EBossPhase.h"
#include "KZBossCharacter.generated.h"

UCLASS()
class KHAZAN_API AKZBossCharacter : public AKZMonsterCharacter//, public IKZDamageInterface
{
	GENERATED_BODY()

public:
	// IIInteractableTarget 인터페이스 오버라이드
	virtual FName GetTargetType() const override { return FName("Boss"); }

	// Sets default values for this character's properties
	AKZBossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Landed(const FHitResult& Hit) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 공격 실행 함수 (몽타주 재생)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void PlayAttackMontage() override;
	
	// 원거리 공격 실행 함수 (몽타주 재생)
	UFUNCTION(BlueprintCallable, Category = "Combat LongRange")
	void PlayJumpAttackMontage();

	UFUNCTION(BlueprintCallable, Category = "Combat LongRange")
	virtual void PlayLongRangeAttackMontage() override;

	UFUNCTION(BlueprintCallable, Category = "Combat LongRange")
	void PlayDashAttackMontage();

	// 특수  애니메이션 몽타주 Movement 입력 함수
	UFUNCTION(BlueprintCallable, Category = "Combat Movement")
	void ExecuteBackStep();

	UFUNCTION(BlueprintCallable, Category = "Combat Movement")
	void ExecuteDash();

	UFUNCTION(BlueprintCallable, Category = "Combat Movement")
	void ExecuteJump();

	//UFUNCTION(BlueprintCallable, Category = "Combat")
	//void ExeCuteThrowBlade();

	// 페이즈 변환 함수
	UFUNCTION(BlueprintCallable, Category = "Boss State")
	void ChangePhase();

	// 페이즈 변환시 시전할 몽타주 호출 함수
	UFUNCTION(BlueprintCallable, Category = "Boss State")
	void PlayPhaseChangingAttackMontage();

public:

	// 일반 공격 몽타주 변수 (에디터에서 설정 가능)
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	//class UAnimMontage* BasicAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AMontagePhase1.5")
	class UAnimMontage* JumpAttackMontage;

	// 원거리 후 대시 공격 몽타주 변수 (에디터에서 설정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AMontagePhase1.5")
	class UAnimMontage* PhaseChangingAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AMontagePhase1.5")
	class UAnimMontage* ThrowAndDashAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AMontagePhase1.5")
	class UAnimMontage* DashAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AMontagePhase1.5")
	class UAnimMontage* DashAttackEndMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AMontagePhase1.5")
	float DashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AMontagePhase1.5")
	float DashUpForce;

	// 백스텝 공격 몽타주 변수 (에디터에서 설정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimMontage* BackStepAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float UpForce;


	// 현재 보스의 페이즈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss State")
	EBossPhase CurrentPhase;


	// 재생할 몽타주를 저장하는 변수
	UPROPERTY(BlueprintReadOnly, Category ="Combat")
	UAnimMontage* CurrentMontage;
	
protected:
	//UPROPERTY(VisibleAnywhere, Category = Stat)
	//TObjectPtr<class UStatComponent> m_pStatComponent;

	// IKZDamageInterface을(를) 통해 상속됨
	//UFUNCTION(BlueprintCallable)
	void ProcessDamage(const FDamageData& DamageData) override;

private:
	bool bIsJumpingToPlayer = false; // 추적 시작 플래그
};
