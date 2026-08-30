// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSquadSystem, Log, All);

class FSquadSystemModule : public IModuleInterface
{
public:
	static TAutoConsoleVariable<bool> CVarMemberStateDebug;
	static TAutoConsoleVariable<bool> CVarSquadTraceDebug;
	
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};
