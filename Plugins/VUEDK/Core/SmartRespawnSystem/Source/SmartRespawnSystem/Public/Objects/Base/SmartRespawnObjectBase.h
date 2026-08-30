// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SmartRespawnObjectBase.generated.h"

class USmartRespawnSubsystem;

UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class SMARTRESPAWNSYSTEM_API USmartRespawnObjectBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	USmartRespawnSubsystem* SmartRespawnSubsystem;

public:
	void Init(USmartRespawnSubsystem* InSmartRespawnSubsystem);

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif
	
	virtual UWorld* GetWorld() const override;
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();
};
