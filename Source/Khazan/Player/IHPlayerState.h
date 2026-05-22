// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "IHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class KHAZAN_API AIHPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AIHPlayerState();
	TObjectPtr<class UInventoryComponent> Get_InventoryComponent() { return InventoryComponent; }

protected:
	UPROPERTY(VisibleAnywhere, Category = Component)
	TObjectPtr<class UInventoryComponent> InventoryComponent;




};
