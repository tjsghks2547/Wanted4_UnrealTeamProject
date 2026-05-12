// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossAI/EBossPhase.h"
#include "KZBossCharacter.generated.h"

UCLASS()
class KHAZAN_API AKZBossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
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
	void PlayAttackMontage();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ExecuteBackStep();

public:

	// 일반 공격 몽타주 변수 (에디터에서 설정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimMontage* BasicAttackMontage;

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

	// 이동속도 변수
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CurrentMovementSpeed;

	// 재생할 몽타주를 저장하는 변수
	UPROPERTY(BlueprintReadOnly, Category ="Combat")
	UAnimMontage* CurrentMontage;
};
