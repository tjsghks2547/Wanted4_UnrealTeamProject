// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Item/TreasureBox.h"
#include "Components/BoxComponent.h"
#include "KZCharacter/KZCharacterPlayer.h"
#include "Components/SlateWrapperTypes.h"
#include "Types/InterActionType.h"
#include "UI/PlayerUIWidget.h"
#include "KZPlayer/KZPlayerController.h"
#include "Animation/Item/TreasureBox/TreasureBox_AnimInstance.h"
#include "BottleItem.h"

// Sets default values
ATreasureBox::ATreasureBox()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시 컴포넌트 설정 ( 크기 설정 )
	GetMesh()->SetRelativeScale3D(FVector(0.01f, 0.01f, 0.01f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));


	// 메시 에셋 지정 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BoxMeshRef(TEXT("/Game/Character/Item/TreaseBox/Treasurebox.Treasurebox"));
	if (BoxMeshRef.Object != NULL)
	{
		GetMesh()->SetSkeletalMesh(BoxMeshRef.Object);
	};

	// 애니 블루프린트 클래스 저장
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Blueprint/Item/TreasureBox/ABP_TreasureBox.ABP_TreasureBox_C"));
	if (AnimInstanceRef.Class != NULL)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	//// 애니메이션 몽타주 저장 
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> OpenBoxMontageRef(TEXT("/Game/BluePrint/Character/Item/TreasureBox/AM_OpenBox.AM_OpenBox"));
	//if (OpenBoxMontageRef.Object != NULL)
	//{
	//	OpenBoxMontage = OpenBoxMontageRef.Object;
	//}

	// Scene 컴포넌트 설정
	BoxColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box_Collider"));
	ensureAlways(BoxColliderComponent);

	// 루트에 붙이기
	BoxColliderComponent->SetupAttachment(RootComponent);

	//// 콜리전 설정
	//m_pBoxColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//m_pBoxColliderComponent->SetGenerateOverlapEvents(true);

	BoxColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ATreasureBox::OnOverlapBegin);
	BoxColliderComponent->OnComponentEndOverlap.AddDynamic(this, &ATreasureBox::OnOverlapEnd);



#pragma region Drop Item 
	static ConstructorHelpers::FClassFinder<ABottleItem> BottleItemClassRef(
		TEXT("/Game/Blueprint/Item/Bottle/BP_Bottle.BP_Bottle_C")
	);

	if (BottleItemClassRef.Class != nullptr)
	{
		DropItemClass = BottleItemClassRef.Class;
	}
#pragma endregion 
}

// Called when the game starts or when spawned
void ATreasureBox::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATreasureBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// 현재 겹치고 있는 Actor 목록 가져오기
	BoxColliderComponent->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		//UE_LOG(LogTemp, Warning, TEXT("겹치는 중: %s"), *Actor->GetName());

		if (Actor->ActorHasTag(TEXT("Player")))
		{
			AKZCharacterPlayer* pPlayer = Cast<AKZCharacterPlayer>(Actor);

			if (pPlayer != NULL)
			{
				if (pPlayer->Get_Ui_Key_Statue() == true)
				{
					// 애니메이션 재생 
					int a = 4;

					if (HasPlayedAnimation == false)
					{
						UTreasureBox_AnimInstance* pAnimInstance = Cast<UTreasureBox_AnimInstance>(GetMesh()->GetAnimInstance());
						pAnimInstance->Set_BoxStatue(true);
						HasPlayedAnimation = true;

						/* 5월 26일 추가 */
						pPlayer->Set_Current_OverlapTypes(EInterActionType::None);
						pPlayer->Render_InterActionUi(EInterActionType::Item, ESlateVisibility::Hidden);
						/* ---------------------- */

						SpawnDropItem();
					}

				}
			}
		}
	}

	// 여기서 UI on/off하면 될듯 

}

// Called to bind functionality to input
void ATreasureBox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATreasureBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")) && HasPlayedAnimation == false)
	{
		AKZCharacterPlayer* pPlayer = Cast<AKZCharacterPlayer>(OtherActor);

		if (pPlayer != NULL)
		{
			pPlayer->Render_InterActionUi(EInterActionType::Item, ESlateVisibility::Visible);
			pPlayer->Set_Current_OverlapTypes(EInterActionType::Item);
		}
	}
}

void ATreasureBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Player")) && HasPlayedAnimation == false)
	{
		AKZCharacterPlayer* pPlayer = Cast<AKZCharacterPlayer>(OtherActor);

		if (pPlayer != NULL)
		{

			pPlayer->Render_InterActionUi(EInterActionType::Item, ESlateVisibility::Collapsed);
			pPlayer->Set_Current_OverlapTypes(EInterActionType::None);
			pPlayer->Ui_Key_State_Reset();
			HasPlayedAnimation = false;

		}
	}
}

void ATreasureBox::SpawnDropItem()
{
	UWorld* World = GetWorld();

	const FVector  SpawnLocation = GetActorLocation() + DropSpawnOffset;
	const FRotator SpawnRotation = GetActorRotation();


	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedItem = World->SpawnActor<AActor>(
		DropItemClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	UPrimitiveComponent* ItemRootComp = Cast<UPrimitiveComponent>(SpawnedItem->GetRootComponent());
	UPrimitiveComponent* BoxRootComp = Cast<UPrimitiveComponent>(GetRootComponent());

	if (ItemRootComp && BoxRootComp)
	{
		ItemRootComp->IgnoreActorWhenMoving(this, true);
		BoxRootComp->IgnoreActorWhenMoving(SpawnedItem, true);
	}


	ABottleItem* pBottle = Cast<ABottleItem>(SpawnedItem);
	pBottle->StartDropMotion(GetActorForwardVector());

}

