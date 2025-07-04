// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/SSUtility.h"
#include "Utility/SSSlotsUtility.h"

USaveManager* USSUtility::CurrentSaveManager = nullptr;
UAutoSaveManager* USSUtility::AutoSaveManager = nullptr;

void USSUtility::Init(USaveManager* SaveManager)
{
	CurrentSaveManager = SaveManager;
}

void USSUtility::SetAutoSaveManager(UAutoSaveManager* InAutoSaveManager)
{
	AutoSaveManager = InAutoSaveManager;
}

USaveManager* USSUtility::GetSaveManager()
{
	return CurrentSaveManager;
}

UAutoSaveManager* USSUtility::GetAutoSaveManager()
{
	return AutoSaveManager;
}

UDefaultSaveGame* USSUtility::GetSaveGame()
{
	if (!Check()) return nullptr;

	return CurrentSaveManager->GetSaveGameInstance();
}

UDefaultSaveGame* USSUtility::GetSharedSaveGame()
{
	if (!Check()) return nullptr;

	return CurrentSaveManager->GetSharedSaveGameInstance();
}

void USSUtility::StartNewSaveGame()
{
	if (!Check()) return;

	CurrentSaveManager->StartNewSaveGame();
}

void USSUtility::ManualSave(UObject* Instigator, const bool bOverrideCurrentSlot)
{
	if (!Check()) return;

	CurrentSaveManager->ManualSave(Instigator, bOverrideCurrentSlot);
}

void USSUtility::SharedSave(UObject* Instigator)
{
	if (!Check()) return;

	CurrentSaveManager->SharedSave(Instigator);
}

void USSUtility::ManualAndSharedSave(UObject* Instigator, const bool bOverrideCurrentSlot)
{
	if (!Check()) return;

	CurrentSaveManager->ManualAndSharedSave(Instigator, bOverrideCurrentSlot);
}

void USSUtility::SelectAndSaveOnSlot(const FString& SlotName, UObject* Instigator)
{
	if (!Check() || !USSSlotsUtility::TrySelectSaveGameSlot(SlotName)) return;

	CurrentSaveManager->Save(SlotName, Instigator);
}

void USSUtility::LoadSelectedSlot(UObject* Instigator)
{
	if (!Check()) return;

	CurrentSaveManager->LoadSelectedSlot(Instigator);
}

void USSUtility::LoadSharedSlot(UObject* Instigator)
{
	if (!Check()) return;

	CurrentSaveManager->LoadSharedSlot(Instigator);
}

void USSUtility::LoadSelectedSlotAndSharedSlot(UObject* Instigator)
{
	if (!Check()) return;

	CurrentSaveManager->LoadSelectedSlotAndSharedSlot(Instigator);
}

void USSUtility::StartTimePlayedTimer()
{
	if (!Check()) return;

	CurrentSaveManager->StartTimePlayedTimer();
}

bool USSUtility::HasInstanceEverLoaded()
{
	if (!Check()) return false;

	return CurrentSaveManager->HasInstanceEverLoaded();
}

bool USSUtility::HasInstanceEverSaved()
{
	if (!Check()) return false;

	return CurrentSaveManager->HasInstanceEverSaved();
}

bool USSUtility::HasSharedInstanceEverLoaded()
{
	if (!Check()) return false;

	return CurrentSaveManager->HasSharedInstanceEverLoaded();
}

bool USSUtility::HasSharedInstanceEverSaved()
{
	if (!Check()) return false;

	return CurrentSaveManager->HasSharedInstanceEverSaved();
}

void USSUtility::SetSaveMasterID(const FName& SaveMasterID)
{
	if (!Check()) return;

	CurrentSaveManager->SetSaveMasterID(SaveMasterID);
}

FName USSUtility::GetSaveMasterID()
{
	if (!Check()) return NAME_None;

	return CurrentSaveManager->GetSaveMasterID();
}

bool USSUtility::Check()
{
	return CurrentSaveManager != nullptr;
}
