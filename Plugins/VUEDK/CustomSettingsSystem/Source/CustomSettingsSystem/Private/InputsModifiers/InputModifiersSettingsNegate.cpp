// Copyright VUEDK, Inc. All Rights Reserved.

#include "InputsModifiers/InputModifiersSettingsNegate.h"
#include "CustomGameUserSettings.h"
#include "CustomSettingsSystem.h"
#include "Utility/CustomGameUserSettingsUtility.h"

void UInputModifiersSettingsNegate::Init()
{
	Super::Init();
	SetNegateValues();
}

FInputActionValue UInputModifiersSettingsNegate::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	Super::ModifyRaw_Implementation(PlayerInput, CurrentValue, DeltaTime);
	ApplyNegate(CurrentValue);
	return CurrentValue;
}

void UInputModifiersSettingsNegate::ApplyNegate(FInputActionValue& CurrentValue) const
{
	switch (CurrentValue.GetValueType())
	{
	case EInputActionValueType::Axis1D:
		{
			float Value = CurrentValue.Get<float>();
			if (bIsXAxisInverted)
				Value = -Value;
			CurrentValue = Value;
			break;
		}
	case EInputActionValueType::Axis2D:
		{
			FVector2D Value = CurrentValue.Get<FVector2D>();
			if (bIsXAxisInverted)
				Value.X = -Value.X;
			if (bIsYAxisInverted)
				Value.Y = -Value.Y;
			CurrentValue = Value;
			break;
		}
	case EInputActionValueType::Axis3D:
		{
			FVector Value = CurrentValue.Get<FVector>();
			if (bIsXAxisInverted)
				Value.X = -Value.X;
			if (bIsYAxisInverted)
				Value.Y = -Value.Y;
			if (bIsZAxisInverted)
				Value.Z = -Value.Z;
			CurrentValue = Value;
			break;
		}
	default:
		break;
	}
}

void UInputModifiersSettingsNegate::SetNegateValues()
{
	const UCustomGameUserSettings* CustomGameUserSettings = UCustomUserSettingsUtility::GetCustomUserSettings();

	if (!IsValid(CustomGameUserSettings))
	{
		UE_LOG(LogCustomSettingsSystem, Error, TEXT("UInputModifierSettingsDeadZone::SetTresholdValue: CustomGameUserSettings is not valid."));
		return;
	}
	
	if (bCanInvertXAxis)
	{
		float Value = 0.0f;
		CustomGameUserSettings->TryGetCustomOption(XAxisTag, Value);
		bIsXAxisInverted = Value > 0.0f;
	}

	if (bCanInvertYAxis)
	{
		float Value = 0.0f;
		CustomGameUserSettings->TryGetCustomOption(YAxisTag, Value);
		bIsYAxisInverted = Value > 0.0f;
	}

	if (bCanInvertZAxis)
	{
		float Value = 0.0f;
		CustomGameUserSettings->TryGetCustomOption(ZAxisTag, Value);
		bIsZAxisInverted = Value > 0.0f;
	}
}
