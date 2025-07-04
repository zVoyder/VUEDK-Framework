// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogInputsHandlerSystem, All, All);

class FInputsHandlerSystemModule : public IModuleInterface
{
    
public:
    virtual void StartupModule() override;
    
    virtual void ShutdownModule() override;
};
