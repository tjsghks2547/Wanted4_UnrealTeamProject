// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TreasureBox.generated.h"

UCLASS()
class KHAZAN_API ATreasureBox : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATreasureBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SpawnDropItem();

protected:
	UPROPERTY(VisibleAnywhere, Category = Collision)
	TObjectPtr<class UBoxComponent> BoxColliderComponent;

	UPROPERTY(VisibleAnywhere, Category = OverlapActor)
	TArray<AActor*> OverlappingActors;

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> OpenBoxMontage;

	bool HasPlayedAnimation = false;

	/* Drop Item ฐüทร */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	TSubclassOf<AActor> DropItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	FVector DropSpawnOffset = FVector(0.f, 0.f, 50.f);

};
