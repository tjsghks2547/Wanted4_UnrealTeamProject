// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemTipWidget.h"
#include "Data/ItemDataTable.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UItemTipWidget::UItemTipWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	/* 아이템 데이터 관련 */
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableRef(TEXT("/Game/Data/DT_Item.DT_Item"));

	if (ItemDataTableRef.Succeeded())
	{
		ItemData.DataTable = ItemDataTableRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> SlotImageTextureGreenRef(TEXT("/Game/UI/Inventory/ItemTips/UI_YH_Common_Tips_Quality_02.UI_YH_Common_Tips_Quality_02"));

	if (SlotImageTextureGreenRef.Object != NULL)
		ArrayBackGroundTexture.Push(SlotImageTextureGreenRef.Object);


	static ConstructorHelpers::FObjectFinder<UTexture2D> SlotImageTextureOrangeRef(TEXT("/Game/UI/Inventory/ItemTips/UI_YH_Common_Tips_Quality_05.UI_YH_Common_Tips_Quality_05"));

	if (SlotImageTextureOrangeRef.Object != NULL)
		ArrayBackGroundTexture.Push(SlotImageTextureOrangeRef.Object);

}

void UItemTipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Image_Icon = Cast<UImage>(GetWidgetFromName(TEXT("Item_Icon_Image")));
	ensureAlways(Image_Icon);
	
	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Item_Description_Text")));
	ensureAlways(Text);

	BackGround_Image = Cast<UImage>(GetWidgetFromName(TEXT("BackGround_Image")));
	ensureAlways(BackGround_Image);

}

void UItemTipWidget::Update_ItemTipWidget(FName _rowName)
{
	FItemDataTable* RowData = ItemData.DataTable->FindRow<FItemDataTable>(
		_rowName,
		TEXT("Item Data Lookup")
	);

	Image_Icon->SetBrushFromTexture(RowData->ItemImage);



	// \n을 실제 줄바꿈으로 변환
	FString Description = RowData->Description.ToString();
	Description = Description.Replace(TEXT("\\n"), TEXT("\n"));
	Text->SetText(FText::FromString(Description));

	Text->SetText(RowData->Description);
	

	/* 배경 색 지정 */

	switch (RowData->ItemType)
	{
	case EItemType::NONE:
		break;
	case EItemType::Consumable:
	{
		BackGround_Image->SetBrushFromTexture(ArrayBackGroundTexture[0]);
	}
	break;
	case EItemType::Weapon:
	{
		BackGround_Image->SetBrushFromTexture(ArrayBackGroundTexture[1]);
	}
	break;
	case EItemType::Armor:
		break;
	case EItemType::Etc:
		break;
	default:
		break;
	}
}
