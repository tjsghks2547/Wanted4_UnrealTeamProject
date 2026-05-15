// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/IInteractableTarget.h"
#include "KZMonsterCharacter.generated.h"

UCLASS()
class KHAZAN_API AKZMonsterCharacter : public ACharacter, public IIInteractableTarget
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKZMonsterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 폰 센싱 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Sensing")
	TObjectPtr<class UPawnSensingComponent> PawnSensing;

	// 폰을 보았을 때 호출될 함수
	UFUNCTION()
	virtual void OnSeePawn(APawn* SeenPawn);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 인터페이스 구현
	virtual FName GetTargetType() const override { return FName("Monster"); }
	virtual bool IsAttackable() const override { return true; }

	virtual void PlayAttackMontage();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimMontage* BasicAttackMontage;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Sensing")
	TObjectPtr<APawn> TargetPawn;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CurrentMovementSpeed;


};
