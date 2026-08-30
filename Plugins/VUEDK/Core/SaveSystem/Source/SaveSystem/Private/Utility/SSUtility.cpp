// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/SSUtility.h"
#include "Utility/SSSlotsUtility.h"

TWeakObjectPtr<USaveManager> USSUtility::SaveManager = nullptr;
TWeakObjectPtr<UAutoSaveManager> USSUtility::AutoSaveManager = nullptr;

void USSUtility::Init(USaveManager* InSaveManager)
{
	SaveManager = InSaveManager;
}

void USSUtility::SetAutoSaveManager(UAutoSaveManager* InAutoSaveManager)
{
	AutoSaveManager = InAutoSaveManager;
}

USaveManager* USSUtility::GetSaveManager()
{
	return SaveManager.Get();
}

UAutoSaveManager* USSUtility::GetAutoSaveManager()
{
	return AutoSaveManager.Get();
}

UDefaultSaveGame* USSUtility::GetSaveGame()
{
	if (!Check())
		return nullptr;

	return SaveManager.Get()->GetSaveGameInstance();
}

UDefaultSaveGame* USSUtility::GetSharedSaveGame()
{
	if (!Check())
		return nullptr;

	return SaveManager.Get()->GetSharedSaveGameInstance();
}

void USSUtility::StartNewSaveGame()
{
	if (!Check())
		return;

	SaveManager.Get()->StartNewSaveGame();
}

void USSUtility::ManualSave(UObject* Instigator, const bool bOverrideCurrentSlot, const bool bAsync)
{
	if (!Check())
		return;

	SaveManager.Get()->ManualSave(Instigator, bOverrideCurrentSlot, bAsync);
}

void USSUtility::SharedSave(UObject* Instigator, const bool bAsync)
{
	if (!Check())
		return;

	SaveManager.Get()->SharedSave(Instigator, bAsync);
}

void USSUtility::ManualAndSharedSave(UObject* Instigator, const bool bOverrideCurrentSlot, const bool bAsync)
{
	if (!Check())
		return;

	SaveManager.Get()->ManualAndSharedSave(Instigator, bOverrideCurrentSlot, bAsync);
}

void USSUtility::SelectAndSaveOnSlot(const FString& SlotName, UObject* Instigator, const bool bAsync)
{
	if (!Check() || !USSSlotsUtility::TrySelectSaveGameSlot(SlotName))
		return;

	SaveManager.Get()->Save(SlotName, Instigator, false, bAsync);
}

void USSUtility::SelectAndLoadFromSlot(const FString& SlotName, UObject* Instigator, const bool bAsync)
{
	if (!Check() || !USSSlotsUtility::TrySelectSaveGameSlot(SlotName))
		return;

	SaveManager.Get()->Load(SlotName, Instigator, false, bAsync);
}

void USSUtility::LoadSelectedSlot(UObject* Instigator, const bool bAsync)
{
	if (!Check())
		return;

	SaveManager.Get()->LoadSelectedSlot(Instigator, bAsync);
}

void USSUtility::LoadSharedSlot(UObject* Instigator, const bool bAsync)
{
	if (!Check())
		return;

	SaveManager.Get()->LoadSharedSlot(Instigator, bAsync);
}

void USSUtility::LoadSelectedSlotAndSharedSlot(UObject* Instigator, const bool bAsync)
{
	if (!Check())
		return;

	SaveManager.Get()->LoadSelectedSlotAndSharedSlot(Instigator, bAsync);
}

void USSUtility::PauseAutoSave()
{
	if (!Check() || !AutoSaveManager.IsValid())
		return;

	AutoSaveManager.Get()->PauseAutoSave();
}

void USSUtility::UnpauseAutoSave()
{
	if (!Check() || !AutoSaveManager.IsValid())
		return;

	AutoSaveManager.Get()->UnpauseAutoSave();
}

void USSUtility::StartTimePlayedTimer()
{
	if (!Check())
		return;

	SaveManager.Get()->StartTimePlayedTimer();
}

bool USSUtility::HasInstanceEverLoaded()
{
	if (!Check())
		return false;

	return SaveManager.Get()->HasInstanceEverLoaded();
}

bool USSUtility::HasInstanceEverSaved()
{
	if (!Check())
		return false;

	return SaveManager.Get()->HasInstanceEverSaved();
}

bool USSUtility::HasSharedInstanceEverLoaded()
{
	if (!Check())
		return false;

	return SaveManager.Get()->HasSharedInstanceEverLoaded();
}

bool USSUtility::HasSharedInstanceEverSaved()
{
	if (!Check())
		return false;

	return SaveManager.Get()->HasSharedInstanceEverSaved();
}

bool USSUtility::IsNewSaveGame()
{
	if (!Check())
		return false;

	return SaveManager.Get()->IsNewSaveGame();
}

bool USSUtility::Check()
{
	return SaveManager.IsValid();
}
