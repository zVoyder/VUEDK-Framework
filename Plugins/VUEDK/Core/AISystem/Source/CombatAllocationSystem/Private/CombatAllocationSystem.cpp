#include "CombatAllocationSystem.h"

#define LOCTEXT_NAMESPACE "FCombatAllocationSystemModule"

DEFINE_LOG_CATEGORY(LogCombatAllocationSystem);

TAutoConsoleVariable<bool> FCombatAllocationSystemModule::CVarDrawDebugCombatSlots(
	TEXT("CombatAllocationSystem.DrawDebugCombatSlots"),
	false,
	TEXT("Draw debug information for combat slots.\n"),
	ECVF_Default
);

void FCombatAllocationSystemModule::StartupModule()
{
}

void FCombatAllocationSystemModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCombatAllocationSystemModule, CombatAllocationSystem)
