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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameData)
	float DamageAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameData)

	AActor* Attacker;
	
	EHitIntensity Intensity;
	EAttackSwingDir SwingDir;
	//EAttackType AttackType;
	//FVector HitLocation;
};

UENUM(BlueprintType)
enum class EAttackSwingDir : uint8 {U, D, L ,R};

UENUM(BlueprintType)
enum class EHitIntensity : uint8 {Weak, Strong, Normal};