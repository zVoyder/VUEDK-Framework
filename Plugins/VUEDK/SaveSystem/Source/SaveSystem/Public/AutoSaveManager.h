// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveManager.h"
#include "Data/Structs/AutoSaveManagerData.h"
#include "AutoSaveManager.generated.h"

UCLASS()
class SAVESYSTEM_API UAutoSaveManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	USaveManager* CurrentSaveManager;
	FAutoSaveManagerData AutoSaveData;
	FTimerHandle AutoSaveTimerHandle;
	bool bIsPaused;

public:
	void Init(USaveManager* SaveManager, const FAutoSaveManagerData& InAutoSaveData);

	UFUNCTION(BlueprintCallable)
	void PauseAutoSave();

	UFUNCTION(BlueprintCallable)
	void UnpauseAutoSave();

	UFUNCTION(BlueprintPure)
	bool IsAutoSavePaused() const;
	
private:
	void AutoSave() const;
	
	FTimerManager* GetWorldTimerManager() const;
};
