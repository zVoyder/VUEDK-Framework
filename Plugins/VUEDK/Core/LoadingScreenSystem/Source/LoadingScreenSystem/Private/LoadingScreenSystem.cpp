// Copyright VUEDK, Inc. All Rights Reserved.

#include "LoadingScreenSystem.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "LoadingScreenSystemSettings.h"

#define LOCTEXT_NAMESPACE "FLoadingScreenSystemModule"

DEFINE_LOG_CATEGORY(LogLoadingScreenSystem);

void FLoadingScreenSystemModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "Loading Screen System",
			LOCTEXT("RuntimeSettingsName", "Loading Screen System"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Loading Screen System Plugin"),
			GetMutableDefault<ULoadingScreenSystemSettings>()
		);
	}
}

void FLoadingScreenSystemModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->UnregisterSettings("Project", "VUEDK", "Loading Screen System");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLoadingScreenSystemModule, LoadingScreenSystem)