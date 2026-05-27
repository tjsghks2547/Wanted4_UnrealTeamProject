// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_BlackHole.h"
#include "KZBoss/KZBossCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "KZMonster/KZMonsterCharacter.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KZCharacter/KZCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimNotifyState_BlackHole::UAnimNotifyState_BlackHole()
{
}

// ... 기존 코드 상단 생략 ...

void UAnimNotifyState_BlackHole::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	AActor* Boss = MeshComp->GetOwner();

	if (!Boss) return;

	PullStrength = 230.0f; // 흡입 세기

	FVector BossLoc = Boss->GetActorLocation();
	
	// 범위 내 플레이어 검출
	TArray<FHitResult> OutHits;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(PullRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Boss);

	bool bHit = Boss->GetWorld()->SweepMultiByChannel(OutHits, BossLoc, BossLoc, FQuat::Identity, ECC_Pawn, Sphere, Params);

	if (!bHit) return;

	for (auto& Hit: OutHits)
	{
		ACharacter* Player = Cast<ACharacter>(Hit.GetActor());
		if (Player && Player->IsPlayerControlled())
		{
			// 방향 계산: 보스위치 - 플레이어 위치
			FVector PlayerLoc = Player->GetActorLocation();
			FVector PullDir = BossLoc - PlayerLoc;
			FVector ToBossDist = PullDir.GetSafeNormal();
			
			// 현재 속도 가져오기
			FVector CurrentVelocity = Player->GetCharacterMovement()->Velocity;

			// 흡입계산 적용.
			FVector PullForce = ToBossDist * PullStrength;

			// 현재 속도 + 흡입력
			// 플레이어가 반대로 달릴 경우 => Input Speed - Pull Force가 되어 느려짐
			// 가만히 있을 경우 => 0 + PullStrength되어 보스 방향으로 끌려감
			Player->GetCharacterMovement()->Velocity += PullForce * FrameDeltaTime * 10.0f;

			Player->GetCharacterMovement()->GroundFriction = 0.8f;
			//if (ToBossDist > StopDistance)
			//{
			//	FVector PullVelocity = PullDir * PullStrength;
			//	Player->GetCharacterMovement()->Velocity =
			//		FMath::VInterpTo(Player->GetCharacterMovement()->Velocity,
			//		PullVelocity,
			//		FrameDeltaTime,
			//		5.0f);
			//}
			
			// 대미지 구역 체크 및 초당 대미지
			float Dist = FVector::Dist(BossLoc, Player->GetActorLocation());
			if (Dist < DamageRadius)
			{
				float CurrentWorldTime = Boss->GetWorld()->GetTimeSeconds();
				float* LastDamageTimePtr = LastDamageTimeMap.Find(Player);
				float LastDamageTime = LastDamageTimePtr ? *LastDamageTimePtr : -1.0f;

				if (CurrentWorldTime - LastDamageTime >= DamageInterval)
				{
					// 대미지 데이터 생성
					FDamageData VacuumDamage;
					VacuumDamage.DamageAmount = 10.0f; // 대미지 양
					VacuumDamage.Attacker = Boss;

					// 대미지 처리
					if (AKZCharacterPlayer* TargetPlayer = Cast<AKZCharacterPlayer>(Player))
					{
						TargetPlayer->ProcessDamage(VacuumDamage);
					}

					// 마지막 대미지 시간 갱신
					LastDamageTimeMap.Add(Boss, CurrentWorldTime);
				}
			}
		}
	}
}

void UAnimNotifyState_BlackHole::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	// 노티파이 종료 시 맵을 비워주기
	LastDamageTimeMap.Empty();
}