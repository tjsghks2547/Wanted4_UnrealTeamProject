// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM (BlueprintType)
enum class EBossPhase : uint8
{
	Phase1A UMETA (DisplayName = "NormalForm"),
	Phase1B UMETA (DisplayName = "NormalForm_Enhance"),
	Phase2A UMETA (DisplayName = "TransForm"),
	Phase2B UMETA (DisplayName = "TransForm_Enhance")
};
