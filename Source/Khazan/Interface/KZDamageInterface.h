// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Collision/KZCollision.h"
#include "KZDamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UKZDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KHAZAN_API IKZDamageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ProcessDamage(const FDamageData& DamageData) = 0;
};
