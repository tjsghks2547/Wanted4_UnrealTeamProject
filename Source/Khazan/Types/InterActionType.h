#pragma once

#include "CoreMinimal.h"
#include "InterActionType.generated.h"

UENUM(BlueprintType)
enum class EInterActionType : uint8
{
	None UMETA(DisplayName = "None"),
	Dialog UMETA(DisplayName = "Dialog"),
	Chest UMETA(DisplayName = "Chest"),
	Item UMETA(DisplayName = "Item")
};


UENUM(BlueprintType)
enum class EInterAction_Key_Type : uint8
{
	F_KEY UMETA(DisplayName = "F_KEY"),

};



USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int Amount = 0;
};



