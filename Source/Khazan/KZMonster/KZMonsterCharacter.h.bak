// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/IInteractableTarget.h"
#include "../Interface/KZDamageInterface.h"
#include "KZMonsterCharacter.generated.h"

class AAIController;
class UBlackboardComponent;

UCLASS()
class KHAZAN_API AKZMonsterCharacter : public ACharacter, public IIInteractableTarget, public IKZDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKZMonsterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// 인터페이스 구현
	virtual FName GetTargetType() const override { return FName("Monster"); }
	virtual bool IsAttackable() const override { return true; }

	virtual void PlayAttackMontage();

	void PlayAttackMontage_Internal(UAnimMontage* MontageToPlay, FName SectionName);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimMontage* BasicAttackMontage;

protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Sensing")
	//TObjectPtr<APawn> TargetPawn;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CurrentMovementSpeed;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	TObjectPtr<class UStatComponent> StatComponent;

	// IKZDamageInterface을(를) 통해 상속됨
	void ProcessDamage(const FDamageData& DamageData) override;

	UPROPERTY()
	TObjectPtr<AAIController> AIC;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComp;
};
