// Copyright VUEDK, Inc. All Rights Reserved.

#include "Utility/EventsDispatcherUtility.h"
#include "EventsDispatcherSystem.h"

TWeakObjectPtr<UEventsDispatcherSubsystem> UEventsDispatcherUtility::EventsDispatcherSubsystem = nullptr;

void UEventsDispatcherUtility::Init(UEventsDispatcherSubsystem* InEventsDispatcherSubsystem)
{
	EventsDispatcherSubsystem = InEventsDispatcherSubsystem;
}

void UEventsDispatcherUtility::CallEvent(const FGameplayTag EventTag, const bool bUsePayload, const bool bStrictDispatch, UObject* Payload)
{
	if (!Check())
		return;
	
	EventsDispatcherSubsystem.Get()->CallEvent(EventTag, bUsePayload, bStrictDispatch, Payload);
}

void UEventsDispatcherUtility::BindEventTo(const FGameplayTag EventTag, const FEventDelegate& EventDelegate)
{
	if (!Check())
		return;
	
	EventsDispatcherSubsystem.Get()->BindEventTo(EventTag, EventDelegate);
}

void UEventsDispatcherUtility::BindPayloadEventTo(const FGameplayTag EventTag, const FEventDelegatePayload& EventDelegate)
{
	if (!Check())
		return;
	
	EventsDispatcherSubsystem.Get()->BindPayloadEventTo(EventTag, EventDelegate);
}

void UEventsDispatcherUtility::UnbindEventFrom(const FGameplayTag EventTag, const FEventDelegate& EventDelegate)
{
	if (!Check())
		return;
	
	EventsDispatcherSubsystem.Get()->UnbindEventFrom(EventTag, EventDelegate);
}

void UEventsDispatcherUtility::UnbindPayloadEventFrom(const FGameplayTag EventTag, const FEventDelegatePayload& EventDelegate)
{
	if (!Check())
		return;

	EventsDispatcherSubsystem.Get()->UnbindPayloadEventFrom(EventTag, EventDelegate);
}

void UEventsDispatcherUtility::UnbindAllEventsFrom(const FGameplayTag EventTag)
{
	if (!Check())
		return;
	
	EventsDispatcherSubsystem.Get()->UnbindAllEventsFrom(EventTag);
}

void UEventsDispatcherUtility::UnbindAllEvents()
{
	if (!Check())
		return;

	EventsDispatcherSubsystem.Get()->UnbindAllEvents();
}

bool UEventsDispatcherUtility::IsBound(const FGameplayTag EventTag)
{
	if (!Check())
		return false;

	return EventsDispatcherSubsystem.Get()->IsBound(EventTag);
}

bool UEventsDispatcherUtility::Check()
{
	return EventsDispatcherSubsystem.IsValid();
}
