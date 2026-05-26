// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossUiWidget.h"
#include "UI/PlayerHpProgressBarWidget.h"
#include "UI/PlayerHpProgressBarWidget_White.h"

UBossUiWidget::UBossUiWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UBossUiWidget::NativeConstruct()
{
	Super::NativeConstruct(); 

	HpProgressBar = Cast<UPlayerHpProgressBarWidget>(GetWidgetFromName(TEXT("WBP_Hp_Bar")));
	
	StaminaProgressBar = Cast<UPlayerHpProgressBarWidget_White>(GetWidgetFromName(TEXT("WBP_Stamina_Bar")));




}

void UBossUiWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

void UBossUiWidget::Setup_Hp(int32 _iCurrentHp, int32 _iMaxHp)
{
	HpProgressBar->Setup_Hp(_iCurrentHp, _iMaxHp);

}

void UBossUiWidget::Setup_Stamina(int32 _iCurrentStamina, int32 _iMaxStamina)
{
	StaminaProgressBar->Setup_HpWhiteProgressBar(_iCurrentStamina, _iMaxStamina);
}



void UBossUiWidget::UpdateHpProgressBar(float _iCurrentHp)
{
	HpProgressBar->UpdateHpProgressBar(_iCurrentHp);
}

void UBossUiWidget::UpdateStaminaProgressBar(float _fCurrentStamina)
{
	StaminaProgressBar->Update_HpProgressHpBarWhite(_fCurrentStamina);
}


