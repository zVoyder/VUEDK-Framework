// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/InputsHandlerUtility.h"
#include "InputsHandlerSystem.h"

UInputsHandlerSubsystem* UInputsHandlerUtility::InputsHandlerSubsystem = nullptr;

void UInputsHandlerUtility::Init(UInputsHandlerSubsystem* InInputsHandler)
{
	if (!IsValid(InInputsHandler))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerUtility::Init: InputsHandlerSubsystem is not valid"));
		return;
	}
	
	InputsHandlerSubsystem = InInputsHandler;
}

UEnhancedInputUserSettings* UInputsHandlerUtility::GetEnhancedInputUserSettings()
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerUtility::GetEnhancedInputUserSettings: InputsHandlerSubsystem is not valid"));
		return nullptr;
	}

	return InputsHandlerSubsystem->GetEnhancedInputUserSettings();
}

void UInputsHandlerUtility::SetModeGameplay()
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerUtility::SetGameplayMappingContext: InputsHandlerSubsystem is not valid"));
		return;
	}

	InputsHandlerSubsystem->SetModeGameplay();
}

void UInputsHandlerUtility::SetModeUI()
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerUtility::SetUIMappingContext: InputsHandlerSubsystem is not valid"));
		return;
	}

	InputsHandlerSubsystem->SetModeUI();
}

bool UInputsHandlerUtility::IsUsingGamepad()
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerUtility::IsUsingGamepad: InputsHandlerSubsystem is not valid"));
		return false;
	}

	return InputsHandlerSubsystem->IsUsingGamepad();
}

bool UInputsHandlerUtility::Check()
{
	return IsValid(InputsHandlerSubsystem);
}
