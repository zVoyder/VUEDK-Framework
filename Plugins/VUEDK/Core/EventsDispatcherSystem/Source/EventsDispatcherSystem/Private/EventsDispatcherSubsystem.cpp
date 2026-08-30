// Copyright VUEDK, Inc. All Rights Reserved.

#include "EventsDispatcherSubsystem.h"
#include "Utility/EventsDispatcherUtility.h"

void UEventsDispatcherSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UEventsDispatcherUtility::Init(this);
}

void UEventsDispatcherSubsystem::Deinitialize()
{
	Super::Deinitialize();
	UnbindAllEvents();
}

void UEventsDispatcherSubsystem::CallEvent(const FGameplayTag& EventTag, const bool bUsePayload, const bool bStrictDispatch, UObject* Payload)
{
	if (!EventTag.IsValid() ||
		(bUsePayload && Payload == nullptr) ||
		EventsMap.IsEmpty() ||
		!EventsMap.Contains(EventTag))
		return;

	if (const FEventEntry* Entry = EventsMap.Find(EventTag)) // Safer access to the entry
		Entry->Broadcast(bUsePayload, bStrictDispatch, Payload);
}

void UEventsDispatcherSubsystem::BindEventTo(const FGameplayTag& EventTag, const FEventDelegate& EventDelegate)
{
	if (!EventTag.IsValid())
		return;
	
	EventsMap.FindOrAdd(EventTag, FEventEntry()).AddUnique(EventDelegate);
}

void UEventsDispatcherSubsystem::BindPayloadEventTo(const FGameplayTag& EventTag, const FEventDelegatePayload& EventDelegate)
{
	if (!EventTag.IsValid())
		return;
	
	EventsMap.FindOrAdd(EventTag, FEventEntry()).AddUnique(EventDelegate);
}

void UEventsDispatcherSubsystem::UnbindEventFrom(const FGameplayTag& EventTag, const FEventDelegate& EventDelegate)
{
	if (!EventTag.IsValid())
		return;
	
	if (FEventEntry* Entry = EventsMap.Find(EventTag))
	{
		Entry->Remove(EventDelegate);
		if (!Entry->IsBound())
			EventsMap.Remove(EventTag);
	}
}

void UEventsDispatcherSubsystem::UnbindPayloadEventFrom(const FGameplayTag& EventTag, const FEventDelegatePayload& EventDelegate)
{
	if (!EventTag.IsValid())
		return;
	
	if (FEventEntry* Entry = EventsMap.Find(EventTag))
	{
		Entry->Remove(EventDelegate);
		if (!Entry->IsBound())
			EventsMap.Remove(EventTag);
	}
}

void UEventsDispatcherSubsystem::UnbindAllEventsFrom(const FGameplayTag& EventTag)
{
	if (!EventTag.IsValid())
		return;
	
	if (EventsMap.Contains(EventTag))
		EventsMap.Remove(EventTag);
}

void UEventsDispatcherSubsystem::UnbindAllEvents()
{
	EventsMap.Empty();
}

bool UEventsDispatcherSubsystem::IsBound(const FGameplayTag& EventTag) const
{
	return EventsMap.Contains(EventTag); // No need to check the entry's IsBound since we remove empty entries
}
