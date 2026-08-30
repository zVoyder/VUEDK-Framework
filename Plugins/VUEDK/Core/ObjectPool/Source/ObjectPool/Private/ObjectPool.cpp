// Copyright VUEDK, Inc. All Rights Reserved.

#include "ObjectPool.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "ObjectPoolSettings.h"

#define LOCTEXT_NAMESPACE "FObjectPoolModule"

void FObjectPoolModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "Object Pool",
			LOCTEXT("RuntimeSettingsName", "Object Pool"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Object Pool Plugin"),
			GetMutableDefault<UObjectPoolSettings>()
		);
	}
}

void FObjectPoolModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->UnregisterSettings("Project", "VUEDK", "Object Pool");
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FObjectPoolModule, ObjectPool)