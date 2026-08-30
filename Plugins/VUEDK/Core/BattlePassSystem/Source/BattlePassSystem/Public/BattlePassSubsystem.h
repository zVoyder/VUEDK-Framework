// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Objects/BattlePass.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BattlePassSubsystem.generated.h"

struct FBattlePassesSaveData;
class UBattlePassSaveModeBase;
class UBattlePassSystemSettings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnBattlePassesLoaded,
	bool, bSuccessful,
	bool, bLoadError
);

UCLASS()
class BATTLEPASSSYSTEM_API UBattlePassSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnBattlePassesLoaded OnBattlePassesLoaded;
	
private:
	UPROPERTY()
	TMap<FGameplayTag, UBattlePass*> BattlePasses;
	UPROPERTY()
	UBattlePassSaveModeBase* BattlePassSaveMode;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UBattlePass* GetBattlePass(const FGameplayTag& BattlePassTag) const;

	const TMap<FGameplayTag, UBattlePass*>& GetAllBattlePasses() const;

private:
	void CreateBattlePasses(const UBattlePassSystemSettings* Settings);

	void CreateSaveMode(const UBattlePassSystemSettings* Settings);

	void SaveBattlePasses() const;

	void LoadBattlePasses() const;

	void OnLoadBattlePassesComplete(FBattlePassesSaveData SavedData);

	void OnBattlePassFailedToLoad(bool bIsError);

	UFUNCTION()
	void OnAnyBattlePassExperienceSet(float NewExperience, int32 NewLevel, int32 OldLevel);
};
