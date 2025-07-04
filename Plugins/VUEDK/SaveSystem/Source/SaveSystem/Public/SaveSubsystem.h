// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AutoSaveManager.h"
#include "SaveManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSubsystem.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class SAVESYSTEM_API USaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	USaveManager* SaveManager;
	UPROPERTY()
	UAutoSaveManager* AutoSaveManager;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	/**
	 * Gets the save manager instance.
	 * @return Pointer to the USaveManager instance.
	 */
	USaveManager* GetSaveManager() const;

	/**
	 * Gets the auto save manager instance.
	 * @return Pointer to the UAutoSaveManager instance.
	 */
	UAutoSaveManager* GetAutoSaveManager() const;
};
