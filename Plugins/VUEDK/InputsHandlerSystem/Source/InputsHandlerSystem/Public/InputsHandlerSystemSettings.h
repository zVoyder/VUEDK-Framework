// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Data/ContextsBehaviour.h"
#include "Data/InputModeGameplay.h"
#include "Data/InputModeUI.h"
#include "UObject/Object.h"
#include "InputsHandlerSystemSettings.generated.h"

UCLASS(Config = InputsHandlerSystemSettings, DefaultConfig, meta = (DisplayName = "Inputs Handler System Settings"))
class INPUTSHANDLERSYSTEM_API UInputsHandlerSystemSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditDefaultsOnly, Category = "Mapping Contexts", meta = (ToolTip = "Input mapping contexts that are always active and shared across all input setups."))
	TSet<TSoftObjectPtr<UInputMappingContext>> CommonContexts;
	UPROPERTY(Config, EditAnywhere, Category = "Mapping Contexts", meta = (ToolTip = "Input mapping contexts that are only active when il gioco è in gameplay mode."))
	TSet<TSoftObjectPtr<UInputMappingContext>> GameplayContexts;
	UPROPERTY(Config, EditAnywhere, Category = "Mapping Contexts", meta = (ToolTip = "Input mapping contexts che sono attivi solo quando il gioco è in UI mode."))
	TSet<TSoftObjectPtr<UInputMappingContext>> UIContexts;
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Behaviour")
	EContextsBehaviour ContextsBehaviour;
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Behaviour", meta = (EditCondition = "ContextsBehaviour == EContextsBehaviour::Priority", ToolTip = "Input mapping contexts that are excluded from gameplay mode when UI mode is active."))
	TSet<TSoftObjectPtr<UInputMappingContext>> ExcludedContexts;
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Priorities")
	int32 CommonContextsPriority = 0;
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Priorities")
	int32 GameplayContextsPriority = 2;
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Priorities")
	int32 UIContextsPriority = 1;
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Input Modes")
	FInputModeGameplay InputModeGameplay;
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Input Modes")
	FInputModeUI InputModeUI;

public:
	explicit UInputsHandlerSystemSettings(const FObjectInitializer& ObjectInitializer);
};
