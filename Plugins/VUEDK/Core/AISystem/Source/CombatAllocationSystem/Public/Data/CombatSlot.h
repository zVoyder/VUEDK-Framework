// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatSlot.generated.h"

USTRUCT(BlueprintType)
struct FCombatSlot
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Location = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AActor> Occupant;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsValid;
	
	FORCEINLINE bool IsAvailable() const
	{
		return bIsValid && !Occupant.IsValid();
	}
};
