// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventorySystem, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogEquipmentSystem, Log, All);

class FInventorySystemModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;
};
