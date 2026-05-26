#pragma once

#include "CoreMinimal.h"
#include "KZCollision.generated.h"

// 콜리전 사용시 활용할 값을 define으로 정의.
#define CPROFILE_NOCOLLISION TEXT("NoCollision")
#define C_CHANNEL_MONSTER ECC_GameTraceChannel2
#define C_CHANNEL_PLAYER ECC_GameTraceChannel1
#define TRACE_ATTACK ECC_GameTraceChannel3

UENUM(BlueprintType)
enum class EAttackSwingDir : uint8 { U, D, L, R, None };

UENUM(BlueprintType)
enum class EHitIntensity : uint8 { Weak, Strong, Normal, None };

USTRUCT(BlueprintType)
struct FDamageData 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameData)
	float DamageAmount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameData)

	AActor* Attacker = nullptr;
	
	EHitIntensity Intensity = EHitIntensity::None;
	EAttackSwingDir SwingDir = EAttackSwingDir::None;
	//EAttackType AttackType;
	//FVector HitLocation;
};

