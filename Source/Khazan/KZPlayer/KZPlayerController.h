// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KZPlayerController.generated.h"

// 전방 선언
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class KHAZAN_API AKZPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AKZPlayerController();

public:
	FORCEINLINE TObjectPtr<class UInputMappingContext> GetInputMappingContext() { return InputMappingContext; }

// 5_11 선환 추가 
protected:
	virtual void BeginPlay() override; //(UI 클래스 CreateWidget 하기 위해서 추가)

public:
	void Open_Inventory(TMap<FName, int32>& _ItemMapContainer);
	void Set_InputUi_IMC();
	void Set_InputGame_IMC();
	TObjectPtr<class AIH_HUD> Get_HUD() { return HUD; }

protected:
	UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputMappingContext> InputUiMappingContext;

	UPROPERTY(VisibleAnywhere, Category = HUD, BlueprintReadOnly)
	TObjectPtr<class AIH_HUD> HUD;

};
