// Fill out your copyright notice in the Description page of Project Settings.


#include "KZCharacterPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#pragma region 선환 헤더 추가 
#include "Component/StatComponent.h"
#include "UI/PlayerUIWidget.h"
#pragma endregion 
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AKZCharacterPlayer::AKZCharacterPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);



	// 카메라 설정. 스프링 암 안에 카메라 넣기.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->ProbeSize = 12.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(SpringArm);


	/* 5_11 선환 추가 Actor Component */
	
	// Actor Component
	m_pStatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));

	/* -----------------------------------  */


	// IMC 에셋 로드
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DeafultContextRef(
		TEXT("/Game/Khazan/Input/IMC_Khazan.IMC_Khazan")
	);
	if (DeafultContextRef.Succeeded())
	{
		DefaultContext = DeafultContextRef.Object;
	}

	// 입력 에셋 로드
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef{
		TEXT("/Game/Khazan/Input/Action/IA_Move.IA_Move")
	};
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef{
		TEXT("/Game/Khazan/Input/Action/IA_Look.IA_Look")
	};
	if (LookActionRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef{
	TEXT("/Game/Khazan/Input/Action/IA_Jump.IA_Jump")
	};
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> DodgeActionRef{
	TEXT("/Game/Khazan/Input/Action/IA_Dodge.IA_Dodge")
	};
	if (DodgeActionRef.Succeeded())
	{
		DodgeAction = DodgeActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> WeakAttackActionRef{
		TEXT("/Game/Khazan/Input/Action/IA_Attack.IA_Attack")
	};
	if (WeakAttackActionRef.Succeeded())
	{
		WeakAttackAction = WeakAttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> StrongAttackActionRef{
		TEXT("/Game/Khazan/Input/Action/IA_StrongAttack.IA_StrongAttack")
	};
	if (StrongAttackActionRef.Succeeded())
	{
		StrongAttackAction = StrongAttackActionRef.Object;
	}

	// 5_11 선환 추가 
	static ConstructorHelpers::FObjectFinder<UInputAction> UiTestActionRef{
		TEXT("/Game/Input/Actions/IA_UiTest.IA_UiTest")
	};
	if (UiTestActionRef.Succeeded())
	{
		UiTestAction = UiTestActionRef.Object;
	}
		static ConstructorHelpers::FObjectFinder<UInputAction> GuardActionRef{
	TEXT("/Game/Khazan/Input/Action/IA_Guard.IA_Guard")
	};
	if (GuardActionRef.Succeeded())
	{
		GuardAction = GuardActionRef.Object;
	}




}

// Called when the game starts or when spawned
void AKZCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl();
	
}


void AKZCharacterPlayer::SetupPlayerUiWidget(UPlayerUIWidget* _InPlayerUiWidget)
{
	// 설정할 플레이어의 체력 및 최대 체력

	m_pStatComponent->SetUp_stat_Hp(1000, 1000);
	m_pStatComponent->SetUp_stat_Stamina(100, 100);

	if (_InPlayerUiWidget)
	{
		// 초기값 초기화
		_InPlayerUiWidget->SetUp_Ui_Hp(m_pStatComponent->GetCurrentHp(), m_pStatComponent->GetMaxHp());
		_InPlayerUiWidget->SetUp_Ui_Stamina(m_pStatComponent->GetCurrentStamina(), m_pStatComponent->GetMaxStamina());

		// Ui widget의 default 값 초기화 하기.
		// Hp 관련 델리게이트 이벤트
		m_pStatComponent->Delegate_OnHpChanged.AddUObject(_InPlayerUiWidget, &UPlayerUIWidget::UpdateHp);
		m_pStatComponent->Delegate_OnHpChanged.AddUObject(_InPlayerUiWidget, &UPlayerUIWidget::UpdateProgressBarHp);

		// Stamina 관련 델리게이트 이벤트
		m_pStatComponent->Delegate_OnStaminaChanged.AddUObject(_InPlayerUiWidget, &UPlayerUIWidget::UpdateProgressBarStamina);


	}
}


// Called every frame
void AKZCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprint && GetVelocity().Size() > 0)
	{
		m_pStatComponent->Apply_Stamina(SprintStaminaConsumptionRate * DeltaTime);

		if (m_pStatComponent->GetCurrentStamina() <= 0)
		{
			StopSprint(FInputActionValue());
		}
	}

	/* 5_12 선환 추가 Ui 동기화 */
	m_pStatComponent->Delegate_OnStaminaChanged.Broadcast(m_pStatComponent->GetCurrentStamina(), m_pStatComponent->GetMaxStamina());
}

// Called to bind functionality to input
void AKZCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 바인딩 - 향상된 입력 시스템 컴포넌트를 활용해서 설정.
	UEnhancedInputComponent* EnhancedInputComponent
		= Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&AKZCharacterPlayer::Move
		);
		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&AKZCharacterPlayer::Look
		);

		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Triggered,
			this,
			&ACharacter::Jump
		);
		
		EnhancedInputComponent->BindAction(
			DodgeAction,
			ETriggerEvent::Triggered,
			this,
			&AKZCharacterPlayer::Dodge
		);

		EnhancedInputComponent->BindAction(
			SprintAction,
			ETriggerEvent::Started,
			this,
			&AKZCharacterPlayer::Sprint
		);
		EnhancedInputComponent->BindAction(
			SprintAction,
			ETriggerEvent::Completed,  // <- 뗐을 때 발동
			this,
			&AKZCharacterPlayer::StopSprint
		);

		EnhancedInputComponent->BindAction(
			WeakAttackAction,
			ETriggerEvent::Started, 
			this,
			&AKZCharacterPlayer::WeakAttack
		);
		EnhancedInputComponent->BindAction(
			WeakAttackAction,
			ETriggerEvent::Triggered,       
			this,
			&AKZCharacterPlayer::WeakAttackTriggered
		);
		EnhancedInputComponent->BindAction(
			WeakAttackAction,
			ETriggerEvent::Completed,
			this,
			&AKZCharacterPlayer::WeakAttackCompleted
		);

		EnhancedInputComponent->BindAction(
			StrongAttackAction,
			ETriggerEvent::Started,
			this,
			&AKZCharacterPlayer::StrongAttack
		);
		EnhancedInputComponent->BindAction(
			GuardAction,
			ETriggerEvent::Started,
			this,
			&AKZCharacterPlayer::Guard
		);
		EnhancedInputComponent->BindAction(
			GuardAction,
			ETriggerEvent::Completed,
			this,
			&AKZCharacterPlayer::StopGuard
		);

		EnhancedInputComponent->BindAction(
			UiTestAction,
			ETriggerEvent::Started,
			this,
			&AKZCharacterPlayer::UiTest
		);
	}

}

void AKZCharacterPlayer::SetStaminaRegenBlock(bool bBlocked)
{
	if (m_pStatComponent)
	{
		m_pStatComponent->bIsStaminaRegenBlocked = bBlocked;
	}
}

void AKZCharacterPlayer::ApplyStaminaTest(float value)
{
	if (m_pStatComponent)
	{
		m_pStatComponent->Apply_Stamina(value);
	}
}

bool AKZCharacterPlayer::HasEnoughStamina(float value)
{
	if (m_pStatComponent)
	{
		return m_pStatComponent->GetCurrentStamina() >= value;
	}
	return false;
}

void AKZCharacterPlayer::SetCharacterControl()
{
	// 입력 매핑 컨텍스트 설정.
	APlayerController* PlayerController
		= Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// 향상된 입력 시스템의 서브 시스템 가져오기.
		UEnhancedInputLocalPlayerSubsystem* InputSystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PlayerController->GetLocalPlayer()
			);
		// 매핑 컨텍스트 추가 (우선순위 0)
		InputSystem->AddMappingContext(DefaultContext, 0);
	}

}

void AKZCharacterPlayer::Move(const FInputActionValue& value)
{
	// 약공격 혹은 강공격 애니메이션 몽타주 진행 중에 끊고 움직이려는 경우
	// Move에서 강제로 몽타주를 끄고 움직임 활성화.
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && WeakAttackMontage && AnimInstance->Montage_IsPlaying(WeakAttackMontage))
		{
			AnimInstance->Montage_Stop(0.2f, WeakAttackMontage);
		}
		else if (AnimInstance && StrongAttackMontage && AnimInstance->Montage_IsPlaying(StrongAttackMontage))
		{
			AnimInstance->Montage_Stop(0.2f, StrongAttackMontage);
		}
	}
	// 입력값 = Vector
	FVector Movement = value.Get<FVector>();

	// 이동할 방향 -> Rotator
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);


	// 앞 방향.
	FVector ForwardVector
		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// 오른쪽 방향.
	FVector RightVector
		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector, Movement.Y);
	AddMovementInput(RightVector, Movement.X);

}

void AKZCharacterPlayer::Sprint(const FInputActionValue& value)
{
	if (m_pStatComponent->GetCurrentStamina() > 0)
	{
		bIsSprint = true;
		m_pStatComponent->bIsStaminaRegenBlocked = true;
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	}
}

void AKZCharacterPlayer::StopSprint(const FInputActionValue& value)
{
	bIsSprint = false;
	m_pStatComponent->bIsStaminaRegenBlocked = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void AKZCharacterPlayer::Look(const FInputActionValue& value)
{
	// 입력값 가져오기.
	FVector2D RotationValue = value.Get<FVector2D>();

	

	// 회전 처리
	AddControllerYawInput(RotationValue.X * 0.7);

	// 마우스를 올리면 위로 보도록 -1을 곱함.
	AddControllerPitchInput((RotationValue.Y * -1) * 0.5);
}

// 회피
void AKZCharacterPlayer::Dodge(const FInputActionValue& value)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	if (m_pStatComponent->GetCurrentStamina() < 25 || AnimInstance->Montage_IsPlaying(DodgeMontage) || bIsDodge) return;

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	

	/* Stamina 관련 테스트 코드(5_12 선환) */
	m_pStatComponent->Apply_Stamina(25);

	FVector InputVector = GetLastMovementInputVector();

	// 아무런 입력 없이 회피 누를 시 Back 으로 회피 몽타주 실행.
	if (InputVector.IsNearlyZero())
	{
		PlayDodgeMontage("Dodge_B");
		return;
	}


	// UnrotateVector -> 캐릭터가 바라보는 정면을 0도로 놓고 계산.
	FVector LocalInput = GetCharacterMovement()->GetLastUpdateRotation().UnrotateVector(InputVector);

	// 입력을 했을 때 회피를 할 8방향을 정하기 위해 Atan2를 이용.
	float Radians = FMath::Atan2(LocalInput.Y, LocalInput.X);
	float Angle = FMath::RadiansToDegrees(Radians);

	FName TargetSection = DetermineDodgeSection(Angle);
	PlayDodgeMontage(TargetSection);

}

void AKZCharacterPlayer::WeakAttack(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling() && m_pStatComponent->GetCurrentStamina() <= 0)
	{
		m_pStatComponent->bIsStaminaRegenBlocked = false;
		return;
	}
	m_pStatComponent->bIsStaminaRegenBlocked = true;
	bIsCharging = true;
	CurrentChargeTime = 0.0f;

	if (CurrentAttackType != EAttackType::None)
	{
		ProcessAttackCommand(EAttackType::Weak);
		return;
	}

	ProcessAttackCommand(EAttackType::Weak);
}

// 차징 시간을 확인하기 위해 설정.
void AKZCharacterPlayer::WeakAttackTriggered(const FInputActionValue& value)
{
	if (bIsCharging)
	{
		CurrentChargeTime += GetWorld()->GetDeltaSeconds();
	}
}

void AKZCharacterPlayer::WeakAttackCompleted(const FInputActionValue& value)
{
	if (!bIsCharging) return;

	// 차징한 시간이 일정 시간보다 길면 차징공격하도록 설정.
	bool bIsChargedAttack = (CurrentChargeTime >= ChargeThreshold);

	ChargeWeakAttackBegin(bIsChargedAttack);

	bIsCharging = false;
	CurrentChargeTime = 0.0f;
}


void AKZCharacterPlayer::StrongAttack(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling() && m_pStatComponent->GetCurrentStamina() <= 0)
	{
		return;
	}
	m_pStatComponent->bIsStaminaRegenBlocked = true;
	//StrongAttackBegin();
	ProcessAttackCommand(EAttackType::Strong);

}


// 5_11 선환 추가 
void AKZCharacterPlayer::UiTest()
{
	/* hp 관련 테스트 코드(5_11 선환) */
	//m_pStatComponent->Apply_Damage(50);
	//
	//m_pStatComponent->Delegate_OnHpChanged.Broadcast(m_pStatComponent->GetCurrentHp());
}



void AKZCharacterPlayer::Guard(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling() || CurrentAttackType != EAttackType::None)
	{
		return;
	}
	bIsGuarding = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	//PlayGuardMontage();
}

void AKZCharacterPlayer::StopGuard(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling() || CurrentAttackType != EAttackType::None)
	{
		return;
	}
	bIsGuarding = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	//StopAnimMontage(GuardMontage);
}

// 데미지를 받은 입장.
void AKZCharacterPlayer::ProcessDamage(const FDamageData& DamageData)
{
	if (bIsDead || bIsInvincible) { return; }

	LastAttacker = DamageData.Attacker;
	//float FinalDamage = DamageData.DamageAmount;

	FString IntensityStr = GetIntensityString(DamageData.DamageAmount);
	FString SwingStr = GetSwingDirString();
	FString PosStr = (IntensityStr == "Strong") ? TEXT("F") : GetAttackerPosString(DamageData.Attacker);

	FName SectionName = *FString::Printf(TEXT("%s%s%s"), *IntensityStr, *SwingStr, *PosStr);

	if (m_pStatComponent)
	{
		m_pStatComponent->Apply_Damage(DamageData.DamageAmount);
		m_pStatComponent->Delegate_OnHpChanged.Broadcast(m_pStatComponent->GetCurrentHp());
		if (m_pStatComponent->GetCurrentHp() <= 0)
		{
			bIsDead = true;
			Dead();
			return;
		}

		if (HitMontage)
		{
			GetCharacterMovement()->StopMovementImmediately();
			//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			PlayAnimMontage(HitMontage, 1.0f, SectionName);

			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && !AnimInstance->Montage_IsPlaying(HitMontage))
			{
				// 몽타주 종료 이벤트에 등록할 델리게이트 설정.
				FOnMontageEnded OnMontageEnded;
				OnMontageEnded.BindUObject(this, &AKZCharacterPlayer::HitMontageEnd);

				// 몽타주 재생 종료 시 발행되는 이벤트에 등록.
				AnimInstance->Montage_SetEndDelegate(OnMontageEnded, HitMontage);
			}
		}
	}
}

void AKZCharacterPlayer::Dead()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreMoveInput(true);
		PC->SetIgnoreLookInput(true);
	}
}

FString AKZCharacterPlayer::GetAttackerPosString(AActor* Attacker)
{

	FVector Forward = GetActorForwardVector();
	FVector ToAttacker = (Attacker->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	float Dot = FVector::DotProduct(Forward, ToAttacker);

	return (Dot >= 0.0f) ? TEXT("F") : TEXT("B");

}

FString AKZCharacterPlayer::GetSwingDirString()
{

	return (FMath::RandBool()) ? TEXT("L") : TEXT("R");
}

FString AKZCharacterPlayer::GetIntensityString(float DamageAmount)
{
	if (DamageAmount < 10.0f)
	{
		return TEXT("Weak");
	}
	else if (DamageAmount > 10.0f && DamageAmount <= 30.0f)
	{
		return TEXT("Normal");
	}
	else if (DamageAmount > 30.0f)
	{
		return TEXT("Strong");
	}
	else
	{
		return TEXT("Normal");
	}
}

void AKZCharacterPlayer::HitMontageEnd(UAnimMontage* TargetMontage, bool bInterrupted)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
