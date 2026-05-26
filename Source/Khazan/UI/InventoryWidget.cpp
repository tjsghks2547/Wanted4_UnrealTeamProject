// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"
#include "UI/ItemWidget.h"
#include "UI/ItemTipWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"


UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	static ConstructorHelpers::FClassFinder<UItemWidget> ItemWidgetRef(TEXT("/Game/UI/Inventory/WBP_Item.WBP_Item_C"));
	if (ItemWidgetRef.Class != NULL)
	{
		ItemWidgetClass = ItemWidgetRef.Class;
	}



}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();


	for (int i = 0; i < 14; i++)
	{
		FString SlotName = FString::Printf(TEXT("Slot_%d"), i);
		SlotArray.Add(Cast<UItemWidget>(GetWidgetFromName(FName(*SlotName))));
		SlotArray[i]->Set_ParentInventory(this);
	}


	ItemTipWidget = Cast<UItemTipWidget>(GetWidgetFromName(TEXT("WBP_ItemTip")));
	ItemTipWidget->SetVisibility(ESlateVisibility::Hidden);
}



void UInventoryWidget::UpdateInventory(TMap<FName, int32>& _ItemMapContainer)
{

	int32 Index = 0;

	for (auto& iter : _ItemMapContainer)
	{
		SlotArray[Index]->Slot_Update(iter.Key, iter.Value);
		Index++;
	}


	/* 여기서는 이제 해당 ui를 rendering 할지 말지를 결정하는 형태 */
	ItemTipWidget->SetVisibility(ESlateVisibility::Hidden);


}

void UInventoryWidget::ShowItemTip(FName _itemData)
{
	ItemTipWidget->SetVisibility(ESlateVisibility::Visible);
	ItemTipWidget->Update_ItemTipWidget(_itemData);
}


void UInventoryWidget::Init_Slot()
{

}


