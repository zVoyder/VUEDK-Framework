// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatAllocator.h"
#include "UObject/Object.h"
#include "CombatAllocatorObjectBase.generated.h"

UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class COMBATALLOCATIONSYSTEM_API UCombatAllocatorObjectBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ProviderTag;
	UPROPERTY(BlueprintReadOnly)
	UCombatAllocator* CombatAllocator;

public:
	void Init(UCombatAllocator* InCombatAllocator, const FGameplayTag InProviderTag);

	UFUNCTION(BlueprintPure)
	AActor* GetOwner() const;
	
#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

	virtual UWorld* GetWorld() const override;
	
protected:
	UFUNCTION(BlueprintPure)
	const UCombatSlotsProviderBase* GetCombatSlotsProvider() const;
	
	virtual void OnInit();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnInit")
	void ReceiveOnInit();
};
