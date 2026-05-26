// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "KZCharacterBase.h"
#include "../Interface/IInteractableTarget.h"
#include "Interface/PlayerUiWidget_Interface.h"
#include "../Interface/KZDamageInterface.h"
#include "../Interface/KZLockOnInterface.h"
#include "KZCharacterPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLockOnStateChanged, bool, bInIsLockedOn);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerDeadDelegate, AKZCharacterPlayer* /*DeadPlayer*/);

// 전방선언.
class UInputAction;

/* 5_18 선환 추가 */
enum class EInterActionType : uint8;
enum class EInterAction_Key_Type : uint8;



UCLASS()
class KHAZAN_API AKZCharacterPlayer : 
	public AKZCharacterBase,
	public IIInteractableTarget,
	public IPlayerUiWidget_Interface, /* 5_11 선환 추가 ( UI Widget과 Player 의존성 없애기 위해 인터페이스 구현 ) */
	public IKZDamageInterface,
	public IKZLockOnInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKZCharacterPlayer();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLockOnStateChanged OnLockOnStateChanged;
	FOnPlayerDeadDelegate OnPlayerDead;

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

	virtual void SetStaminaRegenBlock(bool bBlocked) override;
	virtual void ApplyStaminaTest(float value) override;
	virtual bool HasEnoughStamina(float value) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class UCameraComponent> Camera;

	// 5_11 선환 추가
	UPROPERTY(VisibleAnywhere, Category = Stat)
	TObjectPtr<class UStatComponent> StatComponent;

	// 5_18 선환 추가 
	UPROPERTY(VisibleAnywhere, Category = UI)
	TObjectPtr<class UUi_InterAction_Component> UiComponent; 

	// 5_20 선환 추가 
	UPROPERTY(VisibleAnywhere, Category = Component)
	TObjectPtr<class UInventoryComponent> InventoryComponent;


	UPROPERTY(EditAnywhere, Category = Stat)
	float SprintStaminaConsumptionRate = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	bool bIsSprint = false;

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
	TObjectPtr<class UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> WeakAttackAction;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> StrongAttackAction;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> GuardAction;

	// 5_11 선환 ( Ui Test를 위해 임시 추가 ) 
	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> InventoryOpenAction;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> LockOnAction;

	// 5_21 추가 
	UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> Ui_Inventory;


	UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputAction> Ui_InterAction;




	void Move(const FInputActionValue& value);
	void Sprint(const FInputActionValue& value);
	void StopSprint(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void Dodge(const FInputActionValue& value);
	void WeakAttack(const FInputActionValue& value);
	void StrongAttack(const FInputActionValue& value);
	void Guard(const FInputActionValue& value);
	void StopGuard(const FInputActionValue& value);
	void LockOn(const FInputActionValue& value);
	void InventoryOpen();

	// 점프
public:
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

	void ExcutePhysicsJump();


	// 차징 공격
protected:
	void WeakAttackTriggered(const FInputActionValue& value);
	void WeakAttackCompleted(const FInputActionValue& value);
	
	float CurrentChargeTime = 0.0f;
	
	const float ChargeThreshold = 0.2f;

	// 충돌판정
protected:
	// 데미지를 받으면 이 함수가 실행됨.
	UFUNCTION(BlueprintCallable)
	void ProcessDamage(const FDamageData& DamageData) override;

	void Dead();

	// 죽음 이벤트를 위한 플래그
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	bool bIsDead = false;

	FString GetAttackerPosString(AActor* Attacker);

	FString GetSwingDirString();

	FString GetIntensityString(float DamageAmout);

	void HitMontageEnd(UAnimMontage* TargetMontage, bool bInterrupted);
	
/* 5_18 선환 추가 ( Ui Dialog Rendering 관련 )  */
public:
	FORCEINLINE void Set_Current_OverlapTypes(EInterActionType _Tag)
	{
		InterActionType = _Tag;
	};
	void Set_Finish_Ui_Key_InterAction(bool _InbKeyFinished)
	{
		HasUiKeyFinished = _InbKeyFinished;
	};

	// 박스와 충돌할때 발생시키는 함수 
	void Render_InterActionUi(EInterActionType _Tag, ESlateVisibility _eSlateVisibility);	
	void Ui_Key_State_Reset();

	bool Get_Ui_Key_Statue() { return HasUiKeyFinished; }

	void Inventory_Close();
	void InterAction();

protected:
	UPROPERTY(VisibleAnywhere, Category = OverlapType, BlueprintReadOnly)
	EInterActionType InterActionType;

	UPROPERTY(VisibleAnywhere)	
	bool HasUiKeyFinished;
/* ---------------------------------------------------- */


	// 회피 시 무적을 위한 Set
public:
	FORCEINLINE void SetInvincible(bool IsInvincible) { bIsInvincible = IsInvincible; }
	FORCEINLINE bool IsInvincible() const { return bIsInvincible; }

protected:
	UPROPERTY(EditAnywhere, Category = Dodge)
	bool bIsInvincible = false;

	// 저스트 가드
protected:
	float GuardStartTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = Guard)
	float JustGuardWindow = 0.2f;

	// 락온
protected:
	UPROPERTY(EditAnywhere, Category = LockOn)
	AActor* LockOnTarget = nullptr;
	UPROPERTY(EditAnywhere, Category = LockOn)
	float LockOnMaxDistance;
	UPROPERTY(EditAnywhere, Category = LockOn)
	float DetectRad;


	UFUNCTION()
	void UpdateMovementForLockOn(bool bInIsLockOn);
	// IKZLockOnInterface을(를) 통해 상속됨
	bool CanTargetLockOn() override;

	FVector GetTargetLocation() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = LockOn)
	bool bIsLockOn = false;


protected:
	// 저스트 가드 이펙트
	UPROPERTY(EditAnyWhere, Category = Effect)
	TObjectPtr<class UNiagaraSystem> ParryEffect;

	// 가드 이펙트
	UPROPERTY(EditAnyWhere, Category = Effect)
	TObjectPtr<class UNiagaraSystem> GuardEffect;
};
