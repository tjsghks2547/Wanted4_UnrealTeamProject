// Fill out your copyright notice in the Description page of Project Settings.


#include "KZMonster/KZMonsterCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Component/StatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/PlayerHpProgressBarWidget.h"

// Sets default values
AKZMonsterCharacter::AKZMonsterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	StatComponent->SetUp_stat_Hp(100, 100);



	// HP_Widget 
	Hp_Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP_Widget"));
	Hp_Widget->SetupAttachment(GetCapsuleComponent());
	Hp_Widget->SetWidgetSpace(EWidgetSpace::Screen);

	Hp_Widget->SetDrawSize(FVector2D(150.f, 10.f));
	Hp_Widget->SetRelativeLocation(FVector(0.f, 0.f, 150.f));



	// Stamina Widget
	Stamina_Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Stamina_Widget"));
	Stamina_Widget->SetupAttachment(GetCapsuleComponent());
	Stamina_Widget->SetWidgetSpace(EWidgetSpace::Screen);

	Stamina_Widget->SetDrawSize(FVector2D(150.f, 10.f));
	Stamina_Widget->SetRelativeLocation(FVector(0.f, 0.f, 140.f));



	// Hp_Widget WBP 클래스 지정
	static ConstructorHelpers::FClassFinder<UUserWidget> HP_WidgetClassRef(
		TEXT("/Game/UI/MonsterUI/WBP_MonsterHpBarDamaged.WBP_MonsterHpBarDamaged_C"));

	if (HP_WidgetClassRef.Succeeded())
	{
		Hp_Widget->SetWidgetClass(HP_WidgetClassRef.Class);
	}



	// Stamina WBP 클래스 지정
	static ConstructorHelpers::FClassFinder<UUserWidget> StaminaWidgetClass(
		TEXT("/Game/UI/MonsterUI/WBP_MonsterStaminaBar.WBP_MonsterStaminaBar_C"));

	if (StaminaWidgetClass.Succeeded())
	{
		Stamina_Widget->SetWidgetClass(StaminaWidgetClass.Class);
	}

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

	Cast<UPlayerHpProgressBarWidget>(Hp_Widget->GetUserWidgetObject())->Setup_Hp(100.f, 100.f);

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
	if (!BasicAttackMontage)return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	// 현재는 간단하게 랜덤으로 공격 애니메이션 선택.
	int32 RandomIdx = FMath::RandRange(1, 2);

	FName SectionName = FName(*FString::Printf(TEXT("Batk%d"), RandomIdx));

	AnimInstance->Montage_Play(BasicAttackMontage);

	AnimInstance->Montage_JumpToSection(SectionName, BasicAttackMontage);

	// 몽타주가 끝났을 때 람다함수 바인딩
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{			
			// 피격 시에는 IsAttacking을 false로 바꾸지 않음
			if (!bInterrupted)
			{
				if (AIC && BlackboardComp)
				{
					BlackboardComp->SetValueAsBool(FName("IsAttacking"), false);
					AIC->ClearFocus(EAIFocusPriority::Gameplay);
				}
			}
		});

	// 몽타주가 끝났을 때, 호출될 델리게이트 설정
	AnimInstance->Montage_SetEndDelegate(EndDelegate, BasicAttackMontage);

}

void AKZMonsterCharacter::PlayLongRangeAttackMontage()
{

}


void AKZMonsterCharacter::ProcessDamage(const FDamageData& DamageData)
{

	if (bIsDead) return;

	LastAttacker = DamageData.Attacker;

	FName SectionName = *FString::Printf(TEXT("Normal_UF")); // Todo: 1은 임시 하드 코딩.

	if (StatComponent)
	{
		StatComponent->Apply_Damage(DamageData.DamageAmount);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Damage"));

		/* 5_24 선환 추가*/
		Cast<UPlayerHpProgressBarWidget>(Hp_Widget->GetUserWidgetObject())->Update_MonsterHpProgressHpBar(StatComponent->GetCurrentHp());
	}

	if (StatComponent)
	{
		//m_pStatComponent->Apply_Damage(DamageData.DamageAmount);
		StatComponent->Delegate_OnHpChanged.Broadcast(StatComponent->GetCurrentHp());

		// 죽음 함수 호출
		if (StatComponent->GetCurrentHp() <= 0)
		{
			bIsDead = true;
			Dead();
			return;
		}

		// 죽지않고 피해를 받은 경우

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance) return;

		// 현재 공격 중인지 확인
		bool bIsAttacking = BlackboardComp->GetValueAsBool(FName("IsAttacking"));


		if (bIsAttacking)
		{
			if (!AdditiveHitMontage) return;

			AnimInstance->Montage_Play(AdditiveHitMontage);

			FOnMontageEnded AdditiveEndDelegate;
			AdditiveEndDelegate.BindUObject(this, &AKZMonsterCharacter::HitMontageEnd);
			AnimInstance->Montage_SetEndDelegate(AdditiveEndDelegate, AdditiveHitMontage);

		}
		else
		{
			if (!HitMontage) return;

			GetCharacterMovement()->StopMovementImmediately();
			PlayAnimMontage(HitMontage, 1.0f, SectionName);

			// 몽타주 종료 이벤트에 등록할 델리게이트 설정.
			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindUObject(this, &AKZMonsterCharacter::HitMontageEnd);
			
			// 몽타주 재생 종료 시 발행되는 이벤트에 등록.
			AnimInstance->Montage_SetEndDelegate(OnMontageEnded, HitMontage);
		}

		// 컨트롤러에게 알림
		if (OnMonsterDamaged.IsBound())
		{
			OnMonsterDamaged.Broadcast(DamageData.Attacker);
		}
	}
}

void AKZMonsterCharacter::Dead()
{
	// Todo: 이동과 행동을 중지하는 로직. 
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	if (AIC)
	{
		AIC->SetIgnoreMoveInput(true);
		AIC->SetIgnoreLookInput(true);

		BlackboardComp->SetValueAsBool(FName("IsDead"), bIsDead);

		PlayAnimMontage(DeathMontage);
		//UE_LOG(LogTemp, Display, TEXT("Death!"));
	}

}

void AKZMonsterCharacter::PlayDeathMontage()
{
	if (!DeathMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 현재는 간단하게 랜덤으로 공격 애니메이션 선택.
		//int32 RandomIdx = FMath::RandRange(1, 2);

		//FName SectionName = FName(*FString::Printf(TEXT("Batk%d"), RandomIdx));

		AnimInstance->Montage_Play(DeathMontage);

		//AnimInstance->Montage_JumpToSection(SectionName, BasicAttackMontage);

		// 몽타주가 끝났을 때 람다함수 바인딩
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
			{
				bDeathAnimationDone = true;
			});

		// 몽타주가 끝났을 때, 호출될 델리게이트 설정
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DeathMontage);
	}

	// 죽는 순산 더 이상 물리 충돌이나 AI 인식이 되지 않도록 설정.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // 상단에 추가
	if (AIC) AIC->StopMovement();
}


void AKZMonsterCharacter::HitMontageEnd(UAnimMontage* TargetMontage, bool bInterrupted)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	
	BlackboardComp->SetValueAsBool(FName("IsHit"), false);
	if (bInterrupted)
	{
		// 공격이 피격으로 끊긴 경우, 공격 상태도 false로 변경
		BlackboardComp->SetValueAsBool(FName("IsAttacking"), false);
	}
}

void AKZMonsterCharacter::AttackCheck()
{
}

void AKZMonsterCharacter::AttackEndCheck()
{
}

void AKZMonsterCharacter::LaunchCharacterNotify(float LaunchForce)
{
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None)
	{
		GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
	}

	if (LastAttacker)
	{
		FVector LaunchDir = GetActorLocation() - LastAttacker->GetActorLocation();

		LaunchDir.Z = 0.0f;
		LaunchDir.Normalize();



		LaunchCharacter((LaunchDir * LaunchForce * 3) + FVector(0.0f, 0.0f, 30.0f), true, false);
	}
	else
	{
		FVector Forward = GetActorForwardVector();
		LaunchCharacter(-1 * Forward * LaunchForce, true, false);
	}

	//GetCharacterMovement()->MovementMode = EMovementMode::MOVE_None;
}

void AKZMonsterCharacter::IsAttackEnd()
{
	BlackboardComp->SetValueAsBool(FName("IsAttacking"), false);
	// 공격중에 플레이어와 충돌을 무시했다면, 공격 종료 후 다시 충돌되도록 복구
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	//무시했던 액터(플레이어) 목록에서 제거
	if (BlackboardComp)
	{
		AActor * TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("PlayerPos")));
		if (TargetActor)
		{
		    this->MoveIgnoreActorRemove(TargetActor);
		}
		
		
		// 무브먼트 모드 복구 (혹시 공중 공격 후였다면)
		if (GetCharacterMovement()->MovementMode == MOVE_Falling)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}

	}
	
	
}

bool AKZMonsterCharacter::CanTargetLockOn()
{
	return true;
}

FVector AKZMonsterCharacter::GetTargetLocation()
{
	return GetActorLocation();
}

//void AKZMonsterCharacter::OnMonsterDamaged(AActor* Attacker)
//{
//	if (BlackboardComp)
//	{
//		BlackboardComp->SetValueAsBool(FName("IsHit"), true);
//		BlackboardComp->SetValueAsObject(FName("PlayerPos"), Attacker);
//		BlackboardComp->SetValueAsBool(FName("IsReturning"), false);
//	}
//}