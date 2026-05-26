// Fill out your copyright notice in the Description page of Project Settings.


#include "KZGamemode.h"
#include "../KZCharacter/KZCharacterPlayer.h"

void AKZGamemode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	AKZCharacterPlayer* Player = Cast<AKZCharacterPlayer>(NewPlayer->GetPawn());
	if (Player)
	{
		TWeakObjectPtr<AController> WeakController = NewPlayer;

		Player->OnPlayerDead.AddLambda([this, WeakController](AKZCharacterPlayer* DeadPlayer)
			{
				if (WeakController.IsValid())
				{
					// 플레이어가 죽었을 때 3초 후에 자동으로 리스폰.
					FTimerHandle RespawnTimerHandle;
					GetWorldTimerManager().SetTimer(RespawnTimerHandle, [this, WeakController, DeadPlayer]()
						{
							if (WeakController.IsValid())
							{
								AController* PC = WeakController.Get();

								PC->UnPossess();

								this->RestartPlayer(PC);

								if (DeadPlayer)
								{
									DeadPlayer->Destroy();
								}
							}

						}, 3.0f, false);
				}
				
			});
	}
}
