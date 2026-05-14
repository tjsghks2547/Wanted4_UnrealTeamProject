#pragma once

#include "CoreMinimal.h"
#include "KZCollision.generated.h"

// 콜리전 사용시 활용할 값을 define으로 정의.
#define CPROFILE_NOCOLLISION TEXT("NoCollision")
#define C_CHANNEL_MONSTER ECC_GameTraceChannel2
#define C_CHANNEL_PLAYER ECC_GameTraceChannel1
#define TRACE_ATTACK ECC_GameTraceChannel3


USTRUCT(BlueprintType)
struct FDamageData 
{
	GENERATED_BODY()

	float DamageAmount;
	AActor* Attacker;
	//EAttackType AttackType;
	//FVector HitLocation;
};