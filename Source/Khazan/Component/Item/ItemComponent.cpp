// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Item/ItemComponent.h"
#include "Component/InventoryComponent.h"


// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableRef(
		TEXT("/Game/Data/DT_Item.DT_Item")
	);

	if (ItemDataTableRef.Succeeded())
	{
		ItemData.DataTable = ItemDataTableRef.Object;
	}

}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UItemComponent::Set_ItemRowName(FName _Name)
{
	ItemData.RowName = _Name;
}


