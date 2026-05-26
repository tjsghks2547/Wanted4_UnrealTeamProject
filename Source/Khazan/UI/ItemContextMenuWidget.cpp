// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemContextMenuWidget.h"
#include "Components/Button.h"
#include "Data/ItemDataTable.h"
#include "KZPlayer/KZPlayerController.h"
#include "HUD/IH_HUD.h"
#include "UI/PlayerUIWidget.h"
#include "UI/QuickSlotWidget.h"

UItemContextMenuWidget::UItemContextMenuWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{


}

void UItemContextMenuWidget::NativeConstruct()
{
	Super::NativeConstruct(); 

	Equip_Button = Cast<UButton>(GetWidgetFromName(TEXT("Equip_Button")));
	ensureAlways(Equip_Button);

	Release_Button = Cast<UButton>(GetWidgetFromName(TEXT("Release_Button")));
	ensureAlways(Release_Button);

	Equip_Button->OnPressed.AddDynamic(this, &UItemContextMenuWidget::ItemEquip);
}

void UItemContextMenuWidget::ItemEquip()
{
	switch (ItemType)
	{
	case EItemType::Consumable:
	{
		AKZPlayerController* PlayerController = Cast<AKZPlayerController>(GetOwningPlayer());
		AIH_HUD* pHUD = Cast<AIH_HUD>(PlayerController->Get_HUD());
		
		UPlayerUIWidget* pMainWidget = Cast<UPlayerUIWidget>(pHUD->Get_MainUI_Widget());

		UQuickSlotWidget* pQuickSlot = Cast<UQuickSlotWidget>(pMainWidget->Get_QuickSlot());

		pQuickSlot->Set_Slot(RowName, ItemAmount);
	
	}
		break;
	case EItemType::Weapon:
		break;
	case EItemType::Armor:
		break;
	case EItemType::Etc:
		break;
	default:
		break;
	}





	this->SetVisibility(ESlateVisibility::Hidden);


}
