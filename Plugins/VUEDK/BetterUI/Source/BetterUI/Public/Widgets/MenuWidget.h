// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MenuManager.h"
#include "MenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnMenuWidgetOpened
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnMenuWidgetClosed
);

UCLASS()
class BETTERUI_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMenuWidgetOpened OnMenuOpened;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMenuWidgetClosed OnMenuClosed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESlateVisibility OpenVisibility = ESlateVisibility::Visible;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESlateVisibility CloseVisibility = ESlateVisibility::Collapsed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ZOrder = 0;

private:
	bool bIsOpen;
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY()
	UMenuManager* MenuManager;

public:
	explicit UMenuWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	/**
	 * @brief Initializes the menu widget with the specified menu manager.
	 * @param InMenuManager The menu manager to associate with this widget.
	 */
	void Init(UMenuManager* InMenuManager);

	/**
	 * @brief Opens the menu natively (internal logic).
	 */
	void NativeOpen();

	/**
	 * @brief Closes the menu natively (internal logic).
	 */
	void NativeClose();

	/**
	 * @brief Closes the menu (Blueprint callable).
	 */
	UFUNCTION(BlueprintCallable)
	void Close();

	/**
	 * @brief Returns true if the menu is currently open.
	 * @return True if open, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsOpen() const;

protected:
	/**
	 * @brief Called when the menu is opened (Blueprint event).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnOpen();

	/**
	 * @brief Called when the menu is closed (Blueprint event).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnClose();

private:
	/**
	 * @brief Sets the menu's visibility to open state.
	 */
	void SetOpenVisibility();

	/**
	 * @brief Sets the menu's visibility to closed state.
	 */
	void SetCloseVisibility();

	/**
	 * @brief Checks the internal state of the menu widget.
	 * @return True if the state is valid, false otherwise.
	 */
	bool Check() const;
};
