// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWeaponSystem, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogShooter, All, All);

class FWeaponSystemModule : public IModuleInterface
{
public:
	static TAutoConsoleVariable<bool> CVarShootTraceDebug;
	static TAutoConsoleVariable<float> CVarShootTraceDebugDuration;
	static TAutoConsoleVariable<bool> CVarMeleeTraceDebug;
	static TAutoConsoleVariable<float> CVarMeleeTraceDebugDuration;
	
	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;
};
