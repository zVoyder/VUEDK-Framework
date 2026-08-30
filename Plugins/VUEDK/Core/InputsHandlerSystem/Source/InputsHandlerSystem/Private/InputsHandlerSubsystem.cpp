// Copyright VUEDK, Inc. All Rights Reserved.

#include "InputsHandlerSubsystem.h"
#include "InputsHandlerSystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Data/ContextsBehaviour.h"
#include "Utility/InputsHandlerUtility.h"

UInputsHandlerSubsystem::UInputsHandlerSubsystem(): PlayerController(nullptr),
                                                    EnhancedInputSubsystem(nullptr),
                                                    InputDeviceSubsystem(nullptr),
                                                    InputsHandlerSystemSettings(nullptr),
                                                    CurrentInputModeType(),
                                                    bIsUsingGamepad(false)
{
}

void UInputsHandlerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InputsHandlerSystemSettings = GetDefault<UInputsHandlerSystemSettings>();
	LoadMappingContexts();
}

void UInputsHandlerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UInputsHandlerSubsystem::PlayerControllerChanged(APlayerController* NewPlayerController)
{
	Super::PlayerControllerChanged(NewPlayerController);
	UnbindCheckGamepad();
	EnhancedInputSubsystem = nullptr;
	CurrentInputModeType = EInputModeType::InputModeNone;
	
	if (!IsValid(NewPlayerController))
	{
		UE_LOG(LogInputsHandlerSystem, Error, TEXT("InputsHandlerSubsystem::PlayerControllerChanged: NewPlayerController is not valid."));
		return;
	}

	PlayerController = NewPlayerController;
	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(NewPlayerController->Player);

	if (!IsValid(LocalPlayer))
	{
		UE_LOG(LogInputsHandlerSystem, Error, TEXT("InputsHandlerSubsystem::PlayerControllerChanged: LocalPlayer is not valid."));
		return;
	}

	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	InputDeviceSubsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();
	AddCommonContexts();
	BindCheckGamepad();
	UInputsHandlerUtility::Init(this);
}

FVector2D UInputsHandlerSubsystem::GetNormalizedMousePosition() const
{
	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::GetNormalizedMousePositionWorld: is not valid."));
		return FVector2D::ZeroVector;
	}

	FVector2D MousePosition;
	World->GetFirstPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y);
	const FVector2D ViewportHalfSize = UWidgetLayoutLibrary::GetViewportSize(World) / 2.f;

	return (MousePosition - ViewportHalfSize) / ViewportHalfSize;
}

UEnhancedInputUserSettings* UInputsHandlerSubsystem::GetEnhancedInputUserSettings() const
{
	if (!IsValid(EnhancedInputSubsystem))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::GetEnhancedInputUserSettings: EnhancedInputSubsystem is not valid"));
		return nullptr;
	}

	return EnhancedInputSubsystem->GetUserSettings();
}

void UInputsHandlerSubsystem::AddCommonMappingContext(UInputMappingContext* MappingContext)
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::AddCommonMappingContext: Check failed."));
		return;
	}

	if (!IsValid(MappingContext))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::AddCommonMappingContext: MappingContext is not valid."));
		return;
	}

	CommonMappingContexts.AddUnique(MappingContext);
	EnhancedInputSubsystem->AddMappingContext(MappingContext, InputsHandlerSystemSettings->CommonContextsPriority);
}

void UInputsHandlerSubsystem::RemoveCommonMappingContext(UInputMappingContext* MappingContext)
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::RemoveCommonMappingContext: Check failed."));
		return;
	}

	if (!IsValid(MappingContext))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::RemoveCommonMappingContext: MappingContext is not valid."));
		return;
	}

	CommonMappingContexts.Remove(MappingContext);
	EnhancedInputSubsystem->RemoveMappingContext(MappingContext);
}

void UInputsHandlerSubsystem::AddGameplayMappingContext(UInputMappingContext* MappingContext)
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::AddGameplayMappingContext: Check failed."));
		return;
	}

	if (!IsValid(MappingContext))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::AddGameplayMappingContext: MappingContext is not valid."));
		return;
	}

	GameplayMappingContexts.AddUnique(MappingContext);
	if (IsInputModeGameplay())
		EnhancedInputSubsystem->AddMappingContext(MappingContext, InputsHandlerSystemSettings->GameplayContextsPriority);
}

void UInputsHandlerSubsystem::RemoveGameplayMappingContext(UInputMappingContext* MappingContext)
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::RemoveGameplayMappingContext: Check failed."));
		return;
	}

	if (!IsValid(MappingContext))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::RemoveGameplayMappingContext: MappingContext is not valid."));
		return;
	}

	GameplayMappingContexts.Remove(MappingContext);
	if (IsInputModeGameplay())
		EnhancedInputSubsystem->RemoveMappingContext(MappingContext);
}

void UInputsHandlerSubsystem::AddUIMappingContext(UInputMappingContext* MappingContext)
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::AddUIMappingContext: Check failed."));
		return;
	}

	if (!IsValid(MappingContext))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::AddUIMappingContext: MappingContext is not valid."));
		return;
	}

	UIMappingContexts.AddUnique(MappingContext);
	if (!IsInputModeGameplay())
		EnhancedInputSubsystem->AddMappingContext(MappingContext, InputsHandlerSystemSettings->UIContextsPriority);
}

void UInputsHandlerSubsystem::RemoveUIMappingContext(UInputMappingContext* MappingContext)
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::RemoveUIMappingContext: Check failed."));
		return;
	}

	if (!IsValid(MappingContext))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::RemoveUIMappingContext: MappingContext is not valid."));
		return;
	}

	UIMappingContexts.Remove(MappingContext);
	if (IsInputModeUI())
		EnhancedInputSubsystem->RemoveMappingContext(MappingContext);
}

void UInputsHandlerSubsystem::AddExcludedMappingContext(UInputMappingContext* MappingContext)
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::AddExcludedMappingContext: Check failed."));
		return;
	}

	if (!IsValid(MappingContext))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::AddExcludedMappingContext: MappingContext is not valid."));
		return;
	}

	ExcludedContexts.AddUnique(MappingContext);
	if (IsInputModeGameplay())
		EnhancedInputSubsystem->RemoveMappingContext(MappingContext);
}

void UInputsHandlerSubsystem::RemoveExcludedMappingContext(UInputMappingContext* MappingContext)
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::RemoveExcludedMappingContext: Check failed."));
		return;
	}

	if (!IsValid(MappingContext))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::RemoveExcludedMappingContext: MappingContext is not valid."));
		return;
	}

	ExcludedContexts.Remove(MappingContext);
	if (IsInputModeGameplay())
		EnhancedInputSubsystem->AddMappingContext(MappingContext, 0);
}

void UInputsHandlerSubsystem::SetModeGameplay()
{
	if (CurrentInputModeType == EInputModeType::InputModeGameplay)
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::SetGameplayMappingContext: Already in gameplay mode."));
		return;
	}
	
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::SetGameplayMappingContext: Check failed."));
		return;
	}

	RemoveUIContexts();
	AddGameplayContexts();
	CurrentInputModeType = EInputModeType::InputModeGameplay;
	SetInputModeGameplay();
	OnGameplayMappingContextSet.Broadcast();
}

void UInputsHandlerSubsystem::SetModeUI()
{
	if (CurrentInputModeType == EInputModeType::InputModeUI)
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::SetGameplayMappingContext: Already in UI mode."));
		return;
	}

	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::SetUIMappingContext: Check failed."));
		return;
	}

	switch (InputsHandlerSystemSettings->ContextsBehaviour)
	{
	case EContextsBehaviour::Switch:
		{
			AddUIContexts();
			RemoveGameplayContexts();
			break;
		}
	case EContextsBehaviour::Priority:
		{
			AddUIContexts();
			RemoveExcludedContexts();
			break;
		}
	default:
		{
			UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::SetUIMappingContext: Invalid ContextsBehaviour."));
			break;
		}
	}

	CurrentInputModeType = EInputModeType::InputModeUI;
	SetInputModeUI();
	OnUIMappingContextSet.Broadcast();
}

TArray<UInputMappingContext*> UInputsHandlerSubsystem::GetAllMappingContexts() const
{
	TArray<UInputMappingContext*> AllContexts;
	AllContexts.Append(CommonMappingContexts);
	AllContexts.Append(GameplayMappingContexts);
	AllContexts.Append(UIMappingContexts);
	return AllContexts;
}

TArray<UInputMappingContext*> UInputsHandlerSubsystem::GetCommonMappingContexts() const
{
	return CommonMappingContexts;
}

TArray<UInputMappingContext*> UInputsHandlerSubsystem::GetGameplayMappingContexts() const
{
	return GameplayMappingContexts;
}

TArray<UInputMappingContext*> UInputsHandlerSubsystem::GetUIMappingContexts() const
{
	return UIMappingContexts;
}

UInputMappingContext* UInputsHandlerSubsystem::GetGameplayMappingContext(const int32 Index) const
{
	if (!Check() || !GameplayMappingContexts.IsValidIndex(Index))
		return nullptr;

	return GameplayMappingContexts[Index];
}

UInputMappingContext* UInputsHandlerSubsystem::GetUIMappingContext(const int32 Index) const
{
	if (!Check() || !UIMappingContexts.IsValidIndex(Index))
		return nullptr;

	return UIMappingContexts[Index];
}

bool UInputsHandlerSubsystem::IsUsingGamepad() const
{
	return bIsUsingGamepad;
}

bool UInputsHandlerSubsystem::IsInputModeGameplay() const
{
	return CurrentInputModeType == EInputModeType::InputModeGameplay;
}

bool UInputsHandlerSubsystem::IsInputModeUI() const
{
	return CurrentInputModeType == EInputModeType::InputModeUI;
}

void UInputsHandlerSubsystem::SetInputModeGameplay() const
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::SetInputModeGameplay: Check failed."));
		return;
	}

	const FInputModeGameplay& InputModeGameplay = InputsHandlerSystemSettings->InputModeGameplay;
	PlayerController->bShowMouseCursor = InputModeGameplay.bShowMouseCursor;
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(InputModeGameplay.bConsumeCaptureMouseDown);

	if (InputModeGameplay.bFlushInputs)
		PlayerController->FlushPressedKeys();

	PlayerController->SetInputMode(InputMode);
	UWidgetBlueprintLibrary::SetFocusToGameViewport();
}

void UInputsHandlerSubsystem::SetInputModeUI() const
{
	if (!Check())
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("InputsHandlerSubsystem::SetInputModeUI: Check failed."));
		return;
	}

	const FInputModeUI& InputModeUI = InputsHandlerSystemSettings->InputModeUI;
	PlayerController->bShowMouseCursor = InputModeUI.bShowMouseCursor;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(InputModeUI.LockMouseToViewportBehavior);
	InputMode.SetHideCursorDuringCapture(InputModeUI.bHideCursorDuringCapture);
	
	if (InputModeUI.bFlushInputs)
		PlayerController->FlushPressedKeys();

	if (InputModeUI.bCenterMouseOnViewport)
		CenterMouseOnViewport();

	PlayerController->SetInputMode(InputMode);
}

void UInputsHandlerSubsystem::AddCommonContexts() const
{
	for (const auto Context : CommonMappingContexts)
		EnhancedInputSubsystem->AddMappingContext(Context, InputsHandlerSystemSettings->CommonContextsPriority);
}

void UInputsHandlerSubsystem::AddGameplayContexts() const
{
	for (const auto Context : GameplayMappingContexts)
		EnhancedInputSubsystem->AddMappingContext(Context, InputsHandlerSystemSettings->GameplayContextsPriority);
}

void UInputsHandlerSubsystem::RemoveGameplayContexts() const
{
	for (const auto Context : GameplayMappingContexts)
		EnhancedInputSubsystem->RemoveMappingContext(Context);
}

void UInputsHandlerSubsystem::AddUIContexts() const
{
	for (const auto Context : UIMappingContexts)
		EnhancedInputSubsystem->AddMappingContext(Context, InputsHandlerSystemSettings->UIContextsPriority);
}

void UInputsHandlerSubsystem::RemoveUIContexts() const
{
	for (const auto Context : UIMappingContexts)
		EnhancedInputSubsystem->RemoveMappingContext(Context);
}

void UInputsHandlerSubsystem::RemoveExcludedContexts() const
{
	for (const auto Context : ExcludedContexts)
		EnhancedInputSubsystem->RemoveMappingContext(Context);
}

void UInputsHandlerSubsystem::BindCheckGamepad()
{
	if (!IsValid(InputDeviceSubsystem))
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("UInputsHandlerSubsystem::BindCheckGamepad: InputDeviceSubsystem is not valid."));
		return;
	}

	CheckUsingGamepad(PlayerController->GetPlatformUserId(), FInputDeviceId());
	InputDeviceSubsystem->OnInputHardwareDeviceChanged.AddUniqueDynamic(this, &UInputsHandlerSubsystem::CheckUsingGamepad);
}

void UInputsHandlerSubsystem::UnbindCheckGamepad()
{
	if (!IsValid(InputDeviceSubsystem))
		return;

	InputDeviceSubsystem->OnInputHardwareDeviceChanged.RemoveDynamic(this, &UInputsHandlerSubsystem::CheckUsingGamepad);
}

void UInputsHandlerSubsystem::CenterMouseOnViewport() const
{
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const int32 CenterX = ViewportSize.X / 2;
	const int32 CenterY = ViewportSize.Y / 2;
	PlayerController->SetMouseLocation(CenterX, CenterY);
}

void UInputsHandlerSubsystem::LoadMappingContexts()
{
	if (!InputsHandlerSystemSettings)
	{
		UE_LOG(LogInputsHandlerSystem, Warning, TEXT("UInputsHandlerSubsystem::LoadMappingContexts: InputsHandlerSystemSettings is not valid."));
		return;
	}

	for (auto Context : InputsHandlerSystemSettings->CommonContexts)
	{
		UInputMappingContext* CommonContext = Context.LoadSynchronous();
		if (!IsValid(CommonContext))
			continue;

		CommonMappingContexts.Add(CommonContext);
	}

	for (auto Context : InputsHandlerSystemSettings->GameplayContexts)
	{
		UInputMappingContext* GameplayContext = Context.LoadSynchronous();
		if (!IsValid(GameplayContext))
			continue;

		GameplayMappingContexts.Add(GameplayContext);
	}

	for (auto Context : InputsHandlerSystemSettings->UIContexts)
	{
		UInputMappingContext* UIContext = Context.LoadSynchronous();
		if (!IsValid(UIContext))
			continue;

		UIMappingContexts.Add(UIContext);
	}

	for (auto Context : InputsHandlerSystemSettings->ExcludedContexts)
	{
		UInputMappingContext* ExcludeContext = Context.LoadSynchronous();
		if (!IsValid(ExcludeContext) || !GameplayMappingContexts.Contains(ExcludeContext))
			continue;

		ExcludedContexts.Add(ExcludeContext);
	}
}

void UInputsHandlerSubsystem::CheckUsingGamepad(const FPlatformUserId UserId, const FInputDeviceId DeviceId)
{
	if (!Check())
		return;

	bIsUsingGamepad = InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(UserId).PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad;
}

bool UInputsHandlerSubsystem::Check() const
{
	return IsValid(PlayerController) && IsValid(EnhancedInputSubsystem) && IsValid(InputDeviceSubsystem) && IsValid(InputsHandlerSystemSettings);
}
