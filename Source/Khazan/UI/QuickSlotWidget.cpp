// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuickSlotWidget.h"
#include "Data/ItemDataTable.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


UQuickSlotWidget::UQuickSlotWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	/* 아이템 데이터 관련 */
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableRef(TEXT("/Game/Data/DT_Item.DT_Item"));

	if (ItemDataTableRef.Succeeded())
	{
		ItemData.DataTable = ItemDataTableRef.Object;

	}



}

void UQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct(); 


	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("Item_Image")));
	ensureAlways(ItemImage);

	
	AmountTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Item_Amount_Text")));
	ensureAlways(AmountTextBlock);

}

void UQuickSlotWidget::Set_Slot(FName _rowName, int32 _iAmount)
{

	FItemDataTable* RowData = ItemData.DataTable->FindRow<FItemDataTable>(
		_rowName,
		TEXT("Item Data Lookup")
	);

	ItemImage->SetBrushFromTexture(RowData->ItemImage);
	AmountTextBlock->SetText(FText::AsNumber(_iAmount));

}
