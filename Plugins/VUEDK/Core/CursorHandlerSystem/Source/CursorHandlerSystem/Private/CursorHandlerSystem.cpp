// Copyright VUEDK, Inc. All Rights Reserved.

#include "CursorHandlerSystem.h"
#include "CursorHandlerSystemSettings.h"
#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FCursorHandlerSystemModule"

DEFINE_LOG_CATEGORY(LogCursorHandlerSystem);

void FCursorHandlerSystemModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "Cursor Handler System",
			LOCTEXT("RuntimeSettingsName", "Cursor Handler System"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Cursor Handler System Plugin"),
			GetMutableDefault<UCursorHandlerSystemSettings>()
		);
	}
}

void FCursorHandlerSystemModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->UnregisterSettings("Project", "VUEDK", "Cursor Handler System");
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FCursorHandlerSystemModule, CursorHandlerSystem)