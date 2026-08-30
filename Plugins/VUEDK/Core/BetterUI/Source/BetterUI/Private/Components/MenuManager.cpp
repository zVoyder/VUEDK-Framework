// Copyright VUEDK, Inc. All Rights Reserved.

#include "Components/MenuManager.h"
#include "BetterUI.h"
#include "Widgets/MenuWidget.h"

void UMenuManager::SetLockedState(const bool bNewLocked)
{
	bLocked = bNewLocked;
}

void UMenuManager::AddMenu(const FGameplayTag MenuTag, UMenuWidget* Menu, int32 ZOrder)
{
	if (!IsValid(Menu))
	{
		UE_LOG(LogBetterUI, Warning, TEXT("UMenuManager::AddMenu: Invalid menu widget for tag %s."), *MenuTag.ToString());
		return;
	}

	if (MenuMap.Contains(MenuTag))
	{
		UE_LOG(LogBetterUI, Warning, TEXT("UMenuManager::AddMenu: Menu with tag %s already exists."), *MenuTag.ToString());
		return;
	}

	Menu->ZOrder = ZOrder;
	MenuMap.Add(MenuTag, Menu);
	Menu->Init(this, MenuTag);
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

	if (IsLocked())
		return Menu;

	if (!IsValid(CurrentOpenMenu))
	{
		SetOpenMenu(Menu);
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
			SetOpenMenu(Menu);
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
	
	SetCloseMenu(CurrentOpenMenu);
}

UMenuWidget* UMenuManager::GetCurrentOpenMenu() const
{
	return CurrentOpenMenu;
}

UMenuWidget* UMenuManager::GetMenuByTag(const FGameplayTag Tag) const
{
	if (!MenuMap.Contains(Tag))
	{
		UE_LOG(LogBetterUI, Warning, TEXT("UMenuManager::GetMenuByTag: Menu with tag %s not found."), *Tag.ToString());
		return nullptr;
	}

	return MenuMap[Tag];
}

bool UMenuManager::IsLocked() const
{
	return bLocked;
}

bool UMenuManager::IsMenuOpen(const FGameplayTag Tag) const
{
	if (!MenuMap.Contains(Tag))
	{
		UE_LOG(LogBetterUI, Warning, TEXT("UMenuManager::IsMenuOpen: Menu with tag %s not found."), *Tag.ToString());
		return false;
	}

	return MenuMap[Tag]->IsOpen();
}

bool UMenuManager::IsAnyMenuOpen() const
{
	return IsValid(CurrentOpenMenu);
}

void UMenuManager::OpenMenu(UMenuWidget* Menu)
{
	if (!Check())
	{
		UE_LOG(LogBetterUI, Error, TEXT("UMenuManager::OpenMenu: MenuManager check failed."));
		return;
	}
	
	SetOpenMenu(Menu);
}

void UMenuManager::CloseMenu(UMenuWidget* Menu)
{
	if (!Check())
	{
		UE_LOG(LogBetterUI, Error, TEXT("UMenuManager::CloseMenuByMenuWidget: MenuManager check failed."));
		return;
	}

	if (CurrentOpenMenu == Menu)
		SetCloseMenu(Menu);
}

void UMenuManager::BeginPlay()
{
	Super::BeginPlay();
	Settings = GetDefault<UBetterUISettings>();
}

void UMenuManager::SetOpenMenu(UMenuWidget* Menu)
{
	Menu->NativeOpen();
	CurrentOpenMenu = Menu;
	OnMenuOpened.Broadcast(CurrentOpenMenu, Menu->GetMenuTag());
}

void UMenuManager::SetCloseMenu(UMenuWidget* Menu)
{
	Menu->NativeClose();
	CurrentOpenMenu = nullptr;
	OnMenuClosed.Broadcast(Menu, Menu->GetMenuTag());
}

bool UMenuManager::Check() const
{
	return IsValid(Settings);
}
