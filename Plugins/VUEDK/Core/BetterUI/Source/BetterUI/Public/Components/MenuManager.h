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
	UPROPERTY()
	bool bLocked;

public:
	/**
	 * @brief Sets the locked state of the menu manager. When locked, no new menus can be opened or closed until it is unlocked.
	 * @param bNewLocked The new locked state to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetLockedState(const bool bNewLocked);

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
	 * @brief Checks if the menu manager is currently locked.
	 * @return True if the menu manager is locked, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsLocked() const;
	
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
	 * @brief Opens the specified menu widget if it is not already open.
	 * @param Menu The menu widget to open.
	 */
	void OpenMenu(UMenuWidget* Menu);
	
	/**
	 * @brief Closes the specified menu widget if it is open.
	 * @param Menu The menu widget to close.
	 */
	void CloseMenu(UMenuWidget* Menu);

protected:
	virtual void BeginPlay() override;
	
private:
	/**
	 * @brief Sets the specified menu as open and updates the current open menu tag.
	 * @param Menu The menu widget to set as open.
	 */
	void SetOpenMenu(UMenuWidget* Menu);

	/**
	 * @brief Sets the specified menu as closed and updates the current open menu tag.
	 * @param Menu The menu widget to set as closed.
	 */
	void SetCloseMenu(UMenuWidget* Menu);

	/**
	 * @brief Checks the internal state of the menu manager for consistency.
	 * @return True if the state is valid, false otherwise.
	 */
	bool Check() const;
};
