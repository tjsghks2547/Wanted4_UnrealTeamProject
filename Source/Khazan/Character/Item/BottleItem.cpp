// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Item/BottleItem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Component/Item/ItemComponent.h"
#include "Component/InventoryComponent.h"
#include "Data/ItemDataTable.h"
#include "Player/IHPlayerState.h"

// Sets default values
ABottleItem::ABottleItem()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// 메시 에셋 지정 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BottleMeshRef(TEXT("/Game/Character/Item/Bottle/bottle.bottle"));
	if (BottleMeshRef.Object != NULL)
	{
		GetMesh()->SetSkeletalMesh(BottleMeshRef.Object);
	}


	// 컴포넌트 부착 
	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("ItemComponent"));
	ensureAlways(ItemComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = RootComponent;

	// 자동으로 바로 날아가지 않게 비활성화
	ProjectileMovement->bAutoActivate = false;

	// 중력 적용 비율
	ProjectileMovement->ProjectileGravityScale = 1.0f;

	// 바닥에 닿았을 때 튕길지 여부
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.2f;

	// 너무 오래 미끄러지지 않게 감속
	ProjectileMovement->Friction = 0.8f;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABottleItem::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ABottleItem::OnOverlapEnd);

	//FName _itemKey, int _itemAmount
}

// Called when the game starts or when spawned
void ABottleItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABottleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0.f, 90.f * DeltaTime, 0.f));

}

// Called to bind functionality to input
void ABottleItem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ABottleItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		if (HasPickUp == false)
		{
			APawn* PlayerPawn = Cast<APawn>(OtherActor);
			AIHPlayerState* pPlayerState = PlayerPawn->GetPlayerState<AIHPlayerState>();
			UInventoryComponent* InventoryComponent = pPlayerState->Get_InventoryComponent();


			if (InventoryComponent != NULL)
			{
				FDataTableRowHandle ItemHandle = ItemComponent->Get_ItemData();

				ItemComponent->Set_ItemRowName(TEXT("Bread"));
				InventoryComponent->Add_Item(ItemHandle.RowName, 1);
			}

			HasPickUp = true;

			Destroy();

			// 해당 수정 전 ( 의존성 깨트리기 이러면 플레이어를 포함안해도 된다.) 
			//ACharacter_NaNally* pPlayer = Cast<ACharacter_NaNally>(OtherActor);
			//
			//FDataTableRowHandle ItemHandle = ItemComponent->Get_ItemData();
			//pPlayer->Get_InventoryComponent()->Add_Item(ItemHandle.RowName, 1);
			//
			//HasPickUp = true;
			//
			//Destroy();

		}

	}
}


void ABottleItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{


	}
}


void ABottleItem::StartDropMotion(const FVector& Direction)
{
	if (ProjectileMovement == nullptr)
		return;

	FVector LaunchDirection = Direction.GetSafeNormal();

	// 앞으로 나가는 힘 + 위로 뜨는 힘
	FVector LaunchVelocity =
		LaunchDirection * 300.f +
		FVector(0.f, 0.f, 500.f);

	ProjectileMovement->Velocity = LaunchVelocity;
	ProjectileMovement->Activate(true);
}