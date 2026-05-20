// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;





	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::Add_Item(FName _itemKey, int _itemAmount)
{

	if (ItemContainer.Find(_itemKey) == NULL)
	{
		ItemContainer.Add(_itemKey, _itemAmount);

		// 여기서 Stack 사이즈 비교 ( 여기서 FDataTableHandle 가져와서 Stack size와 비교 한다음에 크기가 더 크다면 Stack Size만 유지하도록 ) 
	}

	else
	{
		ItemContainer[_itemKey] += _itemAmount;

		// 여기서 Stack 사이즈 비교 ( 여기서 FDataTableHandle 가져와서 Stack size와 비교 한다음에 크기가 더 크다면 Stack Size만 유지하도록 ) 

	}
}

