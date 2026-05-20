// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

enum class EItemType : uint8;
/**
 * 
 */
UCLASS()
class KHAZAN_API UItemWidget :public UUserWidget
{
	GENERATED_BODY()

public:
	UItemWidget(const FObjectInitializer& ObjectInitializer);
	

protected:
	virtual void NativeConstruct() override;


public:
	//TObjectPtr<class UImage> Get_ImageUi() { return ItemImage; }
	void Slot_Update(FName _ItemKey, int32 _iAmount);


protected:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class UTexture2D>> ArrayBackGroundTexture; 

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UImage> ItemBackGroundImage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UImage> ItemImage;

	UPROPERTY()
	TObjectPtr<class UButton> Button_Item;

	UPROPERTY()
	TObjectPtr<class UImage> Image_ButtonBorder;

	UPROPERTY(EditAnywhere, Category = "Item UI")
	TObjectPtr<class UTexture2D> NormalBorderButtonTexture;

	UPROPERTY(EditAnywhere, Category = "Item UI")
	TObjectPtr<class UTexture2D> SelectedBorderButtonTexture;

	
	UPROPERTY()
	FDataTableRowHandle ItemData;

};
