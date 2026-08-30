// Copyright VUEDK, Inc. All Rights Reserved.

#include "BetterUI.h"
#include "BetterUISettings.h"
#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FBetterUIModule"

DEFINE_LOG_CATEGORY(LogBetterUI);

void FBetterUIModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "Better UI",
			LOCTEXT("RuntimeSettingsName", "Better UI"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Better UI Plugin"),
			GetMutableDefault<UBetterUISettings>()
		);
	}
}

void FBetterUIModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->UnregisterSettings("Project", "VUEDK", "BetterUI");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBetterUIModule, BetterUI)
