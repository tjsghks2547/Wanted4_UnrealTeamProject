// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/IHPlayerState.h"
#include "Component/InventoryComponent.h"

AIHPlayerState::AIHPlayerState()
{

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("UiInventory"));

}
