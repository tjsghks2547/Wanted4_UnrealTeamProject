// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemContextMenuWidget.generated.h"

enum class EItemType : uint8;
/**
 * 
 */
UCLASS()
class KHAZAN_API UItemContextMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UItemContextMenuWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;


public:
	void Set_ItemType(FName _RowName, EItemType _eItemType, int32 _iAmount) 
	{ 
		ItemType = _eItemType; 
		ItemAmount = _iAmount; 
		RowName = _RowName;
	}
	
	UFUNCTION()
	void ItemEquip();

	

protected:
	UPROPERTY()
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	TObjectPtr<class UButton> Equip_Button;

	// 기본으로 넣을 머터리얼 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite , Category = UI)
	TObjectPtr<class UButton> Release_Button;

	UPROPERTY()
	int32 ItemAmount = 0; 

	UPROPERTY()
	FName RowName;
};
