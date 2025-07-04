// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StatOperation.generated.h"

UCLASS(Blueprintable, BlueprintType)
class RPGSYSTEM_API UStatOperation : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	UObject* Payload;
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	float ModifierValue;
	
public:
	void Init(UObject* InPayload, const float InModifierValue);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	float GetResultOperation() const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();
};
