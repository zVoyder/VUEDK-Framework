// Copyright VUEDK, Inc. All Rights Reserved.

#include "CustomSettingsSystem.h"
#include "CSSSettings.h"
#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FCustomSettingsSystemModule"

DEFINE_LOG_CATEGORY(LogCustomSettingsSystem);

void FCustomSettingsSystemModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "CSSSettings",
			LOCTEXT("RuntimeSettingsName", "Custom Settings System"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Custom Settings Plugin"),
			GetMutableDefault<UCSSSettings>()
		);
	}
}

void FCustomSettingsSystemModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->UnregisterSettings("Project", "VUEDK", "CSSSettings");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomSettingsSystemModule, CustomSettingsSystem)
