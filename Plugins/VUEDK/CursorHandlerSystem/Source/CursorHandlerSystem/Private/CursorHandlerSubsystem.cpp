// Copyright VUEDK, Inc. All Rights Reserved.

#include "CursorHandlerSubsystem.h"
#include "CursorHandlerSystem.h"

void UCursorHandlerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CursorHandlerSystemSettings = GetDefault<UCursorHandlerSystemSettings>();
	InputDeviceSubsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();
	bIsTickEnabled = true;
}

void UCursorHandlerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	UnbindCheckGamepad();
	bIsTickEnabled = false;
}

void UCursorHandlerSubsystem::PlayerControllerChanged(APlayerController* NewPlayerController)
{
	Super::PlayerControllerChanged(NewPlayerController);
	UnbindCheckGamepad();
	
	if (!IsValid(NewPlayerController))
	{
		UE_LOG(LogCursorHandlerSystem, Warning, TEXT("CursorHandlerSubsystem::PlayerControllerChanged: NewPlayerController is not valid."));
		return;
	}
	
	PlayerController = NewPlayerController;
	CreateCursorsWidgets();
	BindCheckGamepad();
	
	if (bIsUsingGamepad)
		SetGamepadCursorWidget();
	else
		SetMouseCursorWidget();
}

void UCursorHandlerSubsystem::Tick(float DeltaTime)
{
	MoveCursorWithAnalog(DeltaTime);
}

bool UCursorHandlerSubsystem::IsTickable() const
{
	return bIsTickEnabled && bIsUsingGamepad && Check();
}

TStatId UCursorHandlerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCursorHandlerSubsystem, STATGROUP_Tickables);
}

void UCursorHandlerSubsystem::MoveCursorWithAnalog(const float DeltaTime) const
{
	FVector2D AnalogValue;
	PlayerController->GetInputAnalogStickState(CursorHandlerSystemSettings->CursorStick, AnalogValue.X, AnalogValue.Y);
	AnalogValue.Y = CursorHandlerSystemSettings->CursorStick == EControllerAnalogStick::CAS_LeftStick ? -AnalogValue.Y : AnalogValue.Y;
	AnalogValue *= CursorHandlerSystemSettings->StickCursorSensitivity * SensScale * DeltaTime;
	FVector2D MousePosition;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	const FVector2D TargetValue = AnalogValue + MousePosition;
	PlayerController->SetMouseLocation(FMath::RoundToInt32(TargetValue.X), FMath::RoundToInt32(TargetValue.Y));
}

void UCursorHandlerSubsystem::BindCheckGamepad()
{
	if (!IsValid(InputDeviceSubsystem))
	{
		UE_LOG(LogCursorHandlerSystem, Warning, TEXT("CursorHandlerSubsystem::BindCheckGamepad: InputDeviceSubsystem is not valid"));
		return;
	}
	
	CheckUsingGamepad(PlayerController->GetPlatformUserId(), FInputDeviceId());
	InputDeviceSubsystem->OnInputHardwareDeviceChanged.AddUniqueDynamic(this, &UCursorHandlerSubsystem::CheckUsingGamepad);
}

void UCursorHandlerSubsystem::UnbindCheckGamepad()
{
	if (!IsValid(InputDeviceSubsystem))
	{
		UE_LOG(LogCursorHandlerSystem, Warning, TEXT("CursorHandlerSubsystem::UnbindCheckGamepad: InputDeviceSubsystem is not valid"));
		return;
	}

	InputDeviceSubsystem->OnInputHardwareDeviceChanged.RemoveDynamic(this, &UCursorHandlerSubsystem::CheckUsingGamepad);
}

void UCursorHandlerSubsystem::CreateCursorsWidgets()
{
	if (!IsValid(CursorHandlerSystemSettings))
	{
		UE_LOG(LogCursorHandlerSystem, Warning, TEXT("CursorHandlerSubsystem::CreateCursorsWidgets: CursorHandlerSystemSettings is not valid"));
		return;
	}

	if (!IsValid(GamepadCursorWidget))
	{
		const TSubclassOf<UUserWidget> GamepadCursorClass = CursorHandlerSystemSettings->GetGamepadCursorClass();
		if (IsValid(GamepadCursorClass))
			GamepadCursorWidget = CreateWidget<UUserWidget>(PlayerController, GamepadCursorClass);
	}

	if (!IsValid(MouseCursorWidget))
	{
		const TSubclassOf<UUserWidget> MouseCursorClass = CursorHandlerSystemSettings->GetMouseCursorClass();
		if (IsValid(MouseCursorClass))
			MouseCursorWidget = CreateWidget<UUserWidget>(PlayerController, MouseCursorClass);
	}
}

void UCursorHandlerSubsystem::CheckUsingGamepad(const FPlatformUserId UserId, const FInputDeviceId DeviceId)
{
	if (!IsValid(InputDeviceSubsystem))
	{
		UE_LOG(LogCursorHandlerSystem, Warning, TEXT("CursorHandlerSubsystem::CheckUsingGamepad: InputDeviceSubsystem is not valid."));
		return;
	}

	bIsUsingGamepad = InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(UserId).PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad;

	if (bIsUsingGamepad)
		SetGamepadCursorWidget();
	else
		SetMouseCursorWidget();
}

void UCursorHandlerSubsystem::SetGamepadCursorWidget() const
{
	PlayerController->SetMouseCursorWidget(EMouseCursor::Type::Default, GamepadCursorWidget);
}

void UCursorHandlerSubsystem::SetMouseCursorWidget() const
{
	PlayerController->SetMouseCursorWidget(EMouseCursor::Type::Default, MouseCursorWidget);
}

bool UCursorHandlerSubsystem::Check() const
{
	return IsValid(PlayerController) && IsValid(InputDeviceSubsystem) && IsValid(CursorHandlerSystemSettings);
}
