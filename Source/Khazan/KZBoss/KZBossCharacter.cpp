// Fill out your copyright notice in the Description page of Project Settings.


#include "KZBossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "../Component/StatComponent.h"

// Sets default values
AKZBossCharacter::AKZBossCharacter()
	// BackStepAttack 이동, 상승 힘의 기본값 초기화
	: Distance(500.0f), UpForce(500.0f), CurrentPhase(EBossPhase::Phase1A)
{
	// 부모 클래스(AKZMonsterCharacter)에서 PawnSensing을 생성함
	
	PrimaryActorTick.bCanEverTick = true;
	CurrentMovementSpeed = 0.0f;

	
	//m_pStatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	StatComponent->SetUp_stat_Hp(100, 100);

	// AI 회전 및 이동 설정
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	// 루트 모션 중 회전 허용
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
}

// Called when the game starts or when spawned
void AKZBossCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AKZBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재 속도를 계산하여 저장 (애니메이션 블루프린트에서 사용됨)
	//CurrentMovementSpeed = GetVelocity().Size();

}

// Called to bind functionality to input
void AKZBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AKZBossCharacter::PlayAttackMontage()
{
	// 1. 유효성 검사 (안전한 프로그래밍)
	if (!BasicAttackMontage || !BackStepAttackMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	// 2. 보스만의 고유 로직: 확률에 따른 공격 선택
	int32 RandomIdx = FMath::RandRange(1, 4);

	UAnimMontage* SelectedMontage = nullptr;
	FName SectionName = NAME_None;

	if (RandomIdx == 4)
	{
		// 백스텝 공격 선택
		SelectedMontage = BackStepAttackMontage;
		// 백스텝은 섹션 점프가 필요 없으므로 NAME_None 유지
	}
	else
	{
		// 일반 공격(1~3) 선택
		SelectedMontage = BasicAttackMontage;
		SectionName = FName(*FString::Printf(TEXT("Batk%d"), RandomIdx));
	}


	AnimInstance->Montage_Play(SelectedMontage);
	// 몽타주가 끝났을 때 람다함수 바인딩
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			if (AIC && BlackboardComp)
			{
				BlackboardComp->SetValueAsBool(FName("IsAttacking"), false);
				AIC->ClearFocus(EAIFocusPriority::Gameplay);
			}
		});

	// 몽타주가 끝났을 때, 호출될 델리게이트 설정
	AnimInstance->Montage_SetEndDelegate(EndDelegate, BasicAttackMontage);

	if (RandomIdx != 4)
	{
		AnimInstance->Montage_JumpToSection(SectionName, SelectedMontage);
	}
	//else {
		//ExecuteBackStep();
	//}

}

void AKZBossCharacter::PlayLongRangeAttackMontage()
{
	Super::PlayLongRangeAttackMontage();

	if (!LongRangeAttackMontage) return;


	//if (AIC)
	//{
	//	if (AIC->GetPathFollowingComponent())
	//	{
	//		// 이동 가이드는 현재 진행중인 이동을 중단하여 백스텝 동작이 원활하게 수행되도록 함
	//		AIC->GetPathFollowingComponent()->AbortMove(*AIC, FPathFollowingResultFlags::MovementStop);
	//	}
	//}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(LongRangeAttackMontage);
	// 몽타주가 끝났을 때 람다함수 바인딩
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			if (AIC && BlackboardComp)
			{
				BlackboardComp->SetValueAsBool(FName("IsAttacking"), false);
				AIC->ClearFocus(EAIFocusPriority::Gameplay);
			}
		});

	// 몽타주가 끝났을 때, 호출될 델리게이트 설정
	AnimInstance->Montage_SetEndDelegate(EndDelegate, LongRangeAttackMontage);
	
}

void AKZBossCharacter::ExecuteBackStep()
{
	if (AIC)
	{
		if (AIC->GetPathFollowingComponent())
		{
			// 이동 가이드는 현재 진행중인 이동을 중단하여 백스텝 동작이 원활하게 수행되도록 함
			AIC->GetPathFollowingComponent()->AbortMove(*AIC, FPathFollowingResultFlags::MovementStop);
		}
	}

	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	FVector BackDir = -GetActorForwardVector();
	FVector LaunchVelocity = 0.66* (BackDir * Distance + FVector(0, 0, UpForce));
	//UE_LOG(LogTemp, Warning, TEXT("BackStep Execute! Vector: %s"), *LaunchVelocity.ToString());
	LaunchCharacter(LaunchVelocity, true, true);
}


void AKZBossCharacter::ProcessDamage(const FDamageData& DamageData)
{
	Super::ProcessDamage(DamageData);

	// Todo: 점심 이후 진행(피격모션으로 인한 공격모션 끊김 문제: 피격쉐이크 애니메이션 애셋 설정에서 
	// // idle넣었는데 평소처럼 보스가 서있거나 전체 동작이 다보임)
	//bool bIsAttacking = BlackboardComp->GetValueAsBool(FName("IsAttacking"));
	//
	//if (bIsAttacking)
	//{
	//	if (AdditiveHitMontage)
	//	{
	//		PlayAnimMontage(AdditiveHitMontage);
	//	}
	//}
	//else
	//{
	//	if (HitMontage)
	//	{
	//		PlayAnimMontage(HitMontage);
	//	}
	//}
}

