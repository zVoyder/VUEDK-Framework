// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCombatAllocationSystem, Log, All);

class FCombatAllocationSystemModule : public IModuleInterface
{
public:
    static TAutoConsoleVariable<bool> CVarDrawDebugCombatSlots;
    
    virtual void StartupModule() override;

    virtual void ShutdownModule() override;
};
