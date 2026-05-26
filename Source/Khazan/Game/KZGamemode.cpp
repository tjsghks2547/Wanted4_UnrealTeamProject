// Fill out your copyright notice in the Description page of Project Settings.


#include "KZGamemode.h"
#include "../KZCharacter/KZCharacterPlayer.h"

void AKZGamemode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	AKZCharacterPlayer* Player = Cast<AKZCharacterPlayer>(NewPlayer->GetPawn());
	if (Player)
	{
		Player->OnPlayerDead.AddLambda([this](AKZCharacterPlayer* DeadPlayer)
			{
				// 플레이어가 죽었을 때 3초 후에 자동으로 리스폰.
				FTimerHandle RespawnTimerHandle;
				GetWorldTimerManager().SetTimer(RespawnTimerHandle, [this, DeadPlayer]()
					{
						if (AController* PlayerController = Cast<AController>(DeadPlayer->GetController()))
						{
							RestartPlayer(PlayerController);
						}
					}, 3.0f, false);
			});
	}
}
