// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/InterActionType.h"
#include "InventoryWidget.generated.h"


/**
 *
 */
UCLASS()
class KHAZAN_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;


public:
	UFUNCTION()
	void UpdateInventory(TMap<FName, int32>& _ItemMapContainer);
	void Init_Slot();
	void ShowItemTip(FName _itemRowName);

private:
	UPROPERTY(VisibleAnywhere)
	int InventorySize = 0;

	UPROPERTY(VisibleAnywhere)
	bool IsAddFailed;

	UPROPERTY(VisibleAnywhere)
	FItemData Slots;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class UItemWidget> ItemWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCanvasPanel> InventroyCanvasPanel;

	UPROPERTY(VisibleAnywhere)
	TArray<class UItemWidget*> SlotArray;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UItemTipWidget> ItemTipWidget;

};
