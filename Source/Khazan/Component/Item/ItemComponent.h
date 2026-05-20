// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"


UCLASS(ClassGroup = (Custom), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class KHAZAN_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	FDataTableRowHandle Get_ItemData() { return ItemData; }
	void Set_ItemRowName(FName _Name);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = item)
	FDataTableRowHandle ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = item)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = item)
	int ItemAmount = 1;

};
