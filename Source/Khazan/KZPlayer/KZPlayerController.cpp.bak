// Fill out your copyright notice in the Description page of Project Settings.


#include "KZPlayerController.h"
#include "UI/PlayerUIWidget.h"

AKZPlayerController::AKZPlayerController()
{

// 5_11 선환 추가
#pragma region UI Widegt
	static ConstructorHelpers::FClassFinder<UPlayerUIWidget> PlayerWidgetRef(TEXT("/Game/UI/WBP_MainUI.WBP_MainUI_C")); 
	
	if(PlayerWidgetRef.Succeeded())
	{
		PlayerUiWidgetClass = PlayerWidgetRef.Class;
	}
#pragma endregion 
// ---------------------------------- // 

}

void AKZPlayerController::BeginPlay()
{
	// 5_11 선환 추가 

	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode; // 입력 모드 설정 ( 추후 수정 예정 ) 
	SetInputMode(GameOnlyInputMode);


	// 위젯 생성
#pragma region 위젯 생성 및 초기화 
	m_pPlayerUiWidget = CreateWidget<UPlayerUIWidget>(this, PlayerUiWidgetClass);

	// 해당 UI 화면에 렌더링 
	if (m_pPlayerUiWidget != NULL)
	{
		m_pPlayerUiWidget->AddToViewport();
	}

#pragma endregion 
	// ---------------------------------- // 

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = -90.0f;
		PlayerCameraManager->ViewPitchMax = 10.0f;

	}


}
