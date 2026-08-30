// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/BattlePass.h"
#include "Objects/BattlePassReward.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveModes/Base/BattlePassSaveModeBase.h"
#include "BattlePassFactory.generated.h"

UCLASS()
class BATTLEPASSSYSTEM_API UBattlePassFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UBattlePassProgression* CreateBattlePassProgression(UBattlePass* RelatedBattlePass);
	
	static UBattlePass* CreateBattlePass(const FGameplayTag& BattlePassTag, UBattlePassData* BattlePassData, const bool bIsActive);

	static UBattlePassTier* CreateBattlePassTier(UBattlePass* BattlePass, const int32 TierIndex, const FBattlePassTierData& BattlePassTierData);
	
	static UBattlePassReward* CreateBattlePassReward(UBattlePassTier* BattlePassTier, UBattlePassRewardData* BattlePassRewardData);

	static UBattlePassSaveModeBase* CreateBattlePassSaveMode(const TSoftClassPtr<UBattlePassSaveModeBase> SaveModeClass, UBattlePassSubsystem* Subsystem);
};
