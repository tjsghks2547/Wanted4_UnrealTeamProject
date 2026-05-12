// Fill out your copyright notice in the Description page of Project Settings.


#include "KZCharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(
		TEXT("/Game/Khazan/Weapon/Khazan_GS_Weapon.Khazan_GS_Weapon")
	);
	if (WeaponMeshRef.Succeeded())
	{
		WeaponMesh = WeaponMeshRef.Object;


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

}

// Called when the game starts or when spawned
void AKZCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// 나중에 락온용으로 쓸 수 있을것으로 보임.
//void AKZCharacterBase::SetCharacterContolData(const UABCharacterControlData* InCharacterControlData)
//{
//	// Pawn.
//
//	bUseControllerRotationYaw
//		= InCharacterControlData->bUseControllerRotationYaw;
//
//	// CharacterMovement.
//
//	GetCharacterMovement()->bUseControllerDesiredRotation
//		= InCharacterControlData->bUseControllerDesiredRotation;
//
//
//	GetCharacterMovement()->bOrientRotationToMovement
//		= InCharacterControlData->bOrientRotationToMovement;
//
//
//	GetCharacterMovement()->RotationRate
//		= InCharacterControlData->RotationRate;
//}

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
	if (bNextCombo)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			CurrentAttackType = NextAttackType;
			if (CurrentCombo < MaxCombo && NextAttackType == EAttackType::Weak)
			{
				CurrentCombo++;
				FName NextSection = *FString::Printf(TEXT("WeakAtk0%d"), CurrentCombo);
				AnimInstance->Montage_JumpToSection(NextSection, WeakAttackMontage);
			}
			else if (NextAttackType == EAttackType::Strong)
			{
				CurrentAttackType = EAttackType::Strong;
				NextAttackType = EAttackType::None;
				StrongAttackBegin();
			}
			bNextCombo = false;
			NextAttackType = EAttackType::None;
		}
	}
}

void AKZCharacterBase::ProcessAttackCommand(EAttackType AttackType)
{
	// 약공격과 강공격의 입력을 받아서 현재 입력 혹은 다음 입력의 타입을 설정.
	if (CurrentAttackType == EAttackType::None)
	{
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
	}

}

// 약공격 시작.
void AKZCharacterBase::WeakAttackBegin()
{
	CurrentCombo = 1;
	

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 몽타주 재생.
		AnimInstance->Montage_Play(WeakAttackMontage);

		// 몽타주 종료 이벤트에 등록할 델리게이트 설정.
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUObject(this, &AKZCharacterBase::AttackActionEnd);
		
		// 몽타주 재생 종료 시 발행되는 이벤트에 등록.
		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, WeakAttackMontage);

		// 공격 중 이동 불가 설정.
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
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
		AnimInstance->Montage_Play(StrongAttackMontage);

		// 몽타주 종료 이벤트에 등록할 델리게이트 설정.
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUObject(this, &AKZCharacterBase::AttackActionEnd);

		// 몽타주 재생 종료 시 발행되는 이벤트에 등록.
		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, StrongAttackMontage);

		// 공격 중 이동 불가 설정.
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
}

void AKZCharacterBase::AttackActionEnd(UAnimMontage* TargetMontage, bool bInterrupted)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// 몽타주 실행중이라면 리셋하지않음.
	if (!AnimInstance && AnimInstance->Montage_IsPlaying(WeakAttackMontage) || AnimInstance->Montage_IsPlaying(StrongAttackMontage))
	{
		return;
	}

	CurrentCombo = 0;
	bNextCombo = false;
	CurrentAttackType = EAttackType::None;
	NextAttackType = EAttackType::None;
	// 이동 모드 복구.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AKZCharacterBase::AttackEndCheck()
{

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	
}

// 콤보에 따라 플레이어를 앞으로 이동.
void AKZCharacterBase::LaunchCharacterNotify(float LaunchForce)
{
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None)
	{
		GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
	} 
	FVector Forward = GetActorForwardVector();
	LaunchCharacter(Forward * LaunchForce, true, false);
	//GetCharacterMovement()->MovementMode = EMovementMode::MOVE_None;
}

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

void AKZCharacterBase::PlayDodgeMontage(FName Section)
{
	if (DodgeMontage && bIsDodge == false)
	{
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
