// Copyright VUEDK, Inc. All Rights Reserved.

#include "SaveSystem.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "SaveSystemSettings.h"

#define LOCTEXT_NAMESPACE "FSaveSystemModule"

DEFINE_LOG_CATEGORY(LogSaveSystem);

void FSaveSystemModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "Save System",
		                                LOCTEXT("RuntimeSettingsName", "Save System"),
		                                LOCTEXT("RuntimeSettingsDescription", "Configure the Save System Plugin"),
		                                GetMutableDefault<USaveSystemSettings>()
		);
	}
}

void FSaveSystemModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		SettingModule->UnregisterSettings("Project", "VUEDK", "Save System");
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSaveSystemModule, SaveSystem)
