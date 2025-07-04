// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/CrosshairsUtility.h"

UCrosshairSubsystem* UCrosshairsUtility::CrosshairSubsystem = nullptr;

void UCrosshairsUtility::Init(UCrosshairSubsystem* InCrosshairSubsystem)
{
	if (!IsValid(InCrosshairSubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("UCrosshairsUtility::Init: Invalid Crosshair Subsystem."));
		return;
	}

	CrosshairSubsystem = InCrosshairSubsystem;
}

void UCrosshairsUtility::Deinit()
{
	CrosshairSubsystem = nullptr;
}

void UCrosshairsUtility::SetDefaultCrosshairInViewport(UObject* Payload)
{
	if (!Check())
		return;

	CrosshairSubsystem->SetDefaultCrosshairInViewport(Payload);
}

void UCrosshairsUtility::SetCrosshairInViewport(const TSubclassOf<UCrosshairWidget> CrosshairWidgetClass, UObject* Payload)
{
	if (!Check())
		return;

	CrosshairSubsystem->SetCrosshairInViewport(CrosshairWidgetClass, Payload);
}

void UCrosshairsUtility::OpenCrosshairInViewport()
{
	if (!Check())
		return;

	CrosshairSubsystem->OpenCrosshairInViewport();
}

void UCrosshairsUtility::CloseCrosshairInViewport(const bool bRemoveFromViewport)
{
	if (!Check())
		return;

	CrosshairSubsystem->CloseCrosshairInViewport(bRemoveFromViewport);
}

void UCrosshairsUtility::RemoveCrosshairFromViewport()
{
	if (!Check())
		return;

	CrosshairSubsystem->RemoveCrosshairFromViewport();
}

UUserWidget* UCrosshairsUtility::GetCurrentCrosshairWidget()
{
	if (!Check())
		return nullptr;

	return CrosshairSubsystem->GetCurrentCrosshairWidget();
}

void UCrosshairsUtility::ShowCrosshair()
{
	if (!Check())
		return;

	CrosshairSubsystem->ShowCrosshair();
}

void UCrosshairsUtility::HideCrosshair()
{
	if (!Check())
		return;

	CrosshairSubsystem->HideCrosshair();
}

bool UCrosshairsUtility::IsSwitchingCrosshairs()
{
	if (!Check())
		return false;

	return CrosshairSubsystem->IsSwitchingCrosshairs();
}

bool UCrosshairsUtility::IsCrosshairVisible()
{
	if (!Check())
		return false;

	return CrosshairSubsystem->IsCrosshairVisible();
}

bool UCrosshairsUtility::Check()
{
	return IsValid(CrosshairSubsystem);
}
