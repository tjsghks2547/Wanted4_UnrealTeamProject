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
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	//TObjectPtr<class UImage> Get_ImageUi() { return ItemImage; }
	void Slot_Update(FName _ItemKey, int32 _iAmount);
	void Set_ParentInventory(class UInventoryWidget* pParentInventoryClass) { ParentInventory = pParentInventoryClass; }

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class UTexture2D>> ArrayBackGroundTexture;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UImage> ItemBackGroundImage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UImage> ItemImage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UButton> ItemClickButton;

	UPROPERTY()
	TObjectPtr<class UImage> Image_ButtonBorder;

	UPROPERTY(EditAnywhere, Category = "Item UI")
	TObjectPtr<class UTexture2D> NormalBorderButtonTexture;

	UPROPERTY(EditAnywhere, Category = "Item UI")
	TObjectPtr<class UTexture2D> SelectedBorderButtonTexture;

	UPROPERTY()
	FDataTableRowHandle ItemData;

	UPROPERTY()
	TObjectPtr<class UTextBlock> AmountTextBlock;

	UPROPERTY()
	TObjectPtr<class UInventoryWidget> ParentInventory;

	UPROPERTY()
	FName ItemName;

protected:
	// 바인딩할 함수 선언
	UFUNCTION()
	void OnItemImageClicked();
};
