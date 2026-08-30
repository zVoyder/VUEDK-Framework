// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimplePerceiverObjectBase.generated.h"

class USimplePerceiver;

UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class SIMPLEPERCEPTIONSYSTEM_API USimplePerceiverObjectBase : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	USimplePerceiver* SimplePerceiver;
	UPROPERTY()
	AActor* Owner;

public:
	virtual void Init(USimplePerceiver* InSimplePerceiver);

	UFUNCTION(BlueprintPure)
	USimplePerceiver* GetPerceiver() const;

	UFUNCTION(BlueprintPure)
	AActor* GetOwner() const;

	UFUNCTION(BlueprintPure)
	FVector GetSightLocation() const;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

	virtual UWorld* GetWorld() const override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnInit();
};
