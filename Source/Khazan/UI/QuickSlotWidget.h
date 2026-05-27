// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickSlotWidget.generated.h"

enum class EItemType : uint8;

/**
 * 
 */
UCLASS()
class KHAZAN_API UQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UQuickSlotWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void Set_Slot(FName _rowName, int32 _iAmount);
	void Change_Amount(int32 _iAmount);
	FName Get_RowName() { return ItemRowName; }

protected:
	UPROPERTY()
	FDataTableRowHandle ItemData;


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UImage> ItemImage;


	UPROPERTY()
	TObjectPtr<class UTextBlock> AmountTextBlock;



	UPROPERTY()
	TObjectPtr<class UTexture2D> BlankImage;

	UPROPERTY()
	FName ItemRowName;

};
