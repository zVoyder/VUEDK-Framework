// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/MenuBehaviour.h"
#include "UObject/Object.h"
#include "BetterUISettings.generated.h"

UCLASS(Config = BetterUISettings, DefaultConfig, meta = (DisplayName = "BetterUI Settings"))
class BETTERUI_API UBetterUISettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Menus Settings")
	EMenuBehaviour MenuBehaviour;
	
	UPROPERTY(Config, EditAnywhere, Category = "UI Navigation")
	bool bTabNavigation;
	UPROPERTY(Config, EditAnywhere, Category = "UI Navigation")
	bool bAnalogNavigation;
	UPROPERTY(Config, EditAnywhere, Category = "UI Navigation")
	bool bKeyNavigation;

public:
	explicit UBetterUISettings(const FObjectInitializer& ObjectInitializer);
};
