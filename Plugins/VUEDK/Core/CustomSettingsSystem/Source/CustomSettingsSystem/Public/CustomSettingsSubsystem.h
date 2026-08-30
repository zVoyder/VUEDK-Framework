// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomGameUserSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CustomSettingsSubsystem.generated.h"

UCLASS()
class CUSTOMSETTINGSSYSTEM_API UCustomSettingsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<FGameplayTag, UCustomOptionObject*> CustomSettingObjectsMap;
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

private:
	void CreateCustomSettingObjects();
	
	UFUNCTION()
	void OnCustomSettingsApplied();

	UFUNCTION()
	void OnCustomOptionChanged(const FGameplayTag& Tag, float Value);
};
