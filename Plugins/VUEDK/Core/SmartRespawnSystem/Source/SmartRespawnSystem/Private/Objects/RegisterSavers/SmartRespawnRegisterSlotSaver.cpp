// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/RegisterSavers/SmartRespawnRegisterSlotSaver.h"
#include "Kismet/GameplayStatics.h"

#define SAVEFILE_NAME TEXT("SmartRespawnRegister")

void USmartRespawnRegisterSlotSaver::OnSaveRegister_Implementation(const FSmartRespawnRegister& RespawnsRegister)
{
	URegisterSaveGame* SaveGame = Cast<URegisterSaveGame>(UGameplayStatics::CreateSaveGameObject(URegisterSaveGame::StaticClass()));
	SaveGame->SavedRespawnsRegister = RespawnsRegister;

	UGameplayStatics::SaveGameToSlot(
		SaveGame,
		SAVEFILE_NAME,
		0
	);
}

void USmartRespawnRegisterSlotSaver::OnRestoreRegister_Implementation()
{
	if (UGameplayStatics::DoesSaveGameExist(SAVEFILE_NAME, 0))
	{
		if (const URegisterSaveGame* LoadedGame = Cast<URegisterSaveGame>(UGameplayStatics::LoadGameFromSlot(SAVEFILE_NAME, 0)))
		{
			const FSmartRespawnRegister RespawnsRegister = LoadedGame->SavedRespawnsRegister;
			SetRegister(RespawnsRegister);
			FinishRestore(true);
			return;
		}
	}

	FinishRestore(false);
}
