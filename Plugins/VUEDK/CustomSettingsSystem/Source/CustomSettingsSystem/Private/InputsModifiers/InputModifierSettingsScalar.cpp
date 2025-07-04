// Copyright VUEDK, Inc. All Rights Reserved.

#include "InputsModifiers/InputModifierSettingsScalar.h"
#include "CustomSettingsSystem.h"
#include "Utility/CustomGameUserSettingsUtility.h"

void UInputModifierSettingsScalar::Init()
{
	Super::Init();
	SetScalarValue();
}

FInputActionValue UInputModifierSettingsScalar::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	Super::ModifyRaw_Implementation(PlayerInput, CurrentValue, DeltaTime);
	ApplyScale(CurrentValue);
	return CurrentValue;
}

void UInputModifierSettingsScalar::OnSettingsApplied()
{
	SetScalarValue();
}

void UInputModifierSettingsScalar::SetScalarValue()
{
	const UCustomGameUserSettings* CustomGameUserSettings = UCustomUserSettingsUtility::GetCustomUserSettings();

	if (!IsValid(CustomGameUserSettings))
	{
		UE_LOG(LogCustomSettingsSystem, Error, TEXT("UInputModifierSettingsScalar::SetScalarValue: CustomGameUserSettings is not valid."));
		return;
	}

	float X = DefaultScalar.X;
	float Y = DefaultScalar.Y;
	float Z = DefaultScalar.Z;

	if (bCanScaleXAxis)
		CustomGameUserSettings->TryGetCustomOption(XTag, X);
	if (bCanScaleYAxis)
		CustomGameUserSettings->TryGetCustomOption(YTag, Y);
	if (bCanScaleZAxis)
		CustomGameUserSettings->TryGetCustomOption(ZTag, Z);

	Scalar = FVector(X, Y, Z);
}

void UInputModifierSettingsScalar::ApplyScale(FInputActionValue& CurrentValue) const
{
	switch (CurrentValue.GetValueType())
	{
	case EInputActionValueType::Axis1D:
		{
			CurrentValue = CurrentValue.Get<float>() * Scalar.X * Multiplier.X;
			break;
		}
	case EInputActionValueType::Axis2D:
		{
			FVector2D Vector2D = CurrentValue.Get<FVector2D>();
			Vector2D.X *= Scalar.X * Multiplier.X;
			Vector2D.Y *= Scalar.Y * Multiplier.Y;
			CurrentValue = Vector2D;
			break;
		}
	case EInputActionValueType::Axis3D:
		{
			CurrentValue = CurrentValue.Get<FVector>() * Scalar * Multiplier;
			break;
		}
	default: ;
	}
}
