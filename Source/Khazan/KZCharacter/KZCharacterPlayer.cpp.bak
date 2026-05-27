// Fill out your copyright notice in the Description page of Project Settings.


#include "KZCharacterPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#pragma region 선환 헤더 추가 
#include "Component/StatComponent.h"
#include "Component/Ui_InterAction_Component.h"
#include "Component/InventoryComponent.h"
#include "UI/PlayerUIWidget.h"
#include "UI/QuickSlotWidget.h"
#include "Types/InterActionType.h"
#include "KZPlayer/KZPlayerController.h"
#include "HUD/IH_HUD.h"
#include "Player/IHPlayerState.h"
#include "Data/ItemDataTable.h"

#pragma endregion 
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "../Game/KZGamemode.h"
#include "Components/CapsuleComponent.h"

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


	/* 5_18 선환 추가 Actor Component */
	
	// Actor Component
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	UiComponent = CreateDefaultSubobject<UUi_InterAction_Component>(TEXT("UiComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("UiInventory"));
	// Scene Component


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
	static ConstructorHelpers::FObjectFinder<UInputAction> InventroyOpenActionRef{
		TEXT("/Game/Khazan/Input/Action/IA_InventoryOpen.IA_InventoryOpen")
	};
	if (InventroyOpenActionRef.Succeeded())
	{
		InventoryOpenAction = InventroyOpenActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> GuardActionRef{
		TEXT("/Game/Khazan/Input/Action/IA_Guard.IA_Guard")
	};
	if (GuardActionRef.Succeeded())
	{
		GuardAction = GuardActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LockOnActionRef{
		TEXT("/Game/Khazan/Input/Action/IA_LockOn.IA_LockOn")
	};
	if (LockOnActionRef.Succeeded())
	{
		LockOnAction = LockOnActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Ui_InventoryActionRef{
		TEXT("/Game/Khazan/Input/UI/IA_Inventory.IA_Inventory")
	};
	if (Ui_InventoryActionRef.Succeeded())
	{
		Ui_Inventory = Ui_InventoryActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Ui_InterActionRef{
		TEXT("/Game/Khazan/Input/Action/IA_InterAction.IA_InterAction")
	};
	if (Ui_InterActionRef.Succeeded())
	{
		Ui_InterAction = Ui_InterActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> QuickSlot_InputActionRef(TEXT("/Game/Khazan/Input/Action/IA_QuickSlotButton.IA_QuickSlotButton"));
	if (QuickSlot_InputActionRef.Object != NULL)
	{
		QuickSlot_InputAction = QuickSlot_InputActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> DeadTestActionRef{
	TEXT("/Game/Khazan/Input/Action/IA_DeadTest.IA_DeadTest")
	};
	if (DeadTestActionRef.Succeeded())
	{
		DeadTestAction = DeadTestActionRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ParryEffectRef{
	TEXT("/Game/Effect/Parry_Effect.Parry_Effect")
	};
	if (ParryEffectRef.Succeeded())
	{
		ParryEffect = ParryEffectRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> GuardEffectRef{
	TEXT("/Game/Effect/Guard_Effect.Guard_Effect")
	};
	if (GuardEffectRef.Succeeded())
	{
		GuardEffect = GuardEffectRef.Object;
	}


	// 5_27 선환 추가 
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableRef(TEXT("/Game/Data/DT_Item.DT_Item"));

	if (ItemDataTableRef.Succeeded())
	{
		ItemData.DataTable = ItemDataTableRef.Object;
	}

}

// Called when the game starts or when spawned
void AKZCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl();

	// 리스폰 시 초기화 해야함.
	if (StatComponent)
	{
		// 5월 26일 선환 수정 (  SetupPlayerUiWidget에서 설정 하기 ) 
		//StatComponent->SetUp_stat_Hp(1000, 1000);
		//StatComponent->SetUp_stat_Stamina(100, 100);
	}



	if (APlayerController* APC = Cast<APlayerController>(GetController()))
	{
		APC->SetIgnoreMoveInput(false);
		APC->SetIgnoreLookInput(false);
	}
	
	OnLockOnStateChanged.AddDynamic(this, &AKZCharacterPlayer::UpdateMovementForLockOn);
	//OnPlayerDead.AddUObject(this, &AKZGamemode::RestartPlayer);
}


void AKZCharacterPlayer::SetupPlayerUiWidget(UPlayerUIWidget* _InPlayerUiWidget)
{
	// 설정할 플레이어의 체력 및 최대 체력

	StatComponent->SetUp_stat_Hp(1000, 1000);
	StatComponent->SetUp_stat_Stamina(100, 100);

	if (_InPlayerUiWidget)
	{
		// 초기값 초기화
		_InPlayerUiWidget->SetUp_Ui_Hp(StatComponent->GetCurrentHp(), StatComponent->GetMaxHp());
		_InPlayerUiWidget->SetUp_Ui_Stamina(StatComponent->GetCurrentStamina(), StatComponent->GetMaxStamina());

		// Ui widget의 default 값 초기화 하기.
		// Hp 관련 델리게이트 이벤트
		StatComponent->Delegate_OnHpChanged.AddUObject(_InPlayerUiWidget, &UPlayerUIWidget::UpdateHp);
		StatComponent->Delegate_OnHpChanged.AddUObject(_InPlayerUiWidget, &UPlayerUIWidget::UpdateProgressBarHp);

		// Stamina 관련 델리게이트 이벤트
		StatComponent->Delegate_OnStaminaChanged.AddUObject(_InPlayerUiWidget, &UPlayerUIWidget::UpdateProgressBarStamina);


#pragma region InterAction 관련
		UiComponent->Delegate_OnDialogRender.AddUObject(_InPlayerUiWidget, &UPlayerUIWidget::Set_DialogRenderOnOff);
		UiComponent->Delegate_OnInterActionFKeyStateChanged.AddUObject(_InPlayerUiWidget, &UPlayerUIWidget::F_KeyStateUpdate);
		UiComponent->Delegate_OnInterActionFKey_SetStateChanged.AddUObject(_InPlayerUiWidget, &UPlayerUIWidget::Set_F_KeyState);
#pragma endregion 


#pragma region InterAction Inventory 관련
		UiComponent->Delegate_InventoryOpen.AddUObject(_InPlayerUiWidget, &UPlayerUIWidget::UpdateInventoryUI);
#pragma endregion 
	}
}


// Called every frame
void AKZCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 락온 타겟이 죽은 경우 자동으로 락온이 풀림.
	if (LockOnTarget)
	{
		IKZLockOnInterface* LockOnInterface = Cast<IKZLockOnInterface>(LockOnTarget);

		if (LockOnInterface == nullptr || !LockOnInterface->CanTargetLockOn())
		{
			LockOnTarget = nullptr;
			OnLockOnStateChanged.Broadcast(false);
			return;
		}

		// 회전값 계산.
		// 락온 타겟의 위치를 기준으로 z 방향으로 + 50만큼 올림 -> 타겟의 머리 쪽을 보도록함.
		FVector TargetLoc = LockOnTarget->GetActorLocation() + FVector(0, 0, -250.0f);
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLoc);

		// 컨트롤러의 회전값 가져오기.
		FRotator CurrentControlRotation = GetControlRotation();

		FRotator TargetRotation = FMath::RInterpTo(CurrentControlRotation, LookAtRotation, DeltaTime, 10.0f);

		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			PC->SetControlRotation(TargetRotation);
		}
	}

	if (bIsSprint && GetVelocity().Size() > 0)
	{
		StatComponent->Apply_Stamina(SprintStaminaConsumptionRate * DeltaTime);

		if (StatComponent->GetCurrentStamina() <= 0)
		{
			StopSprint(FInputActionValue());
		}
	}

	/* 5_12 선환 추가 Ui 동기화 */
	StatComponent->Delegate_OnStaminaChanged.Broadcast(StatComponent->GetCurrentStamina(), StatComponent->GetMaxStamina());
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
			&AKZCharacterPlayer::Jump
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
			InventoryOpenAction,
			ETriggerEvent::Started,
			this,
			&AKZCharacterPlayer::InventoryOpen
		);

		EnhancedInputComponent->BindAction(
			Ui_Inventory,
			ETriggerEvent::Started,
			this,
			&AKZCharacterPlayer::Inventory_Close
		);

		EnhancedInputComponent->BindAction(
			Ui_InterAction,
			ETriggerEvent::Triggered,
			this,
			&AKZCharacterPlayer::InterAction
		);

		EnhancedInputComponent->BindAction(
			QuickSlot_InputAction,
			ETriggerEvent::Started,
			this,
			&AKZCharacterPlayer::QuickSlotUse
		);



		EnhancedInputComponent->BindAction(
			LockOnAction,
			ETriggerEvent::Started,
			this,
			&AKZCharacterPlayer::LockOn
		);

		EnhancedInputComponent->BindAction(
			DeadTestAction,
			ETriggerEvent::Started,
			this,
			&AKZCharacterPlayer::Dead
		);
	}

}

void AKZCharacterPlayer::SetStaminaRegenBlock(bool bBlocked)
{
	if (StatComponent)
	{
		StatComponent->bIsStaminaRegenBlocked = bBlocked;
	}
}

void AKZCharacterPlayer::ApplyStaminaTest(float value)
{
	if (StatComponent)
	{
		StatComponent->Apply_Stamina(value);
	}
}

bool AKZCharacterPlayer::HasEnoughStamina(float value)
{
	if (StatComponent)
	{
		return StatComponent->GetCurrentStamina() >= value;
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
		if (InputSystem)
		{
			InputSystem->ClearAllMappings();

			// 매핑 컨텍스트 추가 (우선순위 0)
			InputSystem->AddMappingContext(DefaultContext, 0);
		}

	}

}

void AKZCharacterPlayer::Move(const FInputActionValue& value)
{
	if (bIsDead) return;

	// 약공격 혹은 강공격 애니메이션 몽타주 진행 중에 끊고 움직이려는 경우
	// Move에서 강제로 몽타주를 끄고 움직임 활성화.
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && ((WeakAttackMontage && AnimInstance->Montage_IsPlaying(WeakAttackMontage)) || 
							 (StrongAttackMontage && AnimInstance->Montage_IsPlaying(StrongAttackMontage))))
		{
			ForceEndAttackState();
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
	if (bIsGuarding || bIsDead) return;

	if (CurrentAttackType != EAttackType::None)
	{
		if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
		{
			ForceEndAttackState();
		}
		else return;
	}

	if (StatComponent->GetCurrentStamina() > 0)
	{
		bIsSprint = true;
		StatComponent->bIsStaminaRegenBlocked = true;
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	}
}

void AKZCharacterPlayer::StopSprint(const FInputActionValue& value)
{
	bIsSprint = false;
	StatComponent->bIsStaminaRegenBlocked = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void AKZCharacterPlayer::Look(const FInputActionValue& value)
{
	if (bIsDead) return;
	// 락온 시 플레이어의 입력에 의한 카메라 이동 방지.
	if (!bIsLockOn)
	{
		// 입력값 가져오기.
		FVector2D RotationValue = value.Get<FVector2D>();
		// 회전 처리
		AddControllerYawInput(RotationValue.X * 0.7);

		// 마우스를 올리면 위로 보도록 -1을 곱함.
		AddControllerPitchInput((RotationValue.Y) * 0.5);
	}

}

// 회피
void AKZCharacterPlayer::Dodge(const FInputActionValue& value)
{
	if (bIsDead) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	if (CurrentAttackType != EAttackType::None)
	{
		if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
		{
			ForceEndAttackState();
		}
		else return;
	}

	if (StatComponent->GetCurrentStamina() < 25 || AnimInstance->Montage_IsPlaying(DodgeMontage) || bIsDodge) return;

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	

	/* Stamina 관련 테스트 코드(5_12 선환) */
	StatComponent->Apply_Stamina(25);

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
	if (bIsDead) return;

	if (GetCharacterMovement()->IsFalling() && StatComponent->GetCurrentStamina() <= 0)
	{
		StatComponent->bIsStaminaRegenBlocked = false;
		return;
	}
	StatComponent->bIsStaminaRegenBlocked = true;
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
	if (bIsDead) return;
	if (GetCharacterMovement()->IsFalling() && StatComponent->GetCurrentStamina() <= 0)
	{
		return;
	}
	StatComponent->bIsStaminaRegenBlocked = true;
	//StrongAttackBegin();
	ProcessAttackCommand(EAttackType::Strong);

}


// 5_11 선환 추가 
void AKZCharacterPlayer::InventoryOpen()
{
	/* hp 관련 테스트 코드(5_11 선환) */
	//m_pStatComponent->Apply_Damage(50);
	//
	//m_pStatComponent->Delegate_OnHpChanged.Broadcast(m_pStatComponent->GetCurrentHp());


	// 5_18 상호작용 UI 테스터 코드 
	//const float DeltaTime = GetWorld()->GetDeltaSeconds();
	//
	///*F키 상호작용 테스트 코드*/
	//switch (InterActionType)
	//{
	//case EInterActionType::None:
	//	break;
	//case EInterActionType::Dialog:
	//	break;
	//case EInterActionType::Chest:
	//	break;
	//case EInterActionType::Item:
	//	UiComponent->Delegate_OnInterActionFKeyStateChanged.Broadcast(DeltaTime);
	//	break;
	//default:
	//	break;
	//}


	// 5_20일 인벤토리 테스트
	//UiComponent->Delegate_InventoryOpen.Broadcast(InventoryComponent->Get_ItemMap());



	AKZPlayerController* pPlayerController = Cast<AKZPlayerController>(GetController());
	AIHPlayerState* pPlayerState = GetPlayerState<AIHPlayerState>();

	pPlayerController->Open_Inventory(pPlayerState->Get_InventoryComponent()->Get_ItemMap());

}

void AKZCharacterPlayer::QuickSlotUse()
{
	AIHPlayerState* pPlayerState = Cast<AIHPlayerState>(GetPlayerState());

	/* 소비할려면 현재 사용하는 아이템의 이름을 알아야함. */

	/* 그러면 플레이어 HUD-> MainUiWidget(PlayerWidget)-> Get_PlayerQuickSlotWidget 해서 가져오기*/
	AKZPlayerController* pPlayerController = Cast<AKZPlayerController>(GetController());

	AIH_HUD* pHUD = pPlayerController->Get_HUD();
	UPlayerUIWidget* pPlayerUiWidget = pHUD->Get_MainUI_Widget();

	UQuickSlotWidget* pQuickSlotWidget = pPlayerUiWidget->Get_QuickSlot();

	FName CureentItemName = pQuickSlotWidget->Get_RowName();




	TMap<FName, int32>& itemContainer = pPlayerState->Get_InventoryComponent()->Get_ItemMap();

	if (itemContainer.Find(CureentItemName) != NULL)
	{
		if (itemContainer[CureentItemName] >= 1)
		{
			FItemDataTable* RowData = ItemData.DataTable->FindRow<FItemDataTable>(
				CureentItemName,
				TEXT("Item Data Lookup")
			);

			itemContainer[CureentItemName] -= 1;
			pQuickSlotWidget->Change_Amount(itemContainer[CureentItemName]);

			int32 Heal_Hp = StatComponent->GetCurrentHp() + RowData->HealAmount;

			if (Heal_Hp >= StatComponent->GetMaxHp())
			{
				Heal_Hp = StatComponent->GetMaxHp();
			}

			StatComponent->Update_Stat_Hp(Heal_Hp);

			StatComponent->Delegate_OnHpChanged.Broadcast(Heal_Hp);


		}

	}

}

void AKZCharacterPlayer::Jump()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (GetCharacterMovement()->IsFalling() ||
		CurrentAttackType != EAttackType::None ||
		AnimInstance->Montage_IsPlaying(JumpMontage) ||
		bIsDead) return;


	if (AnimInstance && JumpMontage)
	{
		AnimInstance->Montage_Play(JumpMontage);
		AnimInstance->Montage_JumpToSection(FName("Prep"), JumpMontage);
	}

}

void AKZCharacterPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimInstance->Montage_IsPlaying(JumpMontage))
	{
		AnimInstance->Montage_JumpToSection(FName("Land"), JumpMontage);
	}

}

void AKZCharacterPlayer::ExcutePhysicsJump()
{
	Super::Jump();
}



void AKZCharacterPlayer::Guard(const FInputActionValue& value)
{
	if (GetCharacterMovement()->IsFalling() || bIsDead) return;

	if (CurrentAttackType != EAttackType::None)
	{
		if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
		{
			ForceEndAttackState();
		}
		else return;
	}

	bIsGuarding = true;
	GuardStartTime = GetWorld()->GetTimeSeconds();
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	//PlayGuardMontage();
}

void AKZCharacterPlayer::StopGuard(const FInputActionValue& value)
{
	// 가드 해제는 어떤 상황에서도 보장되어야 입력과 캐릭터 상태가 동기화됨
	bIsGuarding = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	//StopAnimMontage(GuardMontage);
}

void AKZCharacterPlayer::LockOn(const FInputActionValue& value)
{
	if (LockOnTarget || bIsDead)
	{
		LockOnTarget = nullptr;
		OnLockOnStateChanged.Broadcast(false);
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(LockOn), true, this);
	FVector MyLoc = GetActorLocation();

	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		MyLoc,
		FQuat::Identity,
		C_CHANNEL_MONSTER,
		FCollisionShape::MakeSphere(1000.0f),
		QueryParams
	);

	AActor* BestTarget = nullptr;
	float BestScore = -1.0f;

	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* Candidate = Result.GetActor();
		// 찾은 액터들 중에 중복 확인.
		if (!Candidate || Candidate == this) continue;

		// 해당 액터가 인터페이스를 가지고 있는지 확인.
		if (Candidate->GetClass()->ImplementsInterface(UKZLockOnInterface::StaticClass()))
		{
			FVector CameraLoc = Camera->GetComponentLocation();
			FVector CameraForward = Camera->GetForwardVector();
			FVector ToTarget = (Candidate->GetActorLocation() - CameraLoc).GetSafeNormal();
			
			IKZLockOnInterface* LockOnInterface = Cast<IKZLockOnInterface>(Candidate);
			if (LockOnInterface)
			{
				float CurrentDot = FVector::DotProduct(CameraForward, ToTarget);

				if (CurrentDot > 0.7f && CurrentDot > BestScore)
				{
					FHitResult ViewHit;
					FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(LockOn), true, this);
					TraceParams.AddIgnoredActor(Candidate);

					if (!GetWorld()->LineTraceSingleByChannel(ViewHit, CameraLoc, Candidate->GetActorLocation(), ECC_Visibility, TraceParams))
					{
						BestScore = CurrentDot;
						BestTarget = Candidate;
					}
				}
			}
		}
	}
	if (BestTarget)
	{
		LockOnTarget = BestTarget;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("TargetFound"));
		UE_LOG(LogTemp, Log, TEXT("Target Found: %s"), *BestTarget->GetName());
		OnLockOnStateChanged.Broadcast(true);
	}
}

// 데미지를 받은 입장.
void AKZCharacterPlayer::ProcessDamage(const FDamageData& DamageData)
{
	if (bIsDead || bIsInvincible) { return; }

	ForceEndAttackState();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// 공격하고 있는 대상을 가져옴.
	LastAttacker = DamageData.Attacker;
	// 가드 상태라면 데미지 반감.
	if (bIsGuarding && StatComponent)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		float GuardDuration = CurrentTime - GuardStartTime;
		GetCharacterMovement()->StopMovementImmediately();
		// 저스트 가드 성공 시 넉백만 있고, 패널티 X
		if (GuardDuration <= JustGuardWindow)
		{
			if (ParryEffect)
			{
				FVector SpawnLoc = GetActorLocation() + (GetActorForwardVector() * 5.0f) + FVector(0, 0, 50.0f);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					ParryEffect,
					SpawnLoc,
					GetActorRotation()
				);

				// 저스트 가드 성공 시 공격자의 그로기게이지 감소.
				// 그로기 데미지 = (몬스터) 스테미너 감소라고 가정.
				// LastAttacker -> 공격자(몬스터)
				// KZCollision.h에 있는 구조체를 만들어 데미지를 보내는 방식 재활용
				// HP데미지 = 0, 그로기 데미지만 존재
				IKZDamageInterface* DamagebleTarget = Cast<IKZDamageInterface>(LastAttacker);
				if (DamagebleTarget)
				{
					FDamageData Data;
					Data.DamageAmount = 0.0f;
					Data.GloggyDamage = DamageData.DamageAmount / 2;
					Data.Attacker = this;

					DamagebleTarget->ProcessDamage(Data);
				}
			}
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Just Guard!!"));
			LaunchCharacterNotify(500.0f);
			return;
		}
		if (GuardEffect)
		{
			FVector SpawnLoc = GetActorLocation() + (GetActorForwardVector() * 5.0f) + FVector(0, 0, 50.0f);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				GuardEffect,
				SpawnLoc,
				GetActorRotation()
			);
		}
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Guard!!"));
		StatComponent->Apply_Damage(DamageData.DamageAmount / 2);
		StatComponent->Delegate_OnHpChanged.Broadcast(StatComponent->GetCurrentHp());
		if (StatComponent->GetCurrentHp() <= 0)
		{
			bIsDead = true;
			Dead();
			OnLockOnStateChanged.Broadcast(false);
			return;
		}
		LaunchCharacterNotify(500.0f);
		return;
	}


	
	//float FinalDamage = DamageData.DamageAmount;

	// 공격하고 있는 대상의 위치와 데미지에 따라서 피격 애니메이션 재생.
	FString IntensityStr = GetIntensityString(DamageData.DamageAmount);
	FString SwingStr = GetSwingDirString();
	FString PosStr = (IntensityStr == "Strong") ? TEXT("F") : GetAttackerPosString(DamageData.Attacker);

	FName SectionName = *FString::Printf(TEXT("%s%s%s"), *IntensityStr, *SwingStr, *PosStr);

	if (StatComponent)
	{
		
		StatComponent->Apply_Damage(DamageData.DamageAmount);
		StatComponent->Delegate_OnHpChanged.Broadcast(StatComponent->GetCurrentHp());
		if (StatComponent->GetCurrentHp() <= 0)
		{
			bIsDead = true;
			Dead();
			return;
		}

		if (HitMontage /* && !AnimInstance->Montage_IsPlaying(HitMontage)*/)
		{
			GetCharacterMovement()->StopMovementImmediately();

			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				PC->SetIgnoreMoveInput(true);
			}
			//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			PlayAnimMontage(HitMontage, 1.0f, SectionName);

			
			if (AnimInstance && AnimInstance->Montage_IsPlaying(HitMontage))
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
	bIsDead = true; // 테스트 전용 나중에 지워야함.
	UE_LOG(LogTemp, Log, TEXT("Player Dead Function Called"));
	if (!bIsDead)
	{
		return;
	}

	// 입력 방지.
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreMoveInput(true);
		PC->SetIgnoreLookInput(true);
	}

	// 플레이어 물리 및 로직 정지.
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	// 충돌 처리
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	GetCapsuleComponent()->SetCollisionResponseToChannel(C_CHANNEL_MONSTER, ECR_Ignore);

	// 락온 해제.
	if (bIsLockOn)
	{
		LockOnTarget = nullptr;
		OnLockOnStateChanged.Broadcast(false);
	}

	if (OnPlayerDead.IsBound())
	{
		UE_LOG(LogTemp, Log, TEXT("Player Dead Event Broadcasted"));
		OnPlayerDead.Broadcast(this);
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

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreMoveInput(false);
	}
}

void AKZCharacterPlayer::Render_InterActionUi(EInterActionType _Tag, ESlateVisibility _eSlateVisibility)
{
	UiComponent->Delegate_OnDialogRender.Broadcast(_Tag, _eSlateVisibility);

	switch (_Tag)
	{
	case EInterActionType::Item:
		//UiComponent->
		break;

	default:
		break;
	}
}

void AKZCharacterPlayer::Ui_Key_State_Reset()
{
	UiComponent->Delegate_OnInterActionFKey_SetStateChanged.Broadcast(0.0f);
}

void AKZCharacterPlayer::Inventory_Close()
{
	AKZPlayerController* pPlayerController = Cast<AKZPlayerController>(GetController());
	AIHPlayerState* pPlayerState = GetPlayerState<AIHPlayerState>();

	pPlayerController->Open_Inventory(pPlayerState->Get_InventoryComponent()->Get_ItemMap());
}

void AKZCharacterPlayer::InterAction()
{
	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	/*F키 상호작용 테스트 코드*/
	switch (InterActionType)
	{
	case EInterActionType::None:
		break;
	case EInterActionType::Dialog:
		break;
	case EInterActionType::Chest:
		break;
	case EInterActionType::Item:
		UiComponent->Delegate_OnInterActionFKeyStateChanged.Broadcast(DeltaTime);
		break;
	default:
		break;
	}
}

void AKZCharacterPlayer::UpdateMovementForLockOn(bool bInIsLockOn)
{
	bIsLockOn = bInIsLockOn;
	if (bIsLockOn)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

bool AKZCharacterPlayer::CanTargetLockOn()
{
	return false;
}

FVector AKZCharacterPlayer::GetTargetLocation()
{
	return FVector();
}

void AKZCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	auto* PC = Cast<AKZPlayerController>(NewController);
	if (PC)
	{
		AIH_HUD* HUD = Cast<AIH_HUD>(PC->GetHUD());
		if (HUD && HUD->Get_MainUI_Widget())
		{
			SetupPlayerUiWidget(HUD->Get_MainUI_Widget());

			HUD->Get_MainUI_Widget()->SetUp_Ui_Hp(StatComponent->GetCurrentHp(), StatComponent->GetMaxHp());
			HUD->Get_MainUI_Widget()->SetUp_Ui_Stamina(StatComponent->GetCurrentStamina(), StatComponent->GetMaxStamina());
		}
	}
}
