// Copyright VUEDK, Inc. All Rights Reserved.

#include "SquadSystem.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "SquadSystemSettings.h"

#define LOCTEXT_NAMESPACE "FSquadSystemModule"

DEFINE_LOG_CATEGORY(LogSquadSystem);

TAutoConsoleVariable<bool> FSquadSystemModule::CVarMemberStateDebug(
	TEXT("SquadSystem.MemberStateDebug"),
	false,
	TEXT("Enable Squad System Member State debug."),
	ECVF_Default
);

TAutoConsoleVariable<bool> FSquadSystemModule::CVarSquadTraceDebug(
	TEXT("SquadSystem.SquadTraceDebug"),
	false,
	TEXT("Enable Squad System Trace debug. Allowing to see connected lines between squad members."),
	ECVF_Default
);

void FSquadSystemModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "Squad System",
			LOCTEXT("RuntimeSettingsName", "Squad System"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Squad System Plugin"),
			GetMutableDefault<USquadSystemSettings>()
		);
	}
}

void FSquadSystemModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		SettingModule->UnregisterSettings("Project", "VUEDK", "Squad System");
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSquadSystemModule, SquadSystem)