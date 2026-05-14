// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStaminaProgressBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UPlayerStaminaProgressBarWidget::UPlayerStaminaProgressBarWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UPlayerStaminaProgressBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pStaminaProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Stamina")));

	if(m_pStaminaProgressBar == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("StaminaProgressBar를 찾지 못했습니다."));
	}

}

void UPlayerStaminaProgressBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	
}

void UPlayerStaminaProgressBarWidget::UpdateProgressBar(float _fCurrentStamina, float _fMaxStamina)	
{

	m_fCurrentStamina = _fCurrentStamina;
	m_fMaxStamina = _fMaxStamina;
	
	
	m_pStaminaProgressBar->SetPercent(m_fCurrentStamina / m_fMaxStamina);

}
