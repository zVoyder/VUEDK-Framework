// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/BattlePassData.h"
#include "SaveModes/BattlePassCryptedBinarySaveMode.h"
#include "SaveModes/Base/BattlePassSaveModeBase.h"
#include "UObject/Object.h"
#include "BattlePassSystemSettings.generated.h"

UCLASS(Config = BattlePassSystemSettings, DefaultConfig, meta = (DisplayName = "Battle Pass System Settings"))
class BATTLEPASSSYSTEM_API UBattlePassSystemSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config, Category = "Activation",
		meta = (ToolTip = "If true, all registered battle passes are active. If false, only those listed in ActiveBattlePasses are active."))
	bool bAllBattlePassesActive = true;
	UPROPERTY(EditAnywhere, Config, Category = "Activation",
		meta = (EditCondition = "!bAllBattlePassesActive",
				ToolTip = "Explicit list of battle passes that are active and can be progressed."))
	TArray<FGameplayTag> ActiveBattlePasses;
	
	UPROPERTY(EditAnywhere, Config, Category = "Registry",
		meta = (ToolTip = "Mapping between battle pass identifiers and their corresponding data assets."))
	TMap<FGameplayTag, TSoftObjectPtr<UBattlePassData>> BattlePasses;

	UPROPERTY(EditAnywhere, Config, Category = "Saving",
	meta = (ToolTip = "The save mode used for saving and loading battle pass progress."))
	TSoftClassPtr<UBattlePassSaveModeBase> SaveModeClass = UBattlePassCryptedBinarySaveMode::StaticClass();
};
