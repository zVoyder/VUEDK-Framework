// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/LoadingScreenData.h"
#include "UObject/Object.h"
#include "LoadingScreenSystemSettings.generated.h"

UCLASS(Config = LoadingScreenSystemSettings, DefaultConfig, meta = (DisplayName = "Loading Screen System Settings"))
class LOADINGSCREENSYSTEM_API ULoadingScreenSystemSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TSoftObjectPtr<ULoadingScreenData> DefaultLoadingScreen;
	UPROPERTY(Config, EditAnywhere, Category = Settings,
		meta = (ToolTip = "Whether to exclude the starting map from the loading screen system. If true, the loading screen will not be shown when the game is launched and the starting map is loaded."))
	bool bExcludeStartingMap = false;
	UPROPERTY(Config, EditAnywhere, Category = Settings,
		meta = (EditCondition = "!bExcludeStartingMap", ToolTip = "Whether to exclude the starting map from the loading screen system on startup. If true, the loading screen will not be shown when the game is launched and the starting map is loaded, but it will be shown for subsequent loads of the starting map."))
	bool bExcludeStartingMapOnStartup = true;
};
