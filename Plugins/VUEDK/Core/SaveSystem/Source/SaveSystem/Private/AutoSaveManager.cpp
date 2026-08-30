// Copyright VUEDK, Inc. All Rights Reserved.

#include "AutoSaveManager.h"
#include "SaveManager.h"
#include "SaveSystem.h"
#include "Constants/SaveConstants.h"
#include "Data/Structs/SlotInfoData.h"
#include "Utility/SSSlotsUtility.h"
#include "Utility/SSUtility.h"

void UAutoSaveManager::Init(USaveManager* SaveManager, const FAutoSaveManagerData& InAutoSaveData)
{
	CurrentSaveManager = SaveManager;
	AutoSaveData = InAutoSaveData;

	AutoSaveTimerHandle = FTimerHandle();
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAutoSaveManager::AutoSave);

	if (FTimerManager* TimerManager = GetWorldTimerManager())
	{
		TimerManager->SetTimer(AutoSaveTimerHandle, TimerDelegate, AutoSaveData.AutoSaveIntervalSeconds, true);
		if (AutoSaveData.bInitPauseState)
			PauseAutoSave();
		else
			UnpauseAutoSave();
	}
	else
	{
		UE_LOG(LogSaveSystem, Error, TEXT("Auto Save Failed to Initialize: World Timer Manager is not valid."));
	}

	USSUtility::SetAutoSaveManager(this);
}

void UAutoSaveManager::AutoSave() const
{
	if (bIsPaused)
		return; // Safe check, should not happen since the timer is paused

	const int32 CurrentAutoSavesNumber = USSSlotsUtility::GetTotalAutoSaveSlots(); 
	FString AutoSaveSlotName = AUTO_SAVE_SLOT_NAME + FString::FromInt(CurrentAutoSavesNumber);
	if (CurrentAutoSavesNumber >= AutoSaveData.MaxAutoSaves)
	{
		FSlotInfoData OutSlotData;
		USSSlotsUtility::TryGetMostAncientSlotInfoData(OutSlotData, ESaveTypeFilter::Auto);
		AutoSaveSlotName = OutSlotData.SlotInfoName;
	}

	CurrentSaveManager->Save(AutoSaveSlotName, CurrentSaveManager);
}

void UAutoSaveManager::PauseAutoSave()
{
	bIsPaused = true;
	GetWorldTimerManager()->PauseTimer(AutoSaveTimerHandle);
}

void UAutoSaveManager::UnpauseAutoSave()
{
	bIsPaused = false;
	GetWorldTimerManager()->UnPauseTimer(AutoSaveTimerHandle);
}

bool UAutoSaveManager::IsAutoSavePaused() const
{
	return bIsPaused;
}

FTimerManager* UAutoSaveManager::GetWorldTimerManager() const
{
	if (!GEngine)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("UAutoSaveManager::GetWorldTimerManager: AutoSave Timer GEngine is not valid."));
		return nullptr;
	}

	const UWorld* World = GEngine->GetWorldFromContextObject(CurrentSaveManager, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		UE_LOG(LogSaveSystem, Error, TEXT("UAutoSaveManager::GetWorldTimerManager: AutoSave Timer World is not valid."));
		return nullptr;
	}

	return &World->GetTimerManager();
}
