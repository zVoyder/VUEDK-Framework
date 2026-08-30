// Copyright VUEDK, Inc. All Rights Reserved.

#include "WeaponSystem.h"

#define LOCTEXT_NAMESPACE "FWeaponSystemModule"

DEFINE_LOG_CATEGORY(LogWeaponSystem);
DEFINE_LOG_CATEGORY(LogShooter);

TAutoConsoleVariable<bool> FWeaponSystemModule::CVarShootTraceDebug(
	TEXT("WeaponSystem.ShootTrace.Debug"),
	false,
	TEXT("Enable debug lines for shoot traces."),
	ECVF_Default
);

TAutoConsoleVariable<float> FWeaponSystemModule::CVarShootTraceDebugDuration(
	TEXT("WeaponSystem.ShootTrace.DebugDuration"),
	5.0f,
	TEXT("Duration for which debug lines for shoot traces are displayed."),
	ECVF_Default
);

TAutoConsoleVariable<bool> FWeaponSystemModule::CVarMeleeTraceDebug(
	TEXT("WeaponSystem.MeleeTrace.Debug"),
	false,
	TEXT("Enable debug lines for melee traces."),
	ECVF_Default
);

TAutoConsoleVariable<float> FWeaponSystemModule::CVarMeleeTraceDebugDuration(
	TEXT("WeaponSystem.MeleeTrace.DebugDuration"),
	5.0f,
	TEXT("Duration for which debug lines for melee traces are displayed."),
	ECVF_Default
);

void FWeaponSystemModule::StartupModule()
{
}

void FWeaponSystemModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWeaponSystemModule, WeaponSystem)
