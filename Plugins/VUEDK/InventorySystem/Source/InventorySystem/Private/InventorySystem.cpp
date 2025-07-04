// Copyright VUEDK, Inc. All Rights Reserved.

#include "InventorySystem.h"
#include "InventorySystemSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FInventorySystemModule"

void FInventorySystemModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "Inventory System",
			LOCTEXT("RuntimeSettingsName", "Inventory System"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Inventory System Plugin"),
			GetMutableDefault<UInventorySystemSettings>()
		);
	}
}

void FInventorySystemModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->UnregisterSettings("Project", "VUEDK", "Inventory System");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInventorySystemModule, InventorySystem)