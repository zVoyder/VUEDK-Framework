// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogCustomSettingsSystem, All, All);

class FCustomSettingsSystemModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;
};
