// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/SmartRespawnRegisterSaverBase.h"
#include "GameFramework/SaveGame.h"
#include "SmartRespawnRegisterSlotSaver.generated.h"

UCLASS()
class SMARTRESPAWNSYSTEM_API URegisterSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FSmartRespawnRegister SavedRespawnsRegister;
};

UCLASS()
class SMARTRESPAWNSYSTEM_API USmartRespawnRegisterSlotSaver : public USmartRespawnRegisterSaverBase
{
	GENERATED_BODY()

public:
	virtual void OnSaveRegister_Implementation(const FSmartRespawnRegister& RespawnsRegister) override;

	virtual void OnRestoreRegister_Implementation() override;
};
