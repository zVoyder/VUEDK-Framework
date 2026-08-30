// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BetterSettingsPanel.generated.h"

UINTERFACE(Blueprintable, BlueprintType)
class UBetterSettingsPanel : public UInterface
{
	GENERATED_BODY()
};

class BETTERUI_API IBetterSettingsPanel
{
	GENERATED_BODY()

public:
	/**
	 * @brief Resets all settings in the panel to their default values.
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Better Settings Panel")
	void ResetSettingsToDefaults();
};
