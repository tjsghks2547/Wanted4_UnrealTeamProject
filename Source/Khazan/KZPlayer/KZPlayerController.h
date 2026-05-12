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

// 5_11 선환 추가 
protected:
	virtual void BeginPlay() override; //(UI 클래스 CreateWidget 하기 위해서 추가)


//  5_11 선환 추가 ( UI 클래스 ) 
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<class UPlayerUIWidget> PlayerUiWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	TObjectPtr<class UPlayerUIWidget> m_pPlayerUiWidget;

};
