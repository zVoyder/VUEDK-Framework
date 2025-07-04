// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EMenuBehaviour : uint8
{
	BlockNewOpenings UMETA(DisplayName = "BlockNewOpenings", ToolTip = "If the menu is open, it will block all other menus from opening."),
	SwitchMenu UMETA(DisplayName = "SwitchMenu", ToolTip = "If the menu is open, it will close the current menu and open the new one."),
	CloseCurrentMenu UMETA(DisplayName = "CloseCurrentMenu", ToolTip = "If the menu is open, it will close the current menu."),
};
