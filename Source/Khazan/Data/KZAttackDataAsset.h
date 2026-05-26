// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KZAttackDataAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FKZAttackDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float DamageAmount = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float StaminaCost = 10.0f;

};

UCLASS()
class KHAZAN_API UKZAttackDataAsset : public UDataAsset
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	TMap<FName, FKZAttackDetails> AttackMap;

};
