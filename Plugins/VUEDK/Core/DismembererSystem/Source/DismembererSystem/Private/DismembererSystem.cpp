// Copyright VUEDK, Inc. All Rights Reserved.

#include "DismembererSystem.h"
#include "DismembererSystemSettings.h"
#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FDismembererSystemModule"

DEFINE_LOG_CATEGORY(LogDismembermentSystem);

void FDismembererSystemModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "Dismemberer System",
			LOCTEXT("RuntimeSettingsName", "Dismemberer System"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Dismemberer System Plugin"),
			GetMutableDefault<UDismembererSystemSettings>()
		);
	}
}

void FDismembererSystemModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->UnregisterSettings("Project", "VUEDK", "Dismemberer System");
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FDismembererSystemModule, DismembererSystem)
