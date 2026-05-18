// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/KZAnimationAttackInterface.h"
#include "../Interface/KZDamageInterface.h"
#include "KZCharacterBase.generated.h"

UENUM()
enum class EAttackType : uint8
{
	None,
	Weak,
	Strong
};

UCLASS()
class KHAZAN_API AKZCharacterBase : public ACharacter, public IKZAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKZCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 컨트롤 데이터 설정.
	// LockOn용으로 쓰기 좋아보임.
	// 유저의 컨트롤 설정(카메라, 회전 등)을 한번에 바꾸는 용도로 사용.
	//virtual void SetCharacterContolData(
	//	const class UABCharacterControlData* InCharacterControlData);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AttackCheck() override;

	virtual void SetStaminaRegenBlock(bool bBlocked) {};
	virtual void ApplyStaminaTest(float value) {};
	virtual bool HasEnoughStamina(float value) { return true;  }

	void EnableWeaponCollision();
	void DisableWeaponCollision();

	// 공격
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TObjectPtr<class UAnimMontage> WeakAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TObjectPtr<class UAnimMontage> StrongAttackMontage;

	// 공격 시작 함수.
	// 다음 공격의 종류를 받아서 넘기기.
	void ProcessAttackCommand(EAttackType AttackType);

	void WeakAttackBegin();

	void ChargeWeakAttackBegin(bool bIsCharged);

	void StrongAttackBegin();

	void AttackActionEnd(UAnimMontage* TargetMontage, bool bInterrupted);


	UPROPERTY(EditAnywhere, category = Attack)
	uint32 CurrentCombo = 0;

	UPROPERTY(EditAnywhere, category = Attack)
	uint32 MaxCombo = 3;

	UPROPERTY(VisibleAnywhere, category = Attack)
	bool bNextCombo = false;

	// 현재 공격의 타입 (약/공)
	UPROPERTY(VisibleAnywhere, category = Attack)
	EAttackType CurrentAttackType = EAttackType::None;

	// 입력을 받은 다음 공격의 타입 (약/공)
	UPROPERTY(VisibleAnywhere, category = Attack)
	EAttackType NextAttackType = EAttackType::None;

	// 공격 도중에 모션을 끊고 움직이고 싶은 경우.
	void AttackEndCheck() override;

	// IKZAnimationAttackInterface을(를) 통해 상속됨
	void LaunchCharacterNotify(float LaunchForce) override;

	bool bIsCharging = false;

	// 가드
protected:
	UPROPERTY(EditAnywhere, Category = Guard)
	TObjectPtr<UAnimMontage> GuardMontage;

	UPROPERTY(EditAnywhere, Category = Guard, BlueprintReadWrite)
	bool bIsGuarding = false;

	void PlayGuardMontage();

	// 무기
protected:
	UPROPERTY(EditAnywhere, category = Weapon)
	TObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, category = Weapon)
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, category = Weapon)
	TObjectPtr<class UBoxComponent> WeaponCollision;



	UFUNCTION()
	void OnWeaponOverlap(
		UPrimitiveComponent * OverlappedComponent,
		AActor * OtherActor,
		UPrimitiveComponent * OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult & SweepResult);

public:
	UPROPERTY(EditAnywhere, Category = "HitActor")
	TArray<AActor*> AlreadyHitActor;

	// 회피
protected:
	UPROPERTY(VisibleAnywhere, category = Dodge)
	bool bIsDodge = false;

	UPROPERTY(EditAnywhere, Category = Dodge)
	UAnimMontage* DodgeMontage;

	void PlayDodgeMontage(FName Section);
	void DodgeMontageEnd(UAnimMontage* TargetMontage, bool bInterrupted);
	UFUNCTION()
	FName DetermineDodgeSection(float Angle);


// 피격
protected:
	UPROPERTY(EditAnywhere, Category = Hit)
	UAnimMontage* HitMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hit)
	TObjectPtr<AActor> LastAttacker;


	// 점프
protected:
	UPROPERTY(EditAnywhere, Category = Jump)
	UAnimMontage* JumpMontage;
};
