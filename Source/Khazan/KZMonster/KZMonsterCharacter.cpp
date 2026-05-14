// Fill out your copyright notice in the Description page of Project Settings.


#include "KZMonster/KZMonsterCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AKZMonsterCharacter::AKZMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 폰 센싱 컴포넌트 생성 및 초기화
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	// 기본 시야 설정
	if (PawnSensing)
	{
		PawnSensing->SensingInterval = 0.25f; // 감지 주기
		PawnSensing->SightRadius = 50.0f;    // 감지 거리
		PawnSensing->SetPeripheralVisionAngle(90.0f); // 시야각 (절반 각도, 총 180도)
	}

}

// Called when the game starts or when spawned
void AKZMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 시야 감지 컴포넌트 초기화
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AKZMonsterCharacter::OnSeePawn);
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

void AKZMonsterCharacter::OnSeePawn(APawn* SeenPawn)
{
	// 플레이어 캐릭터인지 확인 (유효하고 플레이어 컨트롤러에 의해 제어되는지 확인)
	if (SeenPawn && SeenPawn->IsPlayerControlled())
	{
		TargetPawn = SeenPawn;
		//UE_LOG(LogTemp, Log, TEXT("Monster spotted player: %s"), *SeenPawn->GetName());
	}
}

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

					AAIController* AIC = Cast<AAIController>(GetController());

					// 
					if (AIC && AIC->GetBlackboardComponent())
					{
						AIC->GetBlackboardComponent()->SetValueAsBool(FName("isAttacking"), false);
					}

				});

			// 몽타주가 끝났을 때, 호출될 델리게이트 설정
			AnimInstance->Montage_SetEndDelegate(EndDelegate, BasicAttackMontage);
		}
	}
}