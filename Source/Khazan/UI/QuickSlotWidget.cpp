// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuickSlotWidget.h"
#include "Data/ItemDataTable.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Player/IHPlayerState.h"
#include "Component/InventoryComponent.h"


UQuickSlotWidget::UQuickSlotWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	/* 아이템 데이터 관련 */
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableRef(TEXT("/Game/Data/DT_Item.DT_Item"));

	if (ItemDataTableRef.Succeeded())
	{
		ItemData.DataTable = ItemDataTableRef.Object;

	}


	static ConstructorHelpers::FObjectFinder<UTexture2D> Texture2DRef(TEXT("/Game/UI/Inventory/ItemTexture/BlankImage.BlankImage"));

	if (Texture2DRef.Succeeded())
	{
		BlankImage = Texture2DRef.Object;
	}

}

void UQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct(); 


	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("Item_Image")));
	ensureAlways(ItemImage);

	
	AmountTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Item_Amount_Text")));
	ensureAlways(AmountTextBlock);

	ItemRowName = NAME_None;
}

void UQuickSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	AIHPlayerState* pPlayerState = Cast<AIHPlayerState>(GetOwningPlayerState());

	TMap<FName, int32>& pIventory = pPlayerState->Get_InventoryComponent()->Get_ItemMap();


	if (ItemRowName != NAME_None)
	{
		AmountTextBlock->SetText(FText::AsNumber(pIventory[ItemRowName]));
	}


}

void UQuickSlotWidget::Set_Slot(FName _rowName, int32 _iAmount)
{

	FItemDataTable* RowData = ItemData.DataTable->FindRow<FItemDataTable>(
		_rowName,
		TEXT("Item Data Lookup")
	);

	ItemImage->SetBrushFromTexture(RowData->ItemImage);
	AmountTextBlock->SetText(FText::AsNumber(_iAmount));


	ItemRowName = _rowName;
}




void UQuickSlotWidget::Change_Amount(int32 _iAmount)
{
	AmountTextBlock->SetText(FText::AsNumber(_iAmount));

	if (_iAmount == 0)
	{
		ItemImage->SetBrushFromTexture(BlankImage);
		ItemRowName = NAME_None;
	}
}
