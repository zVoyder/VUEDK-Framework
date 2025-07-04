// Copyright VUEDK, Inc. All Rights Reserved.

#include "Components/MenuManager.h"
#include "BetterUI.h"
#include "Widgets/MenuWidget.h"

void UMenuManager::BeginPlay()
{
	Super::BeginPlay();
	Settings = GetDefault<UBetterUISettings>();
}

void UMenuManager::AddMenu(const FGameplayTag MenuTag, UMenuWidget* Menu, int32 ZOrder)
{
	if (MenuMap.Contains(MenuTag))
	{
		UE_LOG(LogBetterUI, Warning, TEXT("UMenuManager::AddMenu: Menu with tag %s already exists."), *MenuTag.ToString());
		return;
	}

	MenuMap.Add(MenuTag, Menu);
	Menu->Init(this);
}

UMenuWidget* UMenuManager::ToggleMenu(const FGameplayTag Tag)
{
	if (!Check())
	{
		UE_LOG(LogBetterUI, Error, TEXT("UMenuManager::ToggleMenu: MenuManager check failed."));
		return nullptr;
	}

	UMenuWidget* Menu = GetMenuByTag(Tag);
	if (!IsValid(Menu))
	{
		UE_LOG(LogBetterUI, Warning, TEXT("UMenuManager::ToggleMenu: Menu with tag %s not found."), *Tag.ToString());
		return nullptr;
	}

	if (!IsValid(CurrentOpenMenu))
	{
		SetOpenMenu(Menu, Tag);
		return Menu;
	}

	if (CurrentOpenMenu == Menu)
	{
		CloseCurrentMenu(); // Close the current menu if it's the same as the one being toggled
		return Menu;
	}

	switch (Settings->MenuBehaviour)
	{
	case EMenuBehaviour::BlockNewOpenings:
		{
			// Do nothing, as the current menu is already open
			break;
		}
	case EMenuBehaviour::SwitchMenu:
		{
			// Close the current menu and open the new one
			CloseCurrentMenu();
			SetOpenMenu(Menu, Tag);
			break;
		}
	case EMenuBehaviour::CloseCurrentMenu:
		{
			// Close the current menu
			CloseCurrentMenu();
			break;
		}
	default: ;
	}

	return Menu;
}

void UMenuManager::CloseCurrentMenu()
{
	if (!IsValid(CurrentOpenMenu))
		return;

	const FGameplayTag* Tag = MenuMap.FindKey(CurrentOpenMenu);
	if (!Tag)
	{
		UE_LOG(LogBetterUI, Warning, TEXT("UMenuManager::CloseCurrentMenu: Menu not found in MenuMap."));
		return;
	}
	
	SetCloseMenu(CurrentOpenMenu, *Tag);
}

UMenuWidget* UMenuManager::GetCurrentOpenMenu() const
{
	return CurrentOpenMenu;
}

UMenuWidget* UMenuManager::GetMenuByTag(const FGameplayTag Tag) const
{
	if (!MenuMap.Contains(Tag))
	{
		UE_LOG(LogBetterUI, Warning, TEXT("UMenuManager::GetMenuByTag: Menu with tag %s not found!"), *Tag.ToString());
		return nullptr;
	}

	return MenuMap[Tag];
}

bool UMenuManager::IsMenuOpen(const FGameplayTag Tag) const
{
	if (!MenuMap.Contains(Tag))
	{
		UE_LOG(LogBetterUI, Warning, TEXT("UMenuManager::IsMenuOpen: Menu with tag %s not found!"), *Tag.ToString());
		return false;
	}

	return MenuMap[Tag]->IsOpen();
}

bool UMenuManager::IsAnyMenuOpen() const
{
	return IsValid(CurrentOpenMenu);
}

void UMenuManager::CloseMenuByMenuWidget(UMenuWidget* Menu)
{
	if (!Check())
	{
		UE_LOG(LogBetterUI, Error, TEXT("UMenuManager::CloseMenuByMenuWidget: MenuManager check failed."));
		return;
	}

	if (CurrentOpenMenu == Menu)
		SetCloseMenu(Menu, FGameplayTag::EmptyTag);
}

void UMenuManager::SetOpenMenu(UMenuWidget* Menu, const FGameplayTag Tag)
{
	Menu->NativeOpen();
	CurrentOpenMenu = Menu;
	OnMenuOpened.Broadcast(CurrentOpenMenu, Tag);
}

void UMenuManager::SetCloseMenu(UMenuWidget* Menu, const FGameplayTag Tag)
{
	Menu->NativeClose();
	CurrentOpenMenu = nullptr;
	OnMenuClosed.Broadcast(Menu, Tag);
}

bool UMenuManager::Check() const
{
	return IsValid(Settings);
}
