// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/IH_HUD.h"
#include "UI/PlayerUIWidget.h"
#include "Component/InventoryComponent.h"

AIH_HUD::AIH_HUD()
{
	static ConstructorHelpers::FClassFinder<UPlayerUIWidget> PlayerWidgetRef(TEXT("/Game/UI/WBP_MainUI.WBP_MainUI_C"));

	if (PlayerWidgetRef.Succeeded())
	{
		PlayerUiWidgetClass = PlayerWidgetRef.Class;
	}

}

void AIH_HUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerUiWidget = CreateWidget<UPlayerUIWidget>(GetOwningPlayerController(), PlayerUiWidgetClass);

	// 위젯 화면에 추가 
	if (PlayerUiWidget != NULL)
	{
		PlayerUiWidget->AddToViewport();
	}

}

void AIH_HUD::OpenInventoryWidget(TMap<FName, int32>& _ItemMapContainer)
{
	PlayerUiWidget->UpdateInventoryUI(_ItemMapContainer);
}
