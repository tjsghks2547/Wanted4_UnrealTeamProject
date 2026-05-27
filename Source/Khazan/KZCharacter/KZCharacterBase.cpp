// Fill out your copyright notice in the Description page of Project Settings.


#include "KZCharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "../Collision/KZCollision.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

// Sets default values
AKZCharacterBase::AKZCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메쉬 로드.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(
		TEXT("/Game/Khazan/Kahzan_mesh.Kahzan_mesh")
	);
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}


	// 약공격 몽타주 로드.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> WeakAttackMontageRef(
		TEXT("/Game/Khazan_anim/Attack/AM_WeakAttack.AM_WeakAttack")
	);
	if (WeakAttackMontageRef.Succeeded())
	{
		WeakAttackMontage = WeakAttackMontageRef.Object;
	}

	// 약공격 몽타주 로드.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StrongAttackMontageRef(
		TEXT("/Game/Khazan_anim/Attack/AM_StrongAttack.AM_StrongAttack")
	);
	if (StrongAttackMontageRef.Succeeded())
	{
		StrongAttackMontage = StrongAttackMontageRef.Object;
	}

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(
		TEXT("Weapon")
	);
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));

	WeaponTrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeaponTrailComponent"));
	WeaponTrailComponent->SetupAttachment(Weapon, TEXT("Weapon_R_Trail"));
	WeaponTrailComponent->SetAutoActivate(false);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> AttackEffectRef{
	TEXT("/Game/Map/VFX/Trail/NS_Trail2.NS_Trail2")
	};
	if (AttackEffectRef.Succeeded())
	{
		AttackEffect = AttackEffectRef.Object;
		WeaponTrailComponent->SetAsset(AttackEffect);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(
		TEXT("/Game/Khazan/Weapon/Khazan_GS_Weapon.Khazan_GS_Weapon")
	);
	if (WeaponMeshRef.Succeeded())
	{
		WeaponMesh = WeaponMeshRef.Object;

		WeaponCollision->SetupAttachment(Weapon);

		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		WeaponCollision->SetCollisionObjectType(ECC_WorldDynamic);
		WeaponCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		WeaponCollision->SetCollisionResponseToChannel(C_CHANNEL_MONSTER, ECR_Overlap);

		WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AKZCharacterBase::OnWeaponOverlap);

		Weapon->SetSkeletalMesh(WeaponMesh.Get());
		Weapon->SetupAttachment(GetMesh(), TEXT("Weapon_R"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeMontageRef(
		TEXT("/Game/Khazan_anim/JumpAndDodge/AM_Dodge.AM_Dodge")
	);
	if (DodgeMontageRef.Succeeded())
	{
		DodgeMontage = DodgeMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GuardMontageRef(
		TEXT("/Game/Khazan_anim/Guard/AM_Guard.AM_Guard")
	);
	if (GuardMontageRef.Succeeded())
	{
		GuardMontage = GuardMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageRef(
		TEXT("/Game/Khazan_anim/Dead_JG_Dam/AM_Hit.AM_Hit")
	);
	if (HitMontageRef.Succeeded())
	{
		HitMontage = HitMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> JumpMontageRef(
		TEXT("/Game/Khazan_anim/JumpAndDodge/AM_Jump.AM_Jump")
	);
	if (JumpMontageRef.Succeeded())
	{
		JumpMontage = JumpMontageRef.Object;
	}

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> HitCameraShakeRef(
		TEXT("/Game/Khazan/Blueprint/CS_Hit.CS_Hit_C")
	);
}

// Called when the game starts or when spawned
void AKZCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AKZCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKZCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AKZCharacterBase::AttackCheck()
{
	if (bNextCombo && CurrentCombo < MaxCombo)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			float RequiredStamina = (NextAttackType == EAttackType::Strong) ? 30.0f : 20.0f;
			if (!HasEnoughStamina(RequiredStamina))
			{
				bNextCombo = false;
				return;
			}

			CurrentAttackType = NextAttackType;
			if (CurrentCombo < MaxCombo && NextAttackType == EAttackType::Weak)
			{
				CurrentCombo++;
				FName NextSection;
				// 차징 상태에 따라 일반약공격 혹은 차지공격으로 전환.
				if (bIsCharging)
				{
					NextSection = *FString::Printf(TEXT("ChargeWait0%d"), CurrentCombo);
				}
				else
				{
					ApplyStaminaTest(20.0f);
					NextSection = *FString::Printf(TEXT("WeakAtk0%d"), CurrentCombo);
					SetCurrentAttackDamage(NextSection);
				}
				AnimInstance->Montage_JumpToSection(NextSection, WeakAttackMontage);
				
			}
			// 다음 공격 예약이 강공격인 경우.
			else if (NextAttackType == EAttackType::Strong)
			{
				CurrentAttackType = EAttackType::Strong;
				NextAttackType = EAttackType::None;
				StrongAttackBegin();
			}
			bNextCombo = false;
			NextAttackType = EAttackType::None;

			// 콤보로 넘어갈 때 이동 모드를 다시 None으로 설정 (공격 중 이동 방지)
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		}
	}
}

// 약 or 강공격 시작 함수.
void AKZCharacterBase::ProcessAttackCommand(EAttackType AttackType)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// 피격 중이거나 회피 중일 때는 공격 불가
	if (AnimInstance->Montage_IsPlaying(HitMontage) || AnimInstance->Montage_IsPlaying(DodgeMontage) || AnimInstance->Montage_IsPlaying(JumpMontage))
	{
		return;
	}

	// 막타(3타) 이거나 차지 어택 중에는 다음 콤보 예약을 받지 않음
	bool bIsChargeAtk = false;
	if (WeakAttackMontage)
	{
		bIsChargeAtk = AnimInstance->Montage_GetCurrentSection(WeakAttackMontage).ToString().Contains(TEXT("ChargeWeak"));
	}

	if (CurrentCombo >= MaxCombo || bIsChargeAtk)
	{
		return;
	}

	float RequiredStamina = (AttackType == EAttackType::Strong) ? 30.0f : 20.0f;
	// 약공격과 강공격의 입력을 받아서 현재 입력 혹은 다음 입력의 타입을 설정.
	if (CurrentAttackType == EAttackType::None)
	{
		if (HasEnoughStamina(RequiredStamina) == false)
		{
			SetStaminaRegenBlock(false);
			return;
		}

		CurrentAttackType = AttackType;
		if (CurrentCombo == 0 && AttackType == EAttackType::Weak)
		{
			WeakAttackBegin();
			return;
		}
		else if (AttackType == EAttackType::Strong)
		{
			StrongAttackBegin();
		}
	}

	else if (CurrentAttackType == EAttackType::Strong || CurrentAttackType == EAttackType::Weak)
	{
		NextAttackType = AttackType;
		bNextCombo = true;

		// 후딜레이 구간(MOVE_Walking)에서 공격 입력 시 즉시 다음 콤보 실행
		if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
		{
			AttackCheck();
		}
	}
}

// 약공격 시작.
void AKZCharacterBase::WeakAttackBegin()
{
	CurrentCombo = 1;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(WeakAttackMontage))
	{

		// 몽타주 재생.
		AnimInstance->Montage_Play(WeakAttackMontage);
		FName JumpSection = *FString::Printf(TEXT("ChargeWait0%d"), CurrentCombo);
		AnimInstance->Montage_JumpToSection(JumpSection, WeakAttackMontage);

		// 몽타주 종료 이벤트에 등록할 델리게이트 설정.
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUObject(this, &AKZCharacterBase::AttackActionEnd);
		
		// 몽타주 재생 종료 시 발행되는 이벤트에 등록.
		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, WeakAttackMontage);

		// 공격 중 이동 불가 설정.
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}

}

// 차지어택 시작 함수.
void AKZCharacterBase::ChargeWeakAttackBegin(bool bIsCharged)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && WeakAttackMontage)
	{
		// 현재 섹션의 이름을 가져와서 Wait 혹은 Hold인 상황을 확인.
		// 약공격 몽타주는 Wait -> Hold(loop)를 하게 되어있음.
		FName CurrentSection = AnimInstance->Montage_GetCurrentSection(WeakAttackMontage);
		int32 ComboNum = CurrentCombo;

		if (CurrentSection.ToString().Contains(TEXT("Wait")) || CurrentSection.ToString().Contains(TEXT("Hold")))
		{
			FName JumpSection;
			if (bIsCharged)
			{
				ApplyStaminaTest(30.0f);
				JumpSection = *FString::Printf(TEXT("ChargeWeakAtk0%d"), ComboNum);
				SetCurrentAttackDamage(JumpSection);
			}
			else
			{
				ApplyStaminaTest(20.0f);
				JumpSection = *FString::Printf(TEXT("WeakAtk0%d"), ComboNum);
				SetCurrentAttackDamage(JumpSection);
			}
			AnimInstance->Montage_JumpToSection(JumpSection, WeakAttackMontage);
		}
	}
}

// 강공격 시작.
void AKZCharacterBase::StrongAttackBegin()
{
	CurrentAttackType = EAttackType::Strong;
	CurrentCombo = 0;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		
		// 몽타주 재생.
		ApplyStaminaTest(30.0f);
		AnimInstance->Montage_Play(StrongAttackMontage);
		SetCurrentAttackDamage(TEXT("StrongAtk"));

		// 몽타주 종료 이벤트에 등록할 델리게이트 설정.
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUObject(this, &AKZCharacterBase::AttackActionEnd);

		// 몽타주 재생 종료 시 발행되는 이벤트에 등록.
		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, StrongAttackMontage);

		// 공격 중 이동 불가 설정.
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
}

// 공격 종료 후에 실행되는 함수
void AKZCharacterBase::AttackActionEnd(UAnimMontage* TargetMontage, bool bInterrupted)
{
	// 몽타주가 인터럽트(피격 등) 되었을 때는 몽타주를 중단시키지 않고 상태만 초기화
	if (bInterrupted)
	{
		CurrentCombo = 0;
		CurrentAttackType = EAttackType::None;
		NextAttackType = EAttackType::None;
		bNextCombo = false;
		SetStaminaRegenBlock(false);
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (CurrentAttackType == EAttackType::None) return;

	// 다른 공격 몽타주가 실행 중이라면 리셋하지 않음
	if (AnimInstance && (AnimInstance->Montage_IsPlaying(WeakAttackMontage) || AnimInstance->Montage_IsPlaying(StrongAttackMontage)))
	{
		return;
	}
	
	ForceEndAttackState();
}

void AKZCharacterBase::AttackEndCheck()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	bool bIsChargeAtk = false;
	if (AnimInstance && WeakAttackMontage)
	{
		bIsChargeAtk = AnimInstance->Montage_GetCurrentSection(WeakAttackMontage).ToString().Contains(TEXT("ChargeWeak"));
	}

	// 막타(3타) 이거나 차지 어택인 경우, 혹은 강공격인 경우 즉시 상태 종료
	if (CurrentCombo >= MaxCombo || bIsChargeAtk || CurrentAttackType == EAttackType::Strong)
	{
		ForceEndAttackState();
		return;
	}

	// 이동만 허용하고 상태는 유지 (콤보 입력을 위해)
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	
	// 만약 이미 예약된 콤보가 있다면 즉시 실행
	if (bNextCombo)
	{
		AttackCheck();
	}
}

void AKZCharacterBase::ForceEndAttackState()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 모든 몽타주(nullptr)가 아닌, 공격용 몽타주만 골라서 중단 (피격 애니메이션 보호)
		if (WeakAttackMontage) AnimInstance->Montage_Stop(0.15f, WeakAttackMontage);
		if (StrongAttackMontage) AnimInstance->Montage_Stop(0.15f, StrongAttackMontage);
	}

	CurrentCombo = 0;
	CurrentAttackType = EAttackType::None;
	NextAttackType = EAttackType::None;
	bNextCombo = false;
	
	// 이동 모드 복구 (공격 중이었다면)
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	SetStaminaRegenBlock(false);
}

// 피격 시 피격 애니메이션에서 설정한 노티파이로 캐릭터가 뒤로 밀림.
void AKZCharacterBase::LaunchCharacterNotify(float LaunchForce)
{
	FVector LaunchDir;

	if (LastAttacker)
	{
		LaunchDir = GetActorLocation() - LastAttacker->GetActorLocation();
	}
	else
	{
		LaunchDir = -1 * GetActorForwardVector();
	}

	LaunchDir.Z = 0.0f;
	LaunchDir.Normalize();

	// 바닥 정보를 가져옵니다.
	FFindFloorResult FloorResult;
	GetCharacterMovement()->FindFloor(GetCapsuleComponent()->GetComponentLocation(), FloorResult, false);

	if (FloorResult.IsWalkableFloor())
	{
		// 수평 넉백 방향을 바닥의 평면(Normal)에 투영합니다.
		LaunchDir = FVector::VectorPlaneProject(LaunchDir, FloorResult.HitResult.Normal).GetSafeNormal();
	}

	float FinalForce = LaunchForce * 3.0f;
	// Z축으로 강제로 뜨는 힘을 없애고 바닥을 따라 미끄러지도록 합니다.
	LaunchCharacter(LaunchDir * FinalForce, true, false);
}

// 가드 몽타주 실행 함수.
void AKZCharacterBase::PlayGuardMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(GuardMontage);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		bIsGuarding = true;
	}
}

//  공격 했을 때만 무기의 콜리전 켜기 / 끄기
void AKZCharacterBase::EnableWeaponCollision()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (WeaponTrailComponent)
	{
		WeaponTrailComponent->Activate(true);
	}
}

void AKZCharacterBase::DisableWeaponCollision()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (WeaponTrailComponent)
	{
		WeaponTrailComponent->Deactivate();
	}
}

// 무기와 상대가 충돌했을 경우에 실행되는 함수.
void AKZCharacterBase::OnWeaponOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// 하나의 대상에게 중복 처리를 방지하기 위해 배열을 이용.
	if (AlreadyHitActor.Contains(OtherActor))
	{
		return;
	}

	// 이미 배열에 있는 대상이라면 리턴, 없으면 배열에 추가.
	AlreadyHitActor.Add(OtherActor);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("공격 성공!"));

	// 인터페이스를 이용해 무기의 데미지 데이터와 공격한 사람의 정보만 던져줌.
	// 그로기 데미지까지 추가해서 몬스터에게 데미지 전달.
	IKZDamageInterface* DamagebleTarget = Cast<IKZDamageInterface>(OtherActor);
	if (DamagebleTarget)
	{
		FDamageData Data;
		Data.DamageAmount = CurrentAttackDamage;
		Data.GloggyDamage = CurrentAttackDamage / 0.5f;
		Data.Attacker = this;

		DamagebleTarget->ProcessDamage(Data);
	}

	APlayerController* PC = Cast<APlayerController>(GetController());

	if(PC && HitCameraShakeClass)
	{
		PC->ClientStartCameraShake(HitCameraShakeClass, 1.0f);
	}

}

// 회피 몽타주 실행 함수.
void AKZCharacterBase::PlayDodgeMontage(FName Section)
{
	if (DodgeMontage && bIsDodge == false)
	{
		ForceEndAttackState();
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		bIsDodge = true;
		PlayAnimMontage(DodgeMontage, 1.0f, Section);
		// 몽타주 종료 이벤트에 등록할 델리게이트 설정.
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUObject(this, &AKZCharacterBase::DodgeMontageEnd);

		// 몽타주 재생 종료 시 발행되는 이벤트에 등록.
		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, DodgeMontage);
	}
}

void AKZCharacterBase::DodgeMontageEnd(UAnimMontage* TargetMontage, bool bInterrupted)
{
	bIsDodge = false;
}

// 회피 방향을 반환하는 함수.
FName AKZCharacterBase::DetermineDodgeSection(float Angle)
{
	if (Angle > -22.5f && Angle <= 22.5f)
	{
		return "Dodge_F";
	}
	else if (Angle > 22.5f && Angle <= 67.5f)
	{
		return "Dodge_FR";
	}
	else if (Angle > 67.5f && Angle <= 112.5f)
	{
		return "Dodge_R";
	}
	else if (Angle > 112.5f && Angle <= 157.5f)
	{
		return "Dodge_BR";
	}
	else if ((Angle > 157.5f && Angle <= 180.0f) || (Angle > -180.0f && Angle <= -157.5f))
	{
		return "Dodge_B";
	}
	else if (Angle > -157.5f && Angle <= -112.5f)
	{
		return "Dodge_BL";
	}
	else if (Angle > -112.5f && Angle <= -67.5f)
	{
		return "Dodge_L";
	}
	else if (Angle > -67.5f && Angle <= -22.5f)
	{
		return "Dodge_FL";
	}
	else
	{
		return "Dodge_B";
	}

}

void AKZCharacterBase::SetCurrentAttackDamage(FName AttackKey)
{
	if (AttackDataAsset && AttackDataAsset->AttackMap.Contains(AttackKey))
	{
		const FKZAttackDetails& Details = AttackDataAsset->AttackMap[AttackKey];
		CurrentAttackDamage = Details.DamageAmount;
		UE_LOG(LogTemp, Log, TEXT("공격 데이터 에셋에서 공격 키에 해당하는 데미지 설정: %s, 데미지: %f"), *AttackKey.ToString(), CurrentAttackDamage);
	}
	else
	{
		CurrentAttackDamage = 20.0f;

		UE_LOG(LogTemp, Warning, TEXT("공격 데이터 에셋이 없거나 공격 키가 존재하지 않습니다: %s"), *AttackKey.ToString());
	}
}


