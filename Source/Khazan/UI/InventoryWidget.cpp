// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"
#include "UI/ItemWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	static ConstructorHelpers::FClassFinder<UItemWidget> ItemWidgetRef(TEXT("/Game/UI/Inventory/WBP_Item.WBP_Item_C"));
	if(ItemWidgetRef.Class != NULL)
	{
		ItemWidgetClass = ItemWidgetRef.Class;
	}
	
	
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventroyCanvasPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel")));
	ensureAlways(InventroyCanvasPanel);


}

void UInventoryWidget::UpdateInventory(TMap<FName, int32>& _ItemMapContainer)
{

	int32 Index = 0;

	for(auto& iter : _ItemMapContainer)
	{
		SlotArray[Index]->Slot_Update(iter.Key, iter.Value);
		Index++;
	}


	/* 여기서는 이제 해당 ui를 rendering 할지 말지를 결정하는 형태 */



}

void UInventoryWidget::Init_Slot()
{
	const int32 TotalSlots = 20; // 5x5
	const int32 ColumnCount = 5;
	const float SlotSize = 100.f;
	const float Padding_Size = 10.f;

	const FVector2D StartPos = FVector2D(100.f, 300.f);

	for (int32 i = 0; i < TotalSlots; i++)
	{
		UItemWidget* NewSlot = CreateWidget<UItemWidget>(this, ItemWidgetClass);

		UCanvasPanelSlot* CanvasSlot = InventroyCanvasPanel->AddChildToCanvas(NewSlot);
		if (CanvasSlot)
		{
			int32 Row = i / ColumnCount;
			int32 Col = i % ColumnCount;
			CanvasSlot->SetPosition(StartPos + FVector2D(
				Col * (SlotSize + Padding_Size),
				Row * (SlotSize + Padding_Size)
			));
			CanvasSlot->SetSize(FVector2D(SlotSize, SlotSize));
		}

		SlotArray.Add(NewSlot); // TArray<UItemWidget*> SlotArray
	}


}
