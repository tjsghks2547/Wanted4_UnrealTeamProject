// Fill out your copyright notice in the Description page of Project Settings.


#include "KZPlayerController.h"
#include "UI/PlayerUIWidget.h"
#include "InputMappingContext.h"
#include "HUD/IH_HUD.h"
#include "EnhancedInputSubsystems.h"

AKZPlayerController::AKZPlayerController()
{

#pragma region IMC Mapping Context
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/Khazan/Input/IMC_Khazan.IMC_Khazan"));

	if (InputMappingContextRef.Object != NULL)
	{
		InputMappingContext = InputMappingContextRef.Object;
	}
#pragma endregion 
// ---------------------------------- // 



#pragma region UI IMC Mapping Context
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputUIMappingContextRef(TEXT("/Game/Khazan/Input/IMC_UI.IMC_UI"));

	if (InputUIMappingContextRef.Object != NULL)
	{
		InputUiMappingContext = InputUIMappingContextRef.Object;
	}
#pragma endregion 

}

void AKZPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 입력 모드 설정.
	// 게임 시작되면 뷰포트로 바로 입력 되도록.
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);




#pragma region HUD 생성

	HUD = GetHUD<AIH_HUD>();

#pragma endregion 
	// ---------------------------------- // 

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = -90.0f;
		PlayerCameraManager->ViewPitchMax = 20.0f;

	}


}


void AKZPlayerController::Open_Inventory(TMap<FName, int32>& _ItemMapContainer)
{
	HUD->OpenInventoryWidget(_ItemMapContainer);
}

void AKZPlayerController::Set_InputUi_IMC()
{

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	// 게임 → UI
	Subsystem->RemoveMappingContext(InputMappingContext);
	Subsystem->AddMappingContext(InputUiMappingContext, 0);

	FInputModeGameAndUI GameAndUIInputMode;
	SetInputMode(GameAndUIInputMode);
}

void AKZPlayerController::Set_InputGame_IMC()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());


	// UI → 게임
	Subsystem->RemoveMappingContext(InputUiMappingContext);
	Subsystem->AddMappingContext(InputMappingContext, 0);

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
