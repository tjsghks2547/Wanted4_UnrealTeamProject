// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemWidget.h"
#include "KZCharacter/KZCharacterPlayer.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Data/ItemDataTable.h"
#include "UI/InventoryWidget.h"
#include "UI/ItemContextMenuWidget.h"

UItemWidget::UItemWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> NormalBorderTextureRef(TEXT("/Game/UI/Inventory/YH_UI_tishikuang_else02.YH_UI_tishikuang_else02"));
	if (NormalBorderTextureRef.Object != NULL)
	{
		NormalBorderButtonTexture = NormalBorderTextureRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UTexture2D> SelectedBorderTextureRef(TEXT("/Game/UI/Inventory/YH_UI_tishikuang_else.YH_UI_tishikuang_else"));
	if (SelectedBorderTextureRef.Object != NULL)
	{
		SelectedBorderButtonTexture = SelectedBorderTextureRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UTexture2D> SlotImageTextureGreenRef(TEXT("/Game/UI/Inventory/UI_ICONbg_green.UI_ICONbg_green"));

	if (SlotImageTextureGreenRef.Object != NULL)
		ArrayBackGroundTexture.Push(SlotImageTextureGreenRef.Object);


	static ConstructorHelpers::FObjectFinder<UTexture2D> SlotImageTextureOrangeRef(TEXT("/Game/UI/Inventory/UI_ICONbg_orange.UI_ICONbg_orange"));

	if (SlotImageTextureOrangeRef.Object != NULL)
		ArrayBackGroundTexture.Push(SlotImageTextureOrangeRef.Object);

	static ConstructorHelpers::FObjectFinder<UTexture2D> SlotImageTextureBlankRef(TEXT("/Game/UI/Inventory/UI_ICONbg_grey.UI_ICONbg_grey"));
	if (SlotImageTextureBlankRef.Object != NULL)
		ArrayBackGroundTexture.Push(SlotImageTextureBlankRef.Object);


	static ConstructorHelpers::FObjectFinder<UTexture2D> Texture2DRef(TEXT("/Game/UI/Inventory/ItemTexture/BlankImage.BlankImage"));
	if (Texture2DRef.Succeeded())
	{
		BlankImage = Texture2DRef.Object;
	}


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

	ItemClickButton = Cast<UButton>(GetWidgetFromName(TEXT("Slot_Button")));
	ensureAlways(ItemClickButton);

	ItemBackGroundImage = Cast<UImage>(GetWidgetFromName(TEXT("Slot_Image")));
	ensureAlways(ItemBackGroundImage);

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("Item_Image")));
	ensureAlways(ItemImage);


	AmountTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Item_Amount_Text")));
	ensureAlways(AmountTextBlock);

	ItemContextMenuWidget = Cast<UItemContextMenuWidget>(GetWidgetFromName(TEXT("WBP_Item_Context_Menu")));
	ensureAlways(ItemContextMenuWidget);

	// Single    델리게이트 → BindDynamic
	// Multicast 델리게이트 → AddDynamic
	// 이벤트 바인딩

	ItemClickButton->OnPressed.AddDynamic(this, &UItemWidget::OnItemImageClicked);

	ItemContextMenuWidget->SetVisibility(ESlateVisibility::Hidden);
}

FReply UItemWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// ESC 키 누르면 인벤토리 닫기
	if (InKeyEvent.GetKey() == EKeys::B)
	{
		// 인벤토리 닫기 로직
		int a = 4;
		return FReply::Handled();  // 이벤트 처리 완료
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		// 장착/해제 로직 호출
		ItemContextMenuWidget->SetVisibility(ESlateVisibility::Visible);

		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UItemWidget::Slot_Update(FName _ItemKey, int32 _iAmount)
{

	FItemDataTable* RowData = ItemData.DataTable->FindRow<FItemDataTable>(
		_ItemKey,
		TEXT("Item Data Lookup")
	);

	if (_iAmount >= 1)
	{
		ItemName = _ItemKey;

		switch (RowData->ItemType)
		{
		case EItemType::Consumable:
		{
			ItemBackGroundImage->SetBrushFromTexture(ArrayBackGroundTexture[0]);
			ItemContextMenuWidget->Set_ItemType(_ItemKey, EItemType::Consumable, _iAmount);
		}
		break;
		case EItemType::Weapon:
			ItemBackGroundImage->SetBrushFromTexture(ArrayBackGroundTexture[1]);
			ItemContextMenuWidget->Set_ItemType(_ItemKey, EItemType::Weapon, _iAmount);
			break;
		case EItemType::Armor:

			break;
		case EItemType::Etc:

			break;
		default:
			break;
		}

		ItemImage->SetBrushFromTexture(RowData->ItemImage);
		AmountTextBlock->SetText(FText::AsNumber(_iAmount));

	}


	else
	{
		ItemName = NAME_None;

		ItemBackGroundImage->SetBrushFromTexture(ArrayBackGroundTexture[2]);
		ItemImage->SetBrushFromTexture(BlankImage);
		AmountTextBlock->SetText(FText::AsNumber(_iAmount));
	}

}

void UItemWidget::OnItemImageClicked()
{
	if (!ItemName.IsNone())
	{
		if (ParentInventory)
		{
			ParentInventory->ShowItemTip(ItemName);
		}
	}
}



