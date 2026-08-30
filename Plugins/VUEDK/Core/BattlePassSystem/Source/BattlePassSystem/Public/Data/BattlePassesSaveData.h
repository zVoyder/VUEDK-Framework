// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Objects/BattlePass.h"
#include "BattlePassesSaveData.generated.h"

USTRUCT(BlueprintType)
struct FBattlePassSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 SavedLevel;
	UPROPERTY()
	float SavedLevelExperience;
	UPROPERTY()
	TMap<int32, bool> SavedTiers;

	FBattlePassSaveData() : SavedLevel(0),
	                        SavedLevelExperience(0.0f),
	                        SavedTiers()
	{
	}
};

USTRUCT(BlueprintType)
struct FBattlePassesSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FGameplayTag, FBattlePassSaveData> SavedBattlePasses;

	FBattlePassesSaveData()
	{
	}

	explicit FBattlePassesSaveData(TMap<FGameplayTag, UBattlePass*> BattlePasses)
	{
		for (const auto& BattlePassPair : BattlePasses)
		{
			const FGameplayTag& BattlePassTag = BattlePassPair.Key;
			const UBattlePass* BattlePass = BattlePassPair.Value;

			if (!IsValid(BattlePass))
				return;

			FBattlePassSaveData SaveData;
			SaveData.SavedLevel = BattlePass->GetLevel();
			SaveData.SavedLevelExperience = BattlePass->GetLevelExperience();

			for (const auto& TierPair : BattlePass->GetTiers())
			{
				int32 TierIndex = TierPair.Key;
				const UBattlePassTier* Tier = TierPair.Value;

				if (!IsValid(Tier))
					continue;

				SaveData.SavedTiers.Add(TierIndex, Tier->IsLocked());
			}

			SavedBattlePasses.Add(BattlePassTag, SaveData);
		}
	}
};
