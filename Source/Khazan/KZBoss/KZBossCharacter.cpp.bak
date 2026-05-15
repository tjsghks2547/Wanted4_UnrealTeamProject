// Fill out your copyright notice in the Description page of Project Settings.


#include "KZBossCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

#include "../Component/StatComponent.h"

// Sets default values
AKZBossCharacter::AKZBossCharacter()
	// BackStepAttack 이동, 현재 페이즈 관련 변수 초기화
	: Distance(500.0f), UpForce(500.0f), CurrentPhase(EBossPhase::Phase1A)
{
	// 부모 클래스(AKZMonsterCharacter)에서 PawnSensing이 생성됨
	
	PrimaryActorTick.bCanEverTick = true;
	CurrentMovementSpeed = 0.0f;

	
	m_pStatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	m_pStatComponent->SetUp_stat_Hp(100, 100);

	// AI 회전 및 이동 설정
	bUseControllerRotationYaw = false;
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

	// 현재 속도를 계산하여 저장 (애니메이션 블루프린트에서 참조됨)
	CurrentMovementSpeed = GetVelocity().Size();

}

// Called to bind functionality to input
void AKZBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AKZBossCharacter::PlayAttackMontage()
{
	Super::PlayAttackMontage();

	if (BasicAttackMontage && BackStepAttackMontage)
	{
		
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 현재는 간단하게 랜덤으로 공격 애니메이션 선택.
			int32 RandomIdx = FMath::RandRange(1, 4);

			FName SectionName = FName(*FString::Printf(TEXT("Batk%d"), RandomIdx));

			UAnimMontage* MontageToPlay = (RandomIdx == 4) ? BackStepAttackMontage : BasicAttackMontage;
	
			AnimInstance->Montage_Play(MontageToPlay);

			if (RandomIdx != 4)
			{
				AnimInstance->Montage_JumpToSection(SectionName, MontageToPlay);
			}



			// 몽타주가 끝났을 때 실행될 함수(람다) 연결
			FOnMontageEnded EndDelegate;
			EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
				{
	
					AAIController* AIC = Cast<AAIController>(GetController());

					// 공격이 끝났을 때 BB의 isAttacking 값을 false로 설정하여 공격 상태 종료
					if (AIC && AIC->GetBlackboardComponent())
					{
						AIC->GetBlackboardComponent()->SetValueAsBool(FName("isAttacking"), false);
					}

				});

			// 몽타주가 끝났을 때, 호출될 델리게이트 설정
			AnimInstance->Montage_SetEndDelegate(EndDelegate,MontageToPlay);
		}
	}
}

void AKZBossCharacter::ExecuteBackStep()
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		if (AIC->GetPathFollowingComponent())
		{
			// 이동 중이던 경로 추적을 중단하여 백스텝 공격이 원활하게 실행되도록 함
			AIC->GetPathFollowingComponent()->AbortMove(*AIC, FPathFollowingResultFlags::MovementStop);
		}
	}

	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	FVector BackDir = -GetActorForwardVector();
	FVector LaunchVelocity = 0.66 * (BackDir * Distance + FVector(0, 0, UpForce));
	UE_LOG(LogTemp, Warning, TEXT("BackStep Execute! Vector: %s"), *LaunchVelocity.ToString());
	LaunchCharacter(LaunchVelocity, true, true);
}
// 인터페이스 구현한 부분 - 현석
// 데미지를 받았을 때 보스 몬스터의 방어력같은 것을 고려한다면, 여기서 데미지 계산을 해주면 됨.
void AKZBossCharacter::ProcessDamage(const FDamageData& DamageData)
{
	if (m_pStatComponent)
	{
		m_pStatComponent->Apply_Damage(DamageData.DamageAmount);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Damage"));
	}

}

