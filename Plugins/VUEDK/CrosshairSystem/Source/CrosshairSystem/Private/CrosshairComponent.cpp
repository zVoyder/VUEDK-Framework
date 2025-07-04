// Copyright VUEDK, Inc. All Rights Reserved.

#include "CrosshairComponent.h"
#include "Utility/CrosshairsUtility.h"

UCrosshairComponent::UCrosshairComponent(): CrosshairPayload(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCrosshairComponent::SetPayload(UObject* Payload)
{
	CrosshairPayload = Payload;
}

void UCrosshairComponent::SetDefaultCrosshair() const
{
	UCrosshairsUtility::SetDefaultCrosshairInViewport(CrosshairPayload);
}

void UCrosshairComponent::SetCrosshair() const
{
	UCrosshairsUtility::SetCrosshairInViewport(CrosshairWidgetClass, CrosshairPayload);
}

void UCrosshairComponent::ShowCrosshair() const
{
	UCrosshairsUtility::ShowCrosshair();
}

void UCrosshairComponent::HideCrosshair() const
{
	UCrosshairsUtility::HideCrosshair();
}

void UCrosshairComponent::CloseCrosshairInViewport() const
{
	UCrosshairsUtility::CloseCrosshairInViewport(true);
}
