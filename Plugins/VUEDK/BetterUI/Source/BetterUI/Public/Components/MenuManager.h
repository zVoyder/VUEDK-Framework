// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BetterUISettings.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "MenuManager.generated.h"

class UMenuWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnMenuOpened,
	UMenuWidget*, MenuWidget,
	FGameplayTag, MenuTag
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnMenuClosed,
	UMenuWidget*, MenuWidget,
	FGameplayTag, MenuTag
);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BETTERUI_API UMenuManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMenuOpened OnMenuOpened;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMenuClosed OnMenuClosed;

private:
	UPROPERTY()
	UMenuWidget* CurrentOpenMenu;
	UPROPERTY()
	TMap<FGameplayTag, UMenuWidget*> MenuMap;
	UPROPERTY()
	const UBetterUISettings* Settings;

public:
	virtual void BeginPlay() override;
	
	/**
	 * @brief Adds a menu to the manager and optionally sets its ZOrder.
	 * @param MenuTag The tag identifying the menu.
	 * @param Menu The menu widget to add.
	 * @param ZOrder The Z-order for the menu (default is 0).
	 */
	UFUNCTION(BlueprintCallable)
	void AddMenu(const FGameplayTag MenuTag, UMenuWidget* Menu, int32 ZOrder = 0);

	/**
	 * @brief Toggles the visibility of the menu associated with the given tag.
	 * @param Tag The tag identifying the menu to toggle.
	 * @return The toggled menu widget, or nullptr if not found.
	 */
	UFUNCTION(BlueprintCallable)
	UMenuWidget* ToggleMenu(const FGameplayTag Tag);
	
	/**
	 * @brief Closes the currently open menu, if any.
	 */
	UFUNCTION(BlueprintCallable)
	void CloseCurrentMenu();

	/**
	 * @brief Gets the currently open menu widget.
	 * @return The currently open menu widget, or nullptr if none is open.
	 */
	UFUNCTION(BlueprintPure)
	UMenuWidget* GetCurrentOpenMenu() const;

	/**
	 * @brief Gets the menu widget associated with the given tag.
	 * @param Tag The tag identifying the menu.
	 * @return The menu widget, or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure)
	UMenuWidget* GetMenuByTag(const FGameplayTag Tag) const;

	/**
	 * @brief Checks if the menu with the given tag is currently open.
	 * @param Tag The tag identifying the menu.
	 * @return True if the menu is open, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsMenuOpen(const FGameplayTag Tag) const;

	/**
	 * @brief Checks if any menu is currently open.
	 * @return True if any menu is open, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsAnyMenuOpen() const;

	/**
	 * @brief Closes the specified menu widget if it is open.
	 * @param Menu The menu widget to close.
	 */
	void CloseMenuByMenuWidget(UMenuWidget* Menu);

private:
	/**
	 * @brief Sets the specified menu as open and updates the current open menu tag.
	 * @param Menu The menu widget to set as open.
	 * @param Tag The tag associated with the menu.
	 */
	void SetOpenMenu(UMenuWidget* Menu, FGameplayTag Tag);

	/**
	 * @brief Sets the specified menu as closed and updates the current open menu tag.
	 * @param Menu The menu widget to set as closed.
	 * @param Tag The tag associated with the menu.
	 */
	void SetCloseMenu(UMenuWidget* Menu, FGameplayTag Tag);

	/**
	 * @brief Checks the internal state of the menu manager for consistency.
	 * @return True if the state is valid, false otherwise.
	 */
	bool Check() const;
};
