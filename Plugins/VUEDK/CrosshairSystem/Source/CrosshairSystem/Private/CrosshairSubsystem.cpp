// Copyright VUEDK, Inc. All Rights Reserved.

#include "CrosshairSubsystem.h"
#include "CrosshairSystem.h"
#include "CrosshairSystemSettings.h"
#include "Factories/CrosshairsFactory.h"
#include "Utility/CrosshairsUtility.h"

void UCrosshairSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Settings = GetDefault<UCrosshairSystemSettings>();

	if (!IsValid(Settings))
	{
		UE_LOG(LogCrosshairSystem, Error, TEXT("UCrosshairSubsystem::PlayerControllerChanged: Invalid Crosshair System Settings."));
		return;
	}

	DefaultCrosshairWidgetClass = GetDefault<UCrosshairSystemSettings>()->GetDefaultCrosshairClass();
	UCrosshairsUtility::Init(this);
}

void UCrosshairSubsystem::Deinitialize()
{
	Super::Deinitialize();
	UCrosshairsUtility::Deinit();
}

void UCrosshairSubsystem::PlayerControllerChanged(APlayerController* NewPlayerController)
{
	Super::PlayerControllerChanged(NewPlayerController);
	CurrentCrosshairWidget = nullptr;
	CurrentCrosshairWidgetClass = nullptr;
}

void UCrosshairSubsystem::SetDefaultCrosshairInViewport(UObject* Payload)
{
	if (!IsValid(DefaultCrosshairWidgetClass))
	{
		UE_LOG(LogCrosshairSystem, Warning, TEXT("UCrosshairSubsystem::SetDefaultCrosshairInViewport: Default Crosshair Widget Class is not valid."));
		return;
	}
	
	SetCrosshairInViewport(DefaultCrosshairWidgetClass, Payload);
}

void UCrosshairSubsystem::SetCrosshairInViewport(const TSubclassOf<UCrosshairWidget> CrosshairWidgetClass, UObject* Payload)
{
	if (!IsValid(CrosshairWidgetClass))
	{
		SetDefaultCrosshairInViewport();
		return;
	}
	
	if (CrosshairWidgetClass == CurrentCrosshairWidgetClass)
		return;

	CurrentCrosshairWidgetClass = CrosshairWidgetClass;
	CurrentCrosshairPayload = Payload;

	bIsSwitchingCrosshairs = true;
	if (IsValid(CurrentCrosshairWidget))
	{
		if (CurrentCrosshairWidget->IsOpen())
		{
			CurrentCrosshairWidget->OnCompleteClosingEvent.AddUniqueDynamic(this, &UCrosshairSubsystem::OnSwitchCrosshairs);
			CurrentCrosshairWidget->StartClosing();
		}
		else
		{
			if (CurrentCrosshairWidget->GetClass() == CurrentCrosshairWidgetClass)
			{
				CurrentCrosshairWidget->OnCompleteOpeningEvent.AddUniqueDynamic(this, &UCrosshairSubsystem::OnCompleteSwitchCrosshairs);
				OpenCrosshairInViewport();
			}
			else
			{
				CurrentCrosshairWidget->RemoveFromParent();
				CreateAndOpen();
				CurrentCrosshairWidget->OnCompleteOpeningEvent.AddUniqueDynamic(this, &UCrosshairSubsystem::OnCompleteSwitchCrosshairs);
			}
		}

		return;
	}

	CreateAndOpen();
}

void UCrosshairSubsystem::OpenCrosshairInViewport() const
{
	if (!IsValid(CurrentCrosshairWidget) || CurrentCrosshairWidget->IsOpen())
		return;

	CurrentCrosshairWidget->StartOpening();
}

void UCrosshairSubsystem::CloseCrosshairInViewport(const bool bRemoveFromViewport)
{
	if (!IsValid(CurrentCrosshairWidget) || !CurrentCrosshairWidget->IsOpen())
		return;

	if (bRemoveFromViewport)
		CurrentCrosshairWidget->OnCompleteClosingEvent.AddUniqueDynamic(this, &UCrosshairSubsystem::RemoveCrosshairFromViewport);

	CurrentCrosshairWidget->StartClosing();
}

void UCrosshairSubsystem::RemoveCrosshairFromViewport()
{
	if (!IsValid(CurrentCrosshairWidget))
		return;

	CurrentCrosshairWidget->OnCompleteClosingEvent.RemoveDynamic(this, &UCrosshairSubsystem::RemoveCrosshairFromViewport);
	CurrentCrosshairWidget->RemoveFromParent();
	CurrentCrosshairWidget = nullptr;
	CurrentCrosshairPayload = nullptr;
	CurrentCrosshairWidgetClass = nullptr;
}

UUserWidget* UCrosshairSubsystem::GetCurrentCrosshairWidget() const
{
	return CurrentCrosshairWidget;
}

void UCrosshairSubsystem::ShowCrosshair() const
{
	if (!IsValid(CurrentCrosshairWidget))
		return;

	CurrentCrosshairWidget->SetVisibility(ESlateVisibility::Visible);
}

void UCrosshairSubsystem::HideCrosshair() const
{
	if (!IsValid(CurrentCrosshairWidget))
		return;

	CurrentCrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
}

bool UCrosshairSubsystem::IsSwitchingCrosshairs() const
{
	return bIsSwitchingCrosshairs;
}

bool UCrosshairSubsystem::IsCrosshairVisible() const
{
	if (!IsValid(CurrentCrosshairWidget))
		return false;

	return CurrentCrosshairWidget->IsVisible();
}

void UCrosshairSubsystem::CreateAndOpen()
{
	CurrentCrosshairWidget = UCrosshairsFactory::CreateCrosshairWidget(GetWorld(), CurrentCrosshairWidgetClass, CurrentCrosshairPayload);
	OpenCrosshairInViewport();
}

void UCrosshairSubsystem::OnSwitchCrosshairs()
{
	CurrentCrosshairWidget->OnCompleteClosingEvent.RemoveDynamic(this, &UCrosshairSubsystem::OnSwitchCrosshairs);
	CurrentCrosshairWidget->OnCompleteOpeningEvent.AddUniqueDynamic(this, &UCrosshairSubsystem::OnCompleteSwitchCrosshairs);
	CurrentCrosshairWidget->RemoveFromParent();
	CreateAndOpen();
}

void UCrosshairSubsystem::OnCompleteSwitchCrosshairs()
{
	CurrentCrosshairWidget->OnCompleteOpeningEvent.RemoveDynamic(this, &UCrosshairSubsystem::OnCompleteSwitchCrosshairs);
	bIsSwitchingCrosshairs = false;
}
