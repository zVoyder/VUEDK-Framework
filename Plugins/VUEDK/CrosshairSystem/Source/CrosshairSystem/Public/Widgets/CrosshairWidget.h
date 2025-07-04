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
	/**
	 * @brief Initializes the crosshair widget with an optional payload.
	 * @param InPayload Optional payload to associate with the widget.
	 */
	void Init(UObject* InPayload = nullptr);

	/**
	 * @brief Starts the opening animation or logic for the crosshair widget.
	 */
	UFUNCTION(BlueprintCallable)
	void StartOpening();

	/**
	 * @brief Starts the closing animation or logic for the crosshair widget.
	 */
	UFUNCTION(BlueprintCallable)
	void StartClosing();

	/**
	 * @brief Completes the opening animation or logic for the crosshair widget.
	 */
	UFUNCTION(BlueprintCallable)
	void CompleteOpening();

	/**
	 * @brief Completes the closing animation or logic for the crosshair widget.
	 */
	UFUNCTION(BlueprintCallable)
	void CompleteClosing();

	/**
	 * @brief Returns true if the crosshair widget is currently open.
	 * @return True if open, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsOpen() const;

protected:
	/**
	 * @brief Returns the payload associated with the crosshair widget.
	 * @return The payload object.
	 */
	UFUNCTION(BlueprintPure)
	UObject* GetPayload() const;
	
	/**
	 * @brief Called when the crosshair widget is initialized (Blueprint event).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	/**
	 * @brief Called when the crosshair widget starts opening (Blueprint event).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnStartOpening();

	/**
	 * @brief Called when the crosshair widget starts closing (Blueprint event).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnStartClosing();

	/**
	 * @brief Called when the crosshair widget completes opening (Blueprint event).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnCompleteOpening();

	/**
	 * @brief Called when the crosshair widget completes closing (Blueprint event).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnCompleteClosing();
};
