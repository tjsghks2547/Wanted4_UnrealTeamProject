// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

enum class EItemType : uint8;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KHAZAN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void Add_Item(FName _itemKey, int _itemAmount);

	UFUNCTION()
	TMap<FName, int32>& Get_ItemMap() { return ItemContainer; }


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InventoryContainer")
	TMap<FName, int32> ItemContainer;
};
