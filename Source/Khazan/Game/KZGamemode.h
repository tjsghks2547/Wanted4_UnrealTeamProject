// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KZGamemode.generated.h"

/**
 * 
 */
UCLASS()
class KHAZAN_API AKZGamemode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	void RestartPlayer(AController* NewPlayer) override;
};
