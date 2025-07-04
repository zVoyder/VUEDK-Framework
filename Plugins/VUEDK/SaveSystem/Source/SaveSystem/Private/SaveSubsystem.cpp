// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveSubsystem.h"
#include "SaveSystemSettings.h"

void USaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	const USaveSystemSettings* Settings = GetDefault<USaveSystemSettings>();
	if (!IsValid(Settings))
	{
		UE_LOG(LogSaveSystem, Error, TEXT("USaveSubsystem::Initialize: SaveSystemSettings is not valid."));
		return;
	}
	
	SaveManager = NewObject<USaveManager>(this);
	SaveManager->Init(Settings->SaveManagerData);

	if (Settings->bEnableAutoSave)
	{
		AutoSaveManager = NewObject<UAutoSaveManager>(this);
		AutoSaveManager->Init(SaveManager, Settings->AutoSaveManagerData);
	}
}

void USaveSubsystem::Deinitialize()
{
	Super::Deinitialize();

	if (IsValid(SaveManager))
		SaveManager->ConditionalBeginDestroy();

	if (IsValid(AutoSaveManager))
		AutoSaveManager->ConditionalBeginDestroy();
}

USaveManager* USaveSubsystem::GetSaveManager() const
{
	return SaveManager;
}

UAutoSaveManager* USaveSubsystem::GetAutoSaveManager() const
{
	return AutoSaveManager;
}
