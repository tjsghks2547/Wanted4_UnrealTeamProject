// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IH_HUD.generated.h"

// Ui 표시 및 숨김 함수를 제공
/**
 * 
 */
UCLASS()
class KHAZAN_API AIH_HUD : public AHUD
{
	GENERATED_BODY()

public:
	AIH_HUD(); 
	
protected:
	virtual void BeginPlay() override;

public:
	void OpenInventoryWidget(TMap<FName, int32>& _ItemMapContainer);
	TObjectPtr<class UPlayerUIWidget> Get_MainUI_Widget() { return PlayerUiWidget; }


protected:
	// 클래스 정보. 클래스 정보를 바탕으로 실제 UI몸체 생성
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UPlayerUIWidget> PlayerUiWidgetClass;

	// 실제 생성된 UI 객체 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	TObjectPtr<class UPlayerUIWidget> PlayerUiWidget;


};
