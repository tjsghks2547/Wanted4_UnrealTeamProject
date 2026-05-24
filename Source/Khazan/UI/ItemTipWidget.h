// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemTipWidget.generated.h"

/**
 * 
 */
UCLASS()
class KHAZAN_API UItemTipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UItemTipWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void Update_ItemTipWidget(FName _rowName); 

protected:
	UPROPERTY()
	FDataTableRowHandle ItemData;
	

	UPROPERTY()
	TObjectPtr<class UImage> Image_Icon;	

	UPROPERTY()
	TObjectPtr<class UTextBlock> Text;


};
