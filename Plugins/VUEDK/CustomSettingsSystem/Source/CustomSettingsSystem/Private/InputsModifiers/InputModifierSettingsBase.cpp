// Copyright VUEDK, Inc. All Rights Reserved.

#include "InputsModifiers/InputModifierSettingsBase.h"

#include "CustomGameUserSettings.h"
#include "CustomSettingsSystem.h"
#include "Utility/CustomGameUserSettingsUtility.h"

#if WITH_EDITOR
EDataValidationResult UInputModifierSettingsBase::IsDataValid(class FDataValidationContext& Context) const
{
	return EDataValidationResult::Valid;
}
#endif

void UInputModifierSettingsBase::Init()
{
	UCustomGameUserSettings* CustomGameUserSettings = UCustomUserSettingsUtility::GetCustomUserSettings();

	if (!IsValid(CustomGameUserSettings))
	{
		UE_LOG(LogCustomSettingsSystem, Error, TEXT("UInputModifierSettingsBase::Init: CustomGameUserSettings is not valid."));
		return;
	}
	
	CustomGameUserSettings->OnCustomSettingsApplied.AddDynamic(this, &UInputModifierSettingsBase::OnSettingsApplied);
}

FInputActionValue UInputModifierSettingsBase::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	if (!bIsInitialized)
	{
		Init();
		bIsInitialized = true;
	}
	
	return CurrentValue;
}

void UInputModifierSettingsBase::OnSettingsApplied()
{
}
