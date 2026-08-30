// Copyright VUEDK, Inc. All Rights Reserved.

#include "BattlePassSystem.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "BattlePassSystemSettings.h"

#define LOCTEXT_NAMESPACE "FBattlePassSystemModule"

DEFINE_LOG_CATEGORY(LogBattlePassSystem);

void FBattlePassSystemModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "Battle Pass System",
		                                LOCTEXT("RuntimeSettingsName", "Battle Pass System"),
		                                LOCTEXT("RuntimeSettingsDescription", "Configure the Battle Pass System Plugin"),
		                                GetMutableDefault<UBattlePassSystemSettings>()
		);
	}
}

void FBattlePassSystemModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		SettingModule->UnregisterSettings("Project", "VUEDK", "Battle Pass System");
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBattlePassSystemModule, BattlePassSystem)
