// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStaminaProgressBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class KHAZAN_API UPlayerStaminaProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerStaminaProgressBarWidget(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


public:
	void UpdateProgressBar(float _fCurrentStamina, float _fMaxStamina); 
	FORCEINLINE void Set_Up_Stamina(float _fCurrentStamina, float _fMaxStamina)
	{
		m_fCurrentStamina = _fCurrentStamina; 
		m_fMaxStamina = _fMaxStamina; 
	};



private:
	TObjectPtr<class UProgressBar> m_pStaminaProgressBar;

	
	UPROPERTY()
	float m_fCurrentStamina;
	UPROPERTY()
	float m_fMaxStamina;



};
