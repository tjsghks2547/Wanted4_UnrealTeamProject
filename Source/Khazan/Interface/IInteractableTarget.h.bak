// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractableTarget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIInteractableTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KHAZAN_API IIInteractableTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	virtual FName GetTargetType() const = 0;

	// AI가 이 대상을 공격 타겟으로 삼을 수 있는 지 여부
	virtual bool IsAttackable() const = 0;

public:
};
