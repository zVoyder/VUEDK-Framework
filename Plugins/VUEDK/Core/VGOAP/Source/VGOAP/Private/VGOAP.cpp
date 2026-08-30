// Copyright VUEDK, Inc. All Rights Reserved.

#include "VGOAP.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "VGOAPSettings.h"

#define LOCTEXT_NAMESPACE "FVGOAPModule"

DEFINE_LOG_CATEGORY(LogVGOAP);

TAutoConsoleVariable<bool> FVGOAPModule::CVarVGOAPDebug(
	TEXT("VGOAP.Debug"),
	false,
	TEXT("Enable VGOAP debug."),
	ECVF_Default
);

void FVGOAPModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "VGOAP",
		                                LOCTEXT("RuntimeSettingsName", "VGOAP"),
		                                LOCTEXT("RuntimeSettingsDescription", "Configure the VGOAP Plugin"),
		                                GetMutableDefault<UVGOAPSettings>()
		);
	}
}

void FVGOAPModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->UnregisterSettings("Project", "VUEDK", "VGOAP");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVGOAPModule, VGOAP)
