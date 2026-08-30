// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSimplePerceptionSystem, Log, All);

class FSimplePerceptionSystemModule : public IModuleInterface
{
public:
    static TAutoConsoleVariable<bool> CVarDebugPerceptionSystem;
    
    virtual void StartupModule() override;
    
    virtual void ShutdownModule() override;
};
