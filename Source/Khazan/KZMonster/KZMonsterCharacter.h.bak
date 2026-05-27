// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/IInteractableTarget.h"
#include "../Interface/KZDamageInterface.h"
#include "../Interface/KZAnimationAttackInterface.h"
#include "../Interface/KZLockOnInterface.h"
#include "KZMonsterCharacter.generated.h"


class AAIController;
class UBlackboardComponent;

// 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonsterDamaged, AActor*, Attacker);

UCLASS()
class KHAZAN_API AKZMonsterCharacter : public ACharacter, public IIInteractableTarget, public IKZDamageInterface, public IKZAnimationAttackInterface, public IKZLockOnInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKZMonsterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// 인터페이스 구현
	virtual FName GetTargetType() const override { return FName("Monster"); }
	virtual bool IsAttackable() const override { return true; }

	virtual void PlayAttackMontage();

	virtual void PlayLongRangeAttackMontage();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimMontage* BasicAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimMontage* AdditiveHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimMontage* DeathMontage;

	// 백스텝 공격 몽타주 변수 (에디터에서 설정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimMontage* LongRangeAttackMontage;
	
	void Dead();

	void PlayDeathMontage();

	void HitMontageEnd(UAnimMontage* TargetMontage, bool bInterrupted);

	bool IsDeathAnimationDone() const { return bDeathAnimationDone;}


	// IKZAnimationAttackInterface을(를) 통해 상속됨
	virtual void AttackCheck() override;
	
	virtual void AttackEndCheck() override;

	void LaunchCharacterNotify(float LaunchForce) override;

	//Setter
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void IsAttackEnd();


protected:

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CurrentMovementSpeed;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	TObjectPtr<class UStatComponent> StatComponent;

	// IKZDamageInterface을(를) 통해 상속됨
	UFUNCTION(BlueprintCallable)
	void ProcessDamage(const FDamageData& DamageData) override;

	UPROPERTY()
	TObjectPtr<AAIController> AIC;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComp;
	
	// IKZLockOnInterface을(를) 통해 상속됨
	bool CanTargetLockOn() override;
	FVector GetTargetLocation() override;

	// 죽음 이벤트를 위한 플래그
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	bool bIsDead = false;

	UPROPERTY()
	bool bDeathAnimationDone = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hit)
	TObjectPtr<AActor> LastAttacker;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMonsterDamaged OnMonsterDamaged;
	
	
/* 5_24 선환 추가*/

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UWidgetComponent> Hp_Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UWidgetComponent> Stamina_Widget;

	UPROPERTY(VisibleAnywhere)
	FName Name;


};
