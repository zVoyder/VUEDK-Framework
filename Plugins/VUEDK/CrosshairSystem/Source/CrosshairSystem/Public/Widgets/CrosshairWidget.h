// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnStartOpening
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnStartClosing
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCompleteOpening
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnCompltedClosing
);

UCLASS(Abstract)
class CROSSHAIRSYSTEM_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnStartOpening OnStartOpeningEvent;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnStartClosing OnStartClosingEvent;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCompleteOpening OnCompleteOpeningEvent;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnCompltedClosing OnCompleteClosingEvent;

private:
	UPROPERTY()
	UObject* Payload;
	bool bIsOpen;

public:
	void Init(UObject* InPayload = nullptr);

	UFUNCTION(BlueprintCallable)
	void StartOpening();

	UFUNCTION(BlueprintCallable)
	void StartClosing();

	UFUNCTION(BlueprintCallable)
	void CompleteOpening();

	UFUNCTION(BlueprintCallable)
	void CompleteClosing();

	UFUNCTION(BlueprintPure)
	bool IsOpen() const;

protected:
	UFUNCTION(BlueprintPure)
	UObject* GetPayload() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	UFUNCTION(BlueprintNativeEvent)
	void OnStartOpening();

	UFUNCTION(BlueprintNativeEvent)
	void OnStartClosing();

	UFUNCTION(BlueprintNativeEvent)
	void OnCompleteOpening();

	UFUNCTION(BlueprintNativeEvent)
	void OnCompleteClosing();
};
