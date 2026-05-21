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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 공격 실행 함수 (몽타주 재생)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void PlayAttackMontage() override;
	
	// 원거리 공격 실행 함수 (몽타주 재생)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void PlayLongRangeAttackMontage() override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ExecuteBackStep();

	//UFUNCTION(BlueprintCallable, Category = "Combat")
	//void ExeCuteThrowBlade();

public:

	// 일반 공격 몽타주 변수 (에디터에서 설정 가능)
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	//class UAnimMontage* BasicAttackMontage;

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
	UFUNCTION(BlueprintCallable)
	void ProcessDamage(const FDamageData& DamageData) override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* AdditiveHitMontage;
};
