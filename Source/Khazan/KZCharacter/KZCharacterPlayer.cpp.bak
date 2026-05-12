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
	//PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);



	// 카메라 설정. 스프링 암 안에 카메라 넣기.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 600.0f;
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

	m_pStatComponent->SetUp_stat_Hp(100, 100);

	if (_InPlayerUiWidget)
	{
		// 초기값 초기화
		_InPlayerUiWidget->SetUp_Ui_Hp(m_pStatComponent->GetCurrentHp(), m_pStatComponent->GetMaxHp());


		// Ui widget의 default 값 초기화 하기.
		m_pStatComponent->Delegate_OnHpChanged.AddUObject(_InPlayerUiWidget, &UPlayerUIWidget::UpdateHp);
		m_pStatComponent->Delegate_OnHpChanged.AddUObject(_InPlayerUiWidget, &UPlayerUIWidget::UpdateProgressBarHp);
	}
}


// Called every frame
void AKZCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
			StrongAttackAction,
			ETriggerEvent::Started,
			this,
			&AKZCharacterPlayer::StrongAttack
		);


		EnhancedInputComponent->BindAction(
			UiTestAction,
			ETriggerEvent::Started,
			this,
			&AKZCharacterPlayer::UiTest
		);
	}

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
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void AKZCharacterPlayer::StopSprint(const FInputActionValue& value)
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AKZCharacterPlayer::Look(const FInputActionValue& value)
{
	// 입력값 가져오기.
	FVector2D RotationValue = value.Get<FVector2D>();

	// 회전 처리
	AddControllerYawInput(RotationValue.X);

	// 마우스를 올리면 위로 보도록 -1을 곱함.
	AddControllerPitchInput(RotationValue.Y * -1);
}

void AKZCharacterPlayer::WeakAttack(const FInputActionValue& value)
{
	ProcessAttackCommand(EAttackType::Weak);
}

void AKZCharacterPlayer::StrongAttack(const FInputActionValue& value)
{
	//StrongAttackBegin();
	ProcessAttackCommand(EAttackType::Strong);
}


// 5_11 선환 추가 
void AKZCharacterPlayer::UiTest()
{
	m_pStatComponent->Apply_Damage(50);

	m_pStatComponent->Delegate_OnHpChanged.Broadcast(m_pStatComponent->GetCurrentHp());

}


