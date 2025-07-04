// Copyright VUEDK, Inc. All Rights Reserved.

#include "InputsHandlerSystem.h"
#include "InputsHandlerSystemSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FInputsHandlerSystemModule"

void FInputsHandlerSystemModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "VUEDK", "Inputs Handler System",
			LOCTEXT("RuntimeSettingsName", "Inputs Handler System"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Inputs Handler System Plugin"),
			GetMutableDefault<UInputsHandlerSystemSettings>()
		);
	}
}

void FInputsHandlerSystemModule::ShutdownModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->UnregisterSettings("Project", "VUEDK", "Inputs Handler System");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FInputsHandlerSystemModule, InputsHandlerSystem)
