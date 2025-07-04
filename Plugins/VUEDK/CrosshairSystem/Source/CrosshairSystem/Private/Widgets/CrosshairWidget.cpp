// Copyright VUEDK, Inc. All Rights Reserved.

#include "Widgets/CrosshairWidget.h"

void UCrosshairWidget::Init(UObject* InPayload)
{
	Payload = InPayload;
	OnInit();
}

void UCrosshairWidget::StartOpening()
{
	OnStartOpening();
	OnStartOpeningEvent.Broadcast();
}

void UCrosshairWidget::StartClosing()
{
	OnStartClosing();
	OnStartClosingEvent.Broadcast();
}

void UCrosshairWidget::CompleteOpening()
{
	bIsOpen = true;
	OnCompleteOpening();
	OnCompleteOpeningEvent.Broadcast();
}

void UCrosshairWidget::CompleteClosing()
{
	bIsOpen = false;
	OnCompleteClosing();
	OnCompleteClosingEvent.Broadcast();
}

bool UCrosshairWidget::IsOpen() const
{
	return bIsOpen;
}

UObject* UCrosshairWidget::GetPayload() const
{
	return Payload;
}

void UCrosshairWidget::OnStartOpening_Implementation()
{
	CompleteOpening();
}

void UCrosshairWidget::OnStartClosing_Implementation()
{
	CompleteClosing();
}

void UCrosshairWidget::OnInit_Implementation()
{
}

void UCrosshairWidget::OnCompleteClosing_Implementation()
{
}

void UCrosshairWidget::OnCompleteOpening_Implementation()
{
}
