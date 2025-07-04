// Copyright VUEDK, Inc. All Rights Reserved.

#include "InputsModifiers/InputModifierSettingsDeadZone.h"
#include "CustomGameUserSettings.h"
#include "CustomSettingsSystem.h"
#include "Utility/CustomGameUserSettingsUtility.h"

void UInputModifierSettingsDeadZone::Init()
{
	Super::Init();
	SetTresholdValue();
}

FInputActionValue UInputModifierSettingsDeadZone::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	Super::ModifyRaw_Implementation(PlayerInput, CurrentValue, DeltaTime);
	ApplyDeadZone(CurrentValue);
	return CurrentValue;
}

void UInputModifierSettingsDeadZone::OnSettingsApplied()
{
	SetTresholdValue();
}

void UInputModifierSettingsDeadZone::ApplyDeadZone(FInputActionValue& CurrentValue) const
{
	switch (CurrentValue.GetValueType())
	{
	case EInputActionValueType::Axis1D:
		{
			float Value = CurrentValue.Get<float>();
			Value = FMath::Abs(Value) < Treshold ? 0.f : Value;
			CurrentValue = Value;
			break;
		}
	case EInputActionValueType::Axis2D:
		{
			const FVector2D Value = CurrentValue.Get<FVector2D>();
			const float Magnitude = Value.Size();

			if (Magnitude < Treshold)
			{
				CurrentValue = FVector2D::ZeroVector;
				return;
			}

			const FVector2D Normalized = Value / Magnitude;
			const float AdjustedMagnitude = (Magnitude - Treshold) / (1.0f - Treshold);
			CurrentValue = Normalized * AdjustedMagnitude;
			break;
		}
	case EInputActionValueType::Axis3D:
		{
			const FVector Value = CurrentValue.Get<FVector>();
			const float Magnitude = Value.Size();

			if (Magnitude < Treshold)
			{
				CurrentValue = FVector::ZeroVector;
				return;
			}

			const FVector Normalized = Value / Magnitude;
			const float AdjustedMagnitude = (Magnitude - Treshold) / (1.0f - Treshold);
			CurrentValue = Normalized * AdjustedMagnitude;
			break;
		}
	default: ;
	}
}

void UInputModifierSettingsDeadZone::SetTresholdValue()
{
	const UCustomGameUserSettings* CustomGameUserSettings = UCustomUserSettingsUtility::GetCustomUserSettings();

	if (!IsValid(CustomGameUserSettings))
	{
		UE_LOG(LogCustomSettingsSystem, Error, TEXT("UInputModifierSettingsDeadZone::SetTresholdValue: CustomGameUserSettings is not valid."));
		return;
	}

	Treshold = DefaultTreshold;
	CustomGameUserSettings->TryGetCustomOption(DeadZoneTag, Treshold);
}
