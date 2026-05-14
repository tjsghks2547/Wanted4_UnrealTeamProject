// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CalculateDistance.generated.h"

/**
 * 
 */
UCLASS()
class KHAZAN_API UBTService_CalculateDistance : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CalculateDistance();

protected:

	/** 매 프레임(또는 설정된 간격마다) 실행되는 로직 */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
