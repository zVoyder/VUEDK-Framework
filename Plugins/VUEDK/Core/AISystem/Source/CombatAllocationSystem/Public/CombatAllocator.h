// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Data/CombatSlot.h"
#include "CombatAllocator.generated.h"

class UCombatSlotsProviderBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMBATALLOCATIONSYSTEM_API UCombatAllocator : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TMap<FGameplayTag, UCombatSlotsProviderBase*> SlotsProviders;

public:
	UCombatAllocator();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	bool TryGetSlot(const FGameplayTag ProviderTag, const AActor* Requester, FCombatSlot& OutSlot);

	UFUNCTION(BlueprintPure)
	bool TryGetSlotAt(const FGameplayTag ProviderTag, const int32 SlotIndex, FCombatSlot& OutSlot);
	
	UFUNCTION(BlueprintCallable)
	bool ClaimSlot(const FGameplayTag ProviderTag, AActor* Requester, int32& OutSlotIndex);

	UFUNCTION(BlueprintCallable)
	bool ClaimSlotAt(const FGameplayTag ProviderTag, AActor* Requester, const int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	bool ReleaseSlot(const FGameplayTag ProviderTag, const AActor* Requester);

	UFUNCTION(BlueprintCallable)
	bool ReleaseSlotAt(const FGameplayTag ProviderTag, const int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void ReleaseAllSlots(const FGameplayTag ProviderTag);

	UFUNCTION(BlueprintCallable)
	void ReleaseSlotFromAllProviders(const AActor* Requester);

	UFUNCTION(BlueprintCallable)
	bool HasSlot(const FGameplayTag ProviderTag, const AActor* Requester, int32& OutSlotIndex) const;

	UFUNCTION(BlueprintPure)
	bool HasAvailableSlots(const FGameplayTag ProviderTag) const;

	UFUNCTION(BlueprintPure)
	bool FindUsingProviders(const AActor* Requester, TArray<FGameplayTag>& OutProviderTags) const;

protected:
	virtual void BeginPlay() override;

private:
	void InitProviders();

	void TickProviders(const float DeltaTime);
};
