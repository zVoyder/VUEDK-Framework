// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogQuestSystem, Log, All);

class FQuestSystemModule : public IModuleInterface
{
    
public:
    virtual void StartupModule() override;
    
    virtual void ShutdownModule() override;
};
