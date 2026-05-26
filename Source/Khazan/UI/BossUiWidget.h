// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossUiWidget.generated.h"

/**
 * 
 */
UCLASS()
class KHAZAN_API UBossUiWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UBossUiWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	
	void UpdateHpProgressBar(float _fCurrentHp);
	void UpdateStaminaProgressBar(float _fCurrentStamina);
	
	void Setup_Hp(int32 _iCurrentHp, int32 _iMaxHp);
	void Setup_Stamina(int32 _iCurrentStamina, int32 _iMaxStamina);

protected:
	// Hp 게이지를 보여주기 위해 사용할 프로그래스 바
	UPROPERTY()
	TObjectPtr<class UPlayerHpProgressBarWidget> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UPlayerHpProgressBarWidget_White> StaminaProgressBar;

};
