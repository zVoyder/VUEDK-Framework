// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/CrosshairsUtility.h"

TWeakObjectPtr<UCrosshairSubsystem> UCrosshairsUtility::CrosshairSubsystem = nullptr;

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

void UCrosshairsUtility::SetGeneralCrosshairInViewport(UObject* Payload, const bool bOverrideDefault)
{
	if (!Check())
		return;

	CrosshairSubsystem.Get()->SetGeneralCrosshairInViewport(Payload, bOverrideDefault);
}

void UCrosshairsUtility::SetDefaultCrosshairInViewport(UObject* Payload)
{
	if (!Check())
		return;

	CrosshairSubsystem.Get()->SetDefaultCrosshairInViewport(Payload);
}

void UCrosshairsUtility::SetCrosshairInViewport(const TSubclassOf<UCrosshairWidget> CrosshairWidgetClass, UObject* Payload, const bool bOverrideDefault)
{
	if (!Check())
		return;

	CrosshairSubsystem.Get()->SetCrosshairInViewport(CrosshairWidgetClass, Payload, bOverrideDefault);
}

void UCrosshairsUtility::OpenCrosshairInViewport()
{
	if (!Check())
		return;

	CrosshairSubsystem.Get()->OpenCrosshairInViewport();
}

void UCrosshairsUtility::CloseCrosshairInViewport(const bool bRemoveFromViewport)
{
	if (!Check())
		return;

	CrosshairSubsystem.Get()->CloseCrosshairInViewport(bRemoveFromViewport);
}

void UCrosshairsUtility::RemoveCrosshairFromViewport()
{
	if (!Check())
		return;

	CrosshairSubsystem.Get()->RemoveCrosshairFromViewport();
}

UUserWidget* UCrosshairsUtility::GetCurrentCrosshairWidget()
{
	if (!Check())
		return nullptr;

	return CrosshairSubsystem.Get()->GetCurrentCrosshairWidget();
}

void UCrosshairsUtility::ShowCrosshair()
{
	if (!Check())
		return;

	CrosshairSubsystem.Get()->ShowCrosshair();
}

void UCrosshairsUtility::HideCrosshair()
{
	if (!Check())
		return;

	CrosshairSubsystem.Get()->HideCrosshair();
}

bool UCrosshairsUtility::IsSwitchingCrosshairs()
{
	if (!Check())
		return false;

	return CrosshairSubsystem.Get()->IsSwitchingCrosshairs();
}

bool UCrosshairsUtility::IsCrosshairVisible()
{
	if (!Check())
		return false;

	return CrosshairSubsystem.Get()->IsCrosshairVisible();
}

bool UCrosshairsUtility::Check()
{
	return CrosshairSubsystem.IsValid();
}
