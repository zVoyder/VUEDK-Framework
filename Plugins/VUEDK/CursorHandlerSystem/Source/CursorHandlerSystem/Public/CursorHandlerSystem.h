// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogCursorHandlerSystem, All, All);

class FCursorHandlerSystemModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    
    virtual void ShutdownModule() override;
};
