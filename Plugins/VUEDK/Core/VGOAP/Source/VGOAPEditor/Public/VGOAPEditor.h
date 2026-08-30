// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Action.h"
#include "AssetTypeActions_Behaviour.h"
#include "AssetTypeActions_Belief.h"
#include "AssetTypeActions_Blackboard.h"
#include "AssetTypeActions_Goal.h"
#include "AssetTypeActions_Sensor.h"
#include "Modules/ModuleManager.h"

class FVGOAPEditorModule : public IModuleInterface
{

public:
    TSharedPtr<FSlateStyleSet> StyleSet;
    static uint32 GOAPCategory;

private:
    TSharedPtr<FAssetTypeActions_Behaviour> BehaviourAssetTypeAction;
    TSharedPtr<FAssetTypeActions_Blackboard> BlackboardAssetTypeAction;
    TSharedPtr<FAssetTypeActions_Sensor> SensorAssetTypeAction;
    TSharedPtr<FAssetTypeActions_Belief> BeliefAssetTypeAction;
    TSharedPtr<FAssetTypeActions_Action> ActionAssetTypeAction;
    TSharedPtr<FAssetTypeActions_Goal> GoalAssetTypeAction;
    
public:
    virtual void StartupModule() override;
    
    virtual void ShutdownModule() override;

    static TArray<FText>& GetGOAPSubMenus();
};
