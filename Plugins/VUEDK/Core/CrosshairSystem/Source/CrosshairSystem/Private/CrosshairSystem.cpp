// Copyright VUEDKs, Inc. All Rights Reserved.

#include "CrosshairSystem.h"
#include "CrosshairSystemSettings.h"
#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FCrosshairSystemModule"

DEFINE_LOG_CATEGORY(LogCrosshairSystem);

void FCrosshairSystemModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "Crosshair System",
			LOCTEXT("RuntimeSettingsName", "Crosshair System"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Crosshair System Plugin"),
			GetMutableDefault<UCrosshairSystemSettings>()
		);
	}
}

void FCrosshairSystemModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->UnregisterSettings("Project", "VUEDK", "Crosshair System");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCrosshairSystemModule, CrosshairSystem)
