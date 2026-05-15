// Fill out your copyright notice in the Description page of Project Settings.


#include "KZMonster/KZMonsterCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Component/StatComponent.h"

// Sets default values
AKZMonsterCharacter::AKZMonsterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	m_pStatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	m_pStatComponent->SetUp_stat_Hp(100, 100);

}

// Called when the game starts or when spawned
void AKZMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 부모 수준에서 한 번만 캐싱
	AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		BlackboardComp = AIC->GetBlackboardComponent();
	}

}

// Called every frame
void AKZMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentMovementSpeed = GetVelocity().Size();

}

// Called to bind functionality to input
void AKZMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
//
//void AKZMonsterCharacter::OnSeePawn(APawn* SeenPawn)
//{
//	// 플레이어 캐릭터인지 확인 (유효하고 플레이어 컨트롤러에 의해 제어되는지 확인)
//	if (SeenPawn && SeenPawn->IsPlayerControlled())
//	{
//		TargetPawn = SeenPawn;
//		//UE_LOG(LogTemp, Log, TEXT("Monster spotted player: %s"), *SeenPawn->GetName());
//	}
//}

void AKZMonsterCharacter::PlayAttackMontage()
{
	// 기본 공격 몽타주 재생 로직
	if (BasicAttackMontage)
	{

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 현재는 간단하게 랜덤으로 공격 애니메이션 선택.
			int32 RandomIdx = FMath::RandRange(1, 2);

			FName SectionName = FName(*FString::Printf(TEXT("Batk%d"), RandomIdx));

			AnimInstance->Montage_Play(BasicAttackMontage);

			AnimInstance->Montage_JumpToSection(SectionName, BasicAttackMontage);




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
		}
	}
}

void AKZMonsterCharacter::PlayAttackMontage_Internal(UAnimMontage* MontageToPlay, FName SectionName)
{
	if (!MontageToPlay) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageToPlay);
		if (!SectionName.IsNone())
		{
			AnimInstance->Montage_JumpToSection(SectionName, MontageToPlay);
		}

		// 공통 종료 처리 (부모가 딱 한 번만 정의)
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
			{

				if (BlackboardComp)
				{
					BlackboardComp->SetValueAsBool(FName("IsAttacking"), false);
				}
				if (AIC)
				{
					AIC->ClearFocus(EAIFocusPriority::Gameplay);
				}
			});

		AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlay);
	}
}


void AKZMonsterCharacter::ProcessDamage(const FDamageData& DamageData)
{
	if (m_pStatComponent)
	{
		m_pStatComponent->Apply_Damage(DamageData.DamageAmount);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Damage"));
	}

	// 복귀 중 피격 시 재타겟 설정.
	if (AIC && BlackboardComp)
	{

		if (BlackboardComp->GetValueAsBool(FName("IsReturning")))
		{
			BlackboardComp->SetValueAsBool(FName("IsReturning"), false); // 복귀 중단
		}

		//if (DamageData.Instigator)
		//{
		//	BlackboardComp->SetValueAsObject(FName("PlayerPos"), DamageData.Instigator);
		//	BlackboardComp->SetValueAsBool(Fname("IsReturning"), false); // 복귀 중단
			//}
	}
}