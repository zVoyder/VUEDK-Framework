// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattlePassSubsystem.h"
#include "Data/BattlePassesSaveData.h"
#include "BattlePassSaveModeBase.generated.h"

DECLARE_DELEGATE_OneParam(
	FOnBattlePassLoadCompleted,
	FBattlePassesSaveData /* LoadedData */
);

DECLARE_DELEGATE_OneParam(
	FOnBattlePassLoadFailed,
	bool /* bIsError */
);

#define SAVE_FILE_NAME TEXT("BattlePass")
#define SAVE_FILE_FULLPATH FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("SaveGames"), SAVE_FILE_NAME)

UCLASS(Abstract, Blueprintable, NotBlueprintType)
class BATTLEPASSSYSTEM_API UBattlePassSaveModeBase : public UObject
{
	GENERATED_BODY()

public:
	FOnBattlePassLoadCompleted OnBattlePassLoadComplete;
	FOnBattlePassLoadFailed OnBattlePassLoadFailed;

protected:
	UPROPERTY()
	UBattlePassSubsystem* BattlePassSubsystem;

public:
	void Init(UBattlePassSubsystem* InSubSystem);

	virtual void Save();

	virtual void Load();

protected:
	UFUNCTION(BlueprintPure)
	FBattlePassesSaveData CreateSaveData() const;

	UFUNCTION(BlueprintCallable)
	void CompleteLoad(const FBattlePassesSaveData& LoadedData) const;

	UFUNCTION(BlueprintCallable)
	void FailLoad(const bool bIsError) const;
};
