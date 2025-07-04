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
	
	UFUNCTION(BlueprintCallable)
	void AddMenu(const FGameplayTag MenuTag, UMenuWidget* Menu, int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable)
	UMenuWidget* ToggleMenu(const FGameplayTag Tag);
	
	UFUNCTION(BlueprintCallable)
	void CloseCurrentMenu();

	UFUNCTION(BlueprintPure)
	UMenuWidget* GetCurrentOpenMenu() const;

	UFUNCTION(BlueprintPure)
	UMenuWidget* GetMenuByTag(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintPure)
	bool IsMenuOpen(const FGameplayTag Tag) const;

	UFUNCTION(BlueprintPure)
	bool IsAnyMenuOpen() const;

	void CloseMenuByMenuWidget(UMenuWidget* Menu);

private:
	void SetOpenMenu(UMenuWidget* Menu, FGameplayTag Tag);

	void SetCloseMenu(UMenuWidget* Menu, FGameplayTag Tag);
	
	bool Check() const;
};
