// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EventsDispatcherSubsystem.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EventsDispatcherUtility.generated.h"

UCLASS()
class EVENTSDISPATCHERSYSTEM_API UEventsDispatcherUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static TWeakObjectPtr<UEventsDispatcherSubsystem> EventsDispatcherSubsystem;

public:
	static void Init(UEventsDispatcherSubsystem* InEventsDispatcherSubsystem);

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static void CallEvent(const FGameplayTag EventTag, const bool bUsePayload = false, const bool bStrictDispatch = false, UObject* Payload = nullptr);

	/**
	 * @brief Binds a delegate to an event by its tag.
	 * @param EventTag Tag of the event to bind to.
	 * @param EventDelegate Delegate to bind to the event.
	 */
	UFUNCTION(BlueprintCallable, Category = "Events Dispatcher")
	static void BindEventTo(const FGameplayTag EventTag, const FEventDelegate& EventDelegate);

	/**
	 * @brief Binds a payload delegate to an event by its tag.
	 * @param EventTag Tag of the event to bind to.
	 * @param EventDelegate Payload delegate to bind to the event.
	 */
	UFUNCTION(BlueprintCallable, Category = "Events Dispatcher")
	static void BindPayloadEventTo(const FGameplayTag EventTag, const FEventDelegatePayload& EventDelegate);

	/**
	 * @brief Unbinds a delegate from an event by its tag.
	 * @param EventTag Tag of the event to unbind from.
	 * @param EventDelegate Delegate to unbind from the event.
	 */
	UFUNCTION(BlueprintCallable, Category = "Events Dispatcher")
	static void UnbindEventFrom(const FGameplayTag EventTag, const FEventDelegate& EventDelegate);

	/**
	 * @brief Unbinds a payload delegate from an event by its tag.
	 * @param EventTag Tag of the event to unbind from.
	 * @param EventDelegate Payload delegate to unbind from the event.
	 */
	UFUNCTION(BlueprintCallable, Category = "Events Dispatcher")
	static void UnbindPayloadEventFrom(const FGameplayTag EventTag, const FEventDelegatePayload& EventDelegate);

	/**
	 * @brief Unbinds all delegates from an event by its tag.
	 * @param EventTag Tag of the event to unbind all delegates from.
	 */
	UFUNCTION(BlueprintCallable, Category = "Events Dispatcher")
	static void UnbindAllEventsFrom(const FGameplayTag EventTag);

	/**
	 * @brief Unbinds all delegates from all events.
	 */
	UFUNCTION(BlueprintCallable, Category = "Events Dispatcher")
	static void UnbindAllEvents();

	UFUNCTION(BlueprintPure, Category = "Events Dispatcher")
	static bool IsBound(const FGameplayTag EventTag);
	
private:
	static bool Check();
};
