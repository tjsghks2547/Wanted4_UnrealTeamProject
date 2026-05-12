// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUIWidget.generated.h"

/**
 *
 */
UCLASS()
class KHAZAN_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerUIWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
public:
	// 캐릭터에서 호출함 함수.

	/* HP 관련 */
	void UpdateHp(int32 _iCurrentHp);
	void UpdateProgressBarHp(int32 _iCurrentHp);
	void SetUp_Ui_Hp(int32 _iCurrentHp, int32 _iMaxHp);
	/* ----------------- */

	/* Stamina 관련 */
	void SetUp_Ui_Stamina(float _fCurrentStamina, float _fMaxStamina);
	void UpdateProgressBarStamina(float _fCurrentStamina, float _fMaxStamina);
	/* ----------------- */



protected:
#pragma region Hp 관련 UI 
	TObjectPtr<class UPlayerHpProgressBarWidget> m_pWidgetHpProgressBar;
	TObjectPtr<class UPlayerHpProgressBarWidget_White> m_pWidgetHpWhiteProgressBar;
	TObjectPtr<class UTextBlock> m_pCurrentHpText;
	TObjectPtr<class UTextBlock> m_pMaxHpText;
#pragma endregion 

#pragma region Stamina 관련 UI
	TObjectPtr<class UPlayerStaminaProgressBarWidget> m_pWidgetStaminProgressBar;
	TObjectPtr<class UTextBlock> m_pCurrentStaminaText;
#pragma endregion 


#pragma region UI Animation 관련
	/* BindWidgetAnim (Blueprint 애니메이션과 자동 연결 )*/
	/* Transient 직렬화 제외, 런타임에만 유효한 포인터임을 명시*/
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> Render_Opacity_StaminaProgressBar;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> Render_Opactiy_StaminaText;

	UPROPERTY()
	bool m_bHasPlayedStaminaRenderOpacity = false;

#pragma endregion 

};
