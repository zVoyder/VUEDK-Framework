// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/InputsHandlerUtility.h"
#include "InputsHandlerSystem.h"

TWeakObjectPtr<UInputsHandlerSubsystem> UInputsHandlerUtility::InputsHandlerSubsystem = nullptr;

void UInputsHandlerUtility::Init(UInputsHandlerSubsystem* InInputsHandlerSubsystem)
{
	if (!IsValid(InInputsHandlerSubsystem))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerUtility::Init: InputsHandlerSubsystem is not valid"));
		return;
	}
	
	InputsHandlerSubsystem = InInputsHandlerSubsystem;
}

UEnhancedInputUserSettings* UInputsHandlerUtility::GetEnhancedInputUserSettings()
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerUtility::GetEnhancedInputUserSettings: InputsHandlerSubsystem is not valid"));
		return nullptr;
	}

	return InputsHandlerSubsystem.Get()->GetEnhancedInputUserSettings();
}

void UInputsHandlerUtility::SetModeGameplay()
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerUtility::SetGameplayMappingContext: InputsHandlerSubsystem is not valid"));
		return;
	}

	InputsHandlerSubsystem.Get()->SetModeGameplay();
}

void UInputsHandlerUtility::SetModeUI()
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerUtility::SetUIMappingContext: InputsHandlerSubsystem is not valid"));
		return;
	}

	InputsHandlerSubsystem.Get()->SetModeUI();
}

bool UInputsHandlerUtility::IsUsingGamepad()
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerUtility::IsUsingGamepad: InputsHandlerSubsystem is not valid"));
		return false;
	}

	return InputsHandlerSubsystem.Get()->IsUsingGamepad();
}

bool UInputsHandlerUtility::Check()
{
	return InputsHandlerSubsystem.IsValid();
}
