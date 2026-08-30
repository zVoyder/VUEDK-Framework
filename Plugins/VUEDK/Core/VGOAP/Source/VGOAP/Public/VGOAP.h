// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVGOAP, Log, All);

class FVGOAPModule : public IModuleInterface
{
public:
	static TAutoConsoleVariable<bool> CVarVGOAPDebug;
	
	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;
};
