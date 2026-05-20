// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Data/ItemDataTable.h"

UItemWidget::UItemWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> NormalBorderTextureRef(TEXT("/Game/UI/Inventory/YH_UI_tishikuang_else02.YH_UI_tishikuang_else02"));
	if(NormalBorderTextureRef.Object != NULL)
	{
		NormalBorderButtonTexture = NormalBorderTextureRef.Object; 
	}


	static ConstructorHelpers::FObjectFinder<UTexture2D> SelectedBorderTextureRef(TEXT("/Game/UI/Inventory/YH_UI_tishikuang_else.YH_UI_tishikuang_else"));
	if (SelectedBorderTextureRef.Object != NULL)
	{
		SelectedBorderButtonTexture = SelectedBorderTextureRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UTexture2D> SlotImageTextureGreenRef(TEXT("/Game/UI/Inventory/UI_ICONbg_green.UI_ICONbg_green"));
	
	if(SlotImageTextureGreenRef.Object != NULL)
		ArrayBackGroundTexture.Push(SlotImageTextureGreenRef.Object);
	

	static ConstructorHelpers::FObjectFinder<UTexture2D> SlotImageTextureOrangeRef(TEXT("/Game/UI/Inventory/UI_ICONbg_orange.UI_ICONbg_orange"));

	if (SlotImageTextureOrangeRef.Object != NULL)
		ArrayBackGroundTexture.Push(SlotImageTextureOrangeRef.Object);



	/* 아이템 데이터 관련 */
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableRef(TEXT("/Game/Data/DT_Item.DT_Item"));

	if (ItemDataTableRef.Succeeded())
	{
		ItemData.DataTable = ItemDataTableRef.Object;
	}
}

void UItemWidget::NativeConstruct()
{
	Super::NativeConstruct(); 

	Button_Item = Cast<UButton>(GetWidgetFromName(TEXT("Slot_Button"))); 
	ensureAlways(Button_Item);


	ItemBackGroundImage = Cast<UImage>(GetWidgetFromName(TEXT("Slot_Image")));
	ensureAlways(ItemBackGroundImage);

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("Item_Image")));
	ensureAlways(ItemImage);
}

void UItemWidget::Slot_Update(FName _ItemKey, int32 _iAmount)
{
	
	FItemDataTable* RowData = ItemData.DataTable->FindRow<FItemDataTable>(
		_ItemKey,
		TEXT("Item Data Lookup")
	);

	switch (RowData->ItemType)
	{
	case EItemType::Consumable:
	{
		ItemBackGroundImage->SetBrushFromTexture(ArrayBackGroundTexture[0]);
	}
		break;
	case EItemType::Weapon:
		ItemBackGroundImage->SetBrushFromTexture(ArrayBackGroundTexture[1]);
		break;
	case EItemType::Armor:
		break;
	case EItemType::Etc:
		break;
	default:
		break;
	}

	ItemImage->SetBrushFromTexture(RowData->ItemImage);

}


