// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattlePassProgression.generated.h"

struct FBattlePassSaveData;
class UBattlePass;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnLevelChanged,
	int32, NewLevel,
	int32, OldLevel
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnExperienceSet,
	float, NewExperience,
	int32, NewLevel,
	int32, OldLevel
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnExperienceAdded,
	float, AddedExperience
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnExperienceRemoved,
	float, RemovedExperience
);

UCLASS(NotBlueprintable, NotBlueprintType)
class BATTLEPASSSYSTEM_API UBattlePassProgression : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnLevelChanged OnLevelChanged;
	UPROPERTY(BlueprintAssignable)
	FOnExperienceSet OnExperienceSet;
	UPROPERTY(BlueprintAssignable)
	FOnExperienceAdded OnExperienceAdded;
	UPROPERTY(BlueprintAssignable)
	FOnExperienceRemoved OnExperienceRemoved;
	
private:
	UPROPERTY()
	UBattlePass* RelatedBattlePass;
	int32 CurrentLevel;
	float CurrentLevelExperience;

public:
	void Init(UBattlePass* InRelatedBattlePass);

	void LoadProgression(const FBattlePassSaveData& BattlePassSaveData);

	void AddLevelExperience(const float Experience);

	void RemoveLevelExperience(const float Experience);

	void SetLevelExperience(const float Experience, bool bNotify = true);
	
	int32 GetLevel() const;
	
	float GetLevelExperience() const;

	float GetExperienceForNextLevel() const;

	float GetExperienceForLevel(const int32 Level) const;

	float GetTotalExperience() const;

private:
	void CalculateLevel();
	
	UCurveFloat* GetProgressionCurve() const;

	bool Check() const;
};
