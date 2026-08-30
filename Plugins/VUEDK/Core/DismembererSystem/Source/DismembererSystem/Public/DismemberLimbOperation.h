// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DismemberedLimb.h"
#include "UObject/Object.h"
#include "DismemberLimbOperation.generated.h"

UCLASS(Blueprintable, BlueprintType)
class DISMEMBERERSYSTEM_API UDismemberLimbOperation : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	UDismemberer* Dismemberer;
	
public:
	void Init(UDismemberer* InDismemberer);
	
	UFUNCTION(BlueprintNativeEvent)
	ADismemberedLimb* SpawnLimbActor(TSubclassOf<ADismemberedLimb> DismemberedLimbClass);

	virtual UWorld* GetWorld() const override;
	
#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif
};
