// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "EventsDispatcherSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE(
	FEventDelegate
);

DECLARE_DYNAMIC_DELEGATE_OneParam(
	FEventDelegatePayload,
	UObject*, Payload
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FEventMulticastDelegate
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FEventMulticastDelegateObject,
	UObject*, Object
);

USTRUCT()
struct FEventEntry
{
	GENERATED_BODY()

	FEventMulticastDelegate Events;
	FEventMulticastDelegateObject ObjectEvents;

	FORCEINLINE void Broadcast(const bool bUsePayload, const bool bStrictDispatch, UObject* Payload) const
	{
		if (bStrictDispatch)
		{
			if (bUsePayload)
				ObjectEvents.Broadcast(Payload);
			else
				Events.Broadcast();

			return;
		}

		Events.Broadcast();
		ObjectEvents.Broadcast(Payload);
	}

	FORCEINLINE void AddUnique(const FEventDelegate& Delegate)
	{
		Events.AddUnique(Delegate);
	}

	FORCEINLINE void AddUnique(const FEventDelegatePayload& Delegate)
	{
		ObjectEvents.AddUnique(Delegate);
	}

	FORCEINLINE void Remove(const FEventDelegate& Delegate)
	{
		Events.Remove(Delegate);
	}

	FORCEINLINE void Remove(const FEventDelegatePayload& Delegate)
	{
		ObjectEvents.Remove(Delegate);
	}

	FORCEINLINE bool IsBound() const
	{
		return Events.IsBound() || ObjectEvents.IsBound();
	}
	
	FORCEINLINE void Clear()
	{
		Events.Clear();
		ObjectEvents.Clear();
	}
};

UCLASS()
class EVENTSDISPATCHERSYSTEM_API UEventsDispatcherSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	TMap<FGameplayTag, FEventEntry> EventsMap;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	void CallEvent(const FGameplayTag& EventTag, const bool bUsePayload = false, const bool bStrictDispatch = false, UObject* Payload = nullptr);

	void BindEventTo(const FGameplayTag& EventTag, const FEventDelegate& EventDelegate);

	void BindPayloadEventTo(const FGameplayTag& EventTag, const FEventDelegatePayload& EventDelegate);

	void UnbindEventFrom(const FGameplayTag& EventTag, const FEventDelegate& EventDelegate);

	void UnbindPayloadEventFrom(const FGameplayTag& EventTag, const FEventDelegatePayload& EventDelegate);

	void UnbindAllEventsFrom(const FGameplayTag& EventTag);

	void UnbindAllEvents();

	bool IsBound(const FGameplayTag& EventTag) const;
};
