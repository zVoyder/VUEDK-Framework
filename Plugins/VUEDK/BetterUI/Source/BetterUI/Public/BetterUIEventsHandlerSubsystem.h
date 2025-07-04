// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BetterUIEventsHandlerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FRequestBack
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FRequestConfirm
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FRequestRightSelect
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FRequestLeftSelect
);

UCLASS(NotBlueprintable, BlueprintType)
class BETTERUI_API UBetterUIEventsHandlerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FRequestBack OnRequestBack;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FRequestConfirm OnRequestConfirm;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FRequestRightSelect OnRequestRightSelect;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FRequestLeftSelect OnRequestLeftSelect;

public:
	UFUNCTION(BlueprintCallable, Category = "Better UI|Events")
	void RequestBack() const;
	
	UFUNCTION(BlueprintCallable, Category = "Better UI|Events")
	void RequestConfirm() const;
	
	UFUNCTION(BlueprintCallable, Category = "Better UI|Events")
	void RequestRightSelect() const;
	
	UFUNCTION(BlueprintCallable, Category = "Better UI|Events")
	void RequestLeftSelect() const;
};
