// Fill out your copyright notice in the Description page of Project Settings.


#include "KZBossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "../Component/StatComponent.h"
#include "Components/CapsuleComponent.h"
#pragma region 선환 추가
#include "Components/WidgetComponent.h"
#include "KZPlayer/KZPlayerController.h"
#include "HUD/IH_HUD.h"
#include "UI/PlayerUIWidget.h"
#include "UI/BossUiWidget.h"
#pragma endregion 

// Sets default values
AKZBossCharacter::AKZBossCharacter()
	// BackStepAttack 이동, 상승 힘의 기본값 초기화
	: Distance(500.0f), UpForce(500.0f), CurrentPhase(EBossPhase::Phase1A)
{
	// 부모 클래스(AKZMonsterCharacter)에서 PawnSensing을 생성함
	
	PrimaryActorTick.bCanEverTick = true;
	CurrentMovementSpeed = 0.0f;

	
	//m_pStatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	StatComponent->SetUp_stat_Hp(450, 450);
	StatComponent->SetUp_stat_Stamina(250, 250);

	// AI 회전 및 이동 설정
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	// 루트 모션 중 회전 허용
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;

	Name = TEXT("Boss");
}

// Called when the game starts or when spawned
void AKZBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	/* 5_26 선환 추가 */
	Hp_Widget->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	Stamina_Widget->GetWidget()->SetVisibility(ESlateVisibility::Hidden);

	/* 5_26 선환 추가 */
	GetWorldTimerManager().SetTimerForNextTick([this]()
		{
			AKZPlayerController* pKZPlayerController = Cast<AKZPlayerController>(GetWorld()->GetFirstPlayerController());

			AIH_HUD* pIH_HUD = pKZPlayerController->Get_HUD();

			pIH_HUD->Get_MainUI_Widget()->Set_Up_BossUi(
				StatComponent->GetCurrentHp(), StatComponent->GetMaxHp(), StatComponent->GetCurrentStamina(), StatComponent->GetMaxStamina());
		});

}

// Called every frame
void AKZBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재 속도를 계산하여 저장 (애니메이션 블루프린트에서 사용됨)
	//CurrentMovementSpeed = GetVelocity().Size();

	if (bIsJumpingToPlayer)
	{
		AActor * Target = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("PlayerPos")));
		if (Target)
		{
			FVector BossLoc = GetActorLocation();
			FVector PlayerLoc = Target->GetActorLocation();

			// Z축 제외한 거리 측정
			float Distance2D = FVector::Dist2D(FVector(BossLoc.X, BossLoc.Y, 0), FVector(PlayerLoc.X,PlayerLoc.Y, 0));
			float VerticalVel = GetCharacterMovement()->Velocity.Z;
			// 보스가 내려가기 시작했고, 플레이어 머리 위에 도달했을 때

			// 방향 벡터
			FVector DirToPlayer = (PlayerLoc - BossLoc).GetSafeNormal();
			DirToPlayer.Z = 0;
	
			// 현재 속도 크기 유지하며 방향만 수정
			float Speed = 1500.0f; // 초기 점프 속도에 맞게 조절
			GetCharacterMovement()->Velocity.X = DirToPlayer.X * Speed;
			GetCharacterMovement()->Velocity.Y = DirToPlayer.Y * Speed;
		}
	}
}

// Called to bind functionality to input
void AKZBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AKZBossCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bIsJumpingToPlayer = false;

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->GravityScale = 1.0f;

	if (AIC)
	{
		AIC->ClearFocus(EAIFocusPriority::Gameplay);
	}

}
void AKZBossCharacter::PlayAttackMontage()
{
	// 1. 유효성 검사 (안전한 프로그래밍)
	if (!BasicAttackMontage || !BackStepAttackMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	int32 RandomIdx = 1;
	
	// 2. 보스만의 고유 로직: 확률에 따른 공격 선택
	if (CurrentPhase == EBossPhase::Phase1A)
	{
		RandomIdx = FMath::RandRange(1, 4);
	}
	else
	{
		RandomIdx = FMath::RandRange(1, 5);
	}

	UAnimMontage* SelectedMontage = nullptr;
	FName SectionName = NAME_None;

	if (RandomIdx == 4)
	{
		// 백스텝 공격 선택
		SelectedMontage = BackStepAttackMontage;
		// 백스텝은 섹션 점프가 필요 없으므로 NAME_None 유지
	}
	else if (RandomIdx == 5)
	{
		// 점프 공격 선택
		SelectedMontage = JumpAttackMontage;
	}
	else
	{
		// 일반 공격(1~3) 선택
		SelectedMontage = BasicAttackMontage;
		SectionName = FName(*FString::Printf(TEXT("Batk%d"), RandomIdx));
	}


	AnimInstance->Montage_Play(SelectedMontage);
	AnimInstance->Montage_JumpToSection(SectionName, SelectedMontage);

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
	AnimInstance->Montage_SetEndDelegate(EndDelegate, SelectedMontage);

}

void AKZBossCharacter::PlayJumpAttackMontage()
{
	if (!JumpAttackMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(JumpAttackMontage);
	// 몽타주가 끝났을 때 람다함수 바인딩
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			if (AIC && BlackboardComp)
			{
				BlackboardComp->SetValueAsBool(FName("IsAttacking"), false);
			}
		});

	// 몽타주가 끝났을 때, 호출될 델리게이트 설정
	AnimInstance->Montage_SetEndDelegate(EndDelegate, JumpAttackMontage);
}

void AKZBossCharacter::PlayLongRangeAttackMontage()
{
	Super::PlayLongRangeAttackMontage();

	if (!LongRangeAttackMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(LongRangeAttackMontage);
	// 몽타주가 끝났을 때 람다함수 바인딩
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			if (AIC && BlackboardComp)
			{
				BlackboardComp->SetValueAsBool(FName("IsAttacking"), false);
				//AIC->ClearFocus(EAIFocusPriority::Gameplay);
			}
		});

	// 몽타주가 끝났을 때, 호출될 델리게이트 설정
	AnimInstance->Montage_SetEndDelegate(EndDelegate, LongRangeAttackMontage);
	
}

void AKZBossCharacter::PlayDashAttackMontage()
{
	if (!ThrowAndDashAttackMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(ThrowAndDashAttackMontage);
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
	LaunchCharacter(LaunchVelocity, true, true);
}

void AKZBossCharacter::ExecuteDash()
{
	if (!AIC || !BlackboardComp) return;

	if (AIC->GetPathFollowingComponent())
	{
		AIC->GetPathFollowingComponent()->AbortMove(*AIC, FPathFollowingResultFlags::MovementStop);
	}
		
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("PlayerPos")));
	if (!TargetActor) return;

	this->MoveIgnoreActorAdd(TargetActor); // 물리적 플레이어 무시(관통 핵심)

	FVector BossLoc = GetActorLocation();
	FVector TargetLoc = TargetActor->GetActorLocation();
	float DistanceToPlayer = FVector::Dist2D(BossLoc, TargetLoc);
	FVector DashDir = (TargetLoc - BossLoc);
	DashDir.Z = 0.0f;// 수평 이동을 위한 Z축 제거
	DashDir.Normalize();

	// 몽타주 시간 하드코딩: 원래는 데이터 애셋이나 변수로 관리하는 게 좋음
	float DashAnimDuration = 0.92f;
	float RequiredSpeed = DistanceToPlayer / DashAnimDuration; // 거 = 속 * 시 이용
	
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);

	FVector LaunchVelocity = DashDir * RequiredSpeed + FVector(0, 0, DashUpForce);

	// 플레이어와 충돌 무시
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	LaunchCharacter(LaunchVelocity, true, true);
	
	// 플레이어를 바라보도록
	SetActorRotation(DashDir.Rotation());
}

void AKZBossCharacter::ExecuteJump()
{
	if (!AIC || !BlackboardComp) return;
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("PlayerPos")));
	if (!TargetActor) return;


	FVector StartLoc = GetActorLocation();
	FVector PlayerLoc = TargetActor->GetActorLocation();

	// 1. 값 설정
	float TimeToArrive = 2.0f; // 목표까지 걸리는 시간 (작을수록 빠르고 강력)
	float JumpBoost = 2.0f;
    float Gravity = GetWorld()->GetGravityZ() * -1.0f * JumpBoost; // 중력값 (보통 980)

	// 2. 가로 이동 벡터 계산
	FVector LaunchVelXY = (PlayerLoc - StartLoc);
	LaunchVelXY.Z = 0; // 수평 거리만 계산
	float HegihtDiff = PlayerLoc.Z - StartLoc.Z;
	LaunchVelXY /= TimeToArrive;

	// 3. 세로 발사 속도 계산 (포물선 공식)
	float HeightDiff = PlayerLoc.Z - StartLoc.Z;
	float LaunchVelZ = (HeightDiff + (0.5f * Gravity * FMath::Square(TimeToArrive))) / TimeToArrive;
	
	// 4. 캐릭터 발사
	FVector FinalLaunchVelocity = LaunchVelXY + FVector(0, 0, LaunchVelZ);

	GetCharacterMovement()->GravityScale = JumpBoost;

	// 점프 전 무브먼트 모드를 Falling으로 변경하여 마찰력 무시
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);

	// 플레이어와 충돌 무시
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	LaunchCharacter(FinalLaunchVelocity, true, true);

	bIsJumpingToPlayer = true;
}

void AKZBossCharacter::ChangePhase()
{
	//  Phase1A UMETA(DisplayName = "NormalForm"),
	//	Phase1B UMETA(DisplayName = "NormalForm_Enhance"),
	//	Phase2A UMETA(DisplayName = "TransForm"),
	//	Phase2B UMETA(DisplayName = "TransForm_Enhance")
	//UE_LOG(LogTemp, Warning, TEXT("ChangePhase Execute!"));

	switch (CurrentPhase) {
	case EBossPhase::Phase1A:
		CurrentPhase = EBossPhase::Phase1B;
		//UE_LOG(LogTemp, Warning, TEXT("Phase1B"));
		break;

	case EBossPhase::Phase1B:
		CurrentPhase = EBossPhase::Phase2A;
		//UE_LOG(LogTemp, Warning, TEXT("Phase2A"));
		break;

	case EBossPhase::Phase2A:
		CurrentPhase = EBossPhase::Phase2B;
		//UE_LOG(LogTemp, Warning, TEXT("Phase2B"));
		break;

	default:
		break;
	}

	// 블랙보드 페이즈 업데이트 
	BlackboardComp->SetValueAsEnum(FName("CurrentPhase"), static_cast<uint8>(CurrentPhase));
	//UE_LOG(LogTemp, Warning, TEXT("%d"), (int8)CurrentPhase);
}

void AKZBossCharacter::PlayPhaseChangingAttackMontage()
{
	if (!PhaseChangingAttackMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(PhaseChangingAttackMontage);

	// 몽타주가 끝났을 때 람다함수 바인딩
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			if (AIC && BlackboardComp)
			{
				this->ChangePhase();// CurrentPhase 업데이트 및 BT CurrentPhase도 업데이트
				BlackboardComp->SetValueAsBool(FName("IsAttacking"), false);
				BlackboardComp->SetValueAsBool(FName("IsPhaseChanging"), false);
				AIC->ClearFocus(EAIFocusPriority::Gameplay);
			}
		});

	// 몽타주가 끝났을 때, 호출될 델리게이트 설정
	AnimInstance->Montage_SetEndDelegate(EndDelegate, PhaseChangingAttackMontage);
}

void AKZBossCharacter::ProcessDamage(const FDamageData& DamageData)
{
	Super::ProcessDamage(DamageData);

	
	bool bIsChanging = BlackboardComp->GetValueAsBool(FName("IsPhaseChanging"));
	if (bIsChanging == false && CurrentPhase == EBossPhase::Phase1A)
	{
		//(CurrentHP / MaxHP <= 0.6f)
		// 페이즈 1A에서만 체력 60% 이하로 떨어지면 페이즈 전환 시도

		// 둘 중 하나만 float로 형변환해도 나눗셈 전체가 float 연산으로 수행
		if(static_cast<float>(StatComponent->GetCurrentHp()) / StatComponent->GetMaxHp() <= 0.6f)
		{
			BlackboardComp->SetValueAsBool(FName("IsPhaseChanging"), true);
			//this->PlayPhaseChangingAttackMontage();
			 // 애니메이션이 끝나면 ChangePhase()가 호출되어 페이즈 전환 및 블랙보드 업데이트가 이루어짐
		}
	}
}

