// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BattlePassProgression.h"
#include "BattlePassTier.h"
#include "Data/BattlePassData.h"
#include "UObject/Object.h"
#include "BattlePass.generated.h"

struct FBattlePassSaveData;

DECLARE_DELEGATE(
	FOnUpdateRewards
);

UCLASS(NotBlueprintable, BlueprintType)
class BATTLEPASSSYSTEM_API UBattlePass : public UObject
{
	GENERATED_BODY()

public:
	FOnUpdateRewards OnUpdateRewards;
	
private:
	FGameplayTag BattlePassTag;
	UPROPERTY()
	UBattlePassData* BattlePassData;
	UPROPERTY()
	TMap<int32, UBattlePassTier*> Tiers;
	UPROPERTY()
	UBattlePassProgression* Progression;
	bool bIsActive;

public:
	void Init(const FGameplayTag& InBattlePassTag, UBattlePassData* InBattlePassData, const bool bInIsActive);

	void LoadBattlePass(const FBattlePassSaveData& BattlePassSaveData);

	UFUNCTION(BlueprintPure)
	UBattlePassData* GetBattlePassData() const;

	UFUNCTION(BlueprintPure)
	UBattlePassTier* FindTier(int32 InTier = 0) const;

	UFUNCTION(BlueprintPure)
	TMap<int32, UBattlePassTier*> GetTiers() const;

	UFUNCTION(BlueprintPure)
	bool IsActive() const;

	UFUNCTION(BlueprintCallable)
	void UnlockTier(const int32 InTier) const;

	UFUNCTION(BlueprintCallable)
	void LockTier(const int32 InTier) const;

	UFUNCTION(BlueprintCallable)
	void AddLevelExperience(const float Experience) const;

	UFUNCTION(BlueprintCallable)
	void RemoveLevelExperience(const float Experience) const;

	UFUNCTION(BlueprintPure)
	float GetLevelExperience() const;

	UFUNCTION(BlueprintPure)
	float GetTotalExperience() const;

	UFUNCTION(BlueprintPure)
	float GetExperienceForNextLevel() const;

	UFUNCTION(BlueprintPure)
	float GetExperienceForLevel(const int32 Level) const;

	UFUNCTION(BlueprintPure)
	int32 GetLevel() const;

	UFUNCTION(BlueprintPure)
	UBattlePassProgression* GetProgression() const;

	UFUNCTION(BlueprintPure)
	FGameplayTag GetBattlePassTag() const;

private:
	void CreateProgression();

	void CreateTiers();

	void UpdateRewards(const bool bNotify = true);

	UFUNCTION()
	void OnLevelChanged(int32 NewLevel, int32 OldLevel);

	UFUNCTION()
	void OnTierStateChanged(const bool bIsLocked);

	bool Check() const;
};
