// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "KZCharacterBase.h"
#include "../Interface/IInteractableTarget.h"
#include "Interface/PlayerUiWidget_Interface.h"
#include "KZCharacterPlayer.generated.h"

// 전방선언.
class UInputAction;

UCLASS()
class KHAZAN_API AKZCharacterPlayer : 
	public AKZCharacterBase,
	public IIInteractableTarget,
	public IPlayerUiWidget_Interface /* 5_11 선환 추가 ( UI Widget과 Player 의존성 없애기 위해 인터페이스 구현 ) */
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKZCharacterPlayer();

protected:
	// Called when the game starts or when spawned
	// 입력 매핑 컨텍스트를 넣는데엔 beginplay
	virtual void BeginPlay() override;


	// IPlayerUiWidget_Interface을(를) 통해 상속됨  ( 5_11 선환 추가 ) 
	void SetupPlayerUiWidget(UPlayerUIWidget* _InPlayerUiWidget) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// 언리얼 엔진 인풋시스템에서 move와 look 함수를 서로 매핑 시켜주는것을 미리 완료시키는 역할을 할 예정.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 카메라
	 
	
	// 인터페이스 함수 오버라이드
	virtual FName GetTargetType() const override { return FName("Player"); }
	virtual bool IsAttackable() const override { return true; }

protected:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class UCameraComponent> Camera;

	// 5_11 선환 추가
	UPROPERTY(VisibleAnywhere, Category = Stat)
	TObjectPtr<class UStatComponent> m_pStatComponent; 

	// 입력 액션
protected:

	void SetCharacterControl();

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputMappingContext> DefaultContext;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> WeakAttackAction;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> StrongAttackAction;

	// 5_11 선환 ( Ui Test를 위해 임시 추가 ) 
	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> UiTestAction;


	void Move(const FInputActionValue& value);
	void Sprint(const FInputActionValue& value);
	void StopSprint(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void WeakAttack(const FInputActionValue& value);
	void StrongAttack(const FInputActionValue& value);
	void UiTest();




};
