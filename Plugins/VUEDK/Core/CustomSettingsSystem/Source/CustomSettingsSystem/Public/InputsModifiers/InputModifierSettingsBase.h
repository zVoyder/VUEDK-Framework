// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "InputModifierSettingsBase.generated.h"

UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class CUSTOMSETTINGSSYSTEM_API UInputModifierSettingsBase : public UInputModifier
{
	GENERATED_BODY()

private:
	bool bIsInitialized;
	
public:
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

protected:
	/**
	 * @brief Initializes the input modifier settings. Called before use.
	 */
	virtual void Init();
	
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

	/**
	 * @brief Called when the settings are applied to the input modifier.
	 */
	UFUNCTION()
	virtual void OnSettingsApplied();
};
