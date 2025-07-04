// Copyright VUEDK, Inc. All Rights Reserved.

#include "Widgets/MenuWidget.h"

#include "BetterUI.h"

UMenuWidget::UMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                        bIsOpen(false),
                                                                        PlayerController(nullptr),
                                                                        MenuManager(nullptr)
{
	SetIsFocusable(true);
}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = GetOwningPlayer();
}

void UMenuWidget::Init(UMenuManager* InMenuManager)
{
	if (!IsValid(InMenuManager))
	{
		UE_LOG(LogBetterUI, Warning, TEXT("UMenuWidget::Init: MenuManager is not valid!"));
		return;
	}

	MenuManager = InMenuManager;
	AddToViewport(ZOrder);
	SetCloseVisibility();
}

void UMenuWidget::NativeOpen()
{
	if (bIsOpen)
		return;
	
	bIsOpen = true;
	SetOpenVisibility();
	OnOpen();
	OnMenuOpened.Broadcast();
}

void UMenuWidget::NativeClose()
{
	if (!bIsOpen)
		return;
	
	SetCloseVisibility();
	bIsOpen = false;
	OnClose();
	OnMenuClosed.Broadcast();
}

void UMenuWidget::Close()
{
	if (!Check())
		return;
	
	MenuManager->CloseMenuByMenuWidget(this);
}

bool UMenuWidget::IsOpen() const
{
	return bIsOpen;
}

void UMenuWidget::OnClose_Implementation()
{
}

void UMenuWidget::OnOpen_Implementation()
{
}

void UMenuWidget::SetOpenVisibility()
{
	SetVisibility(OpenVisibility);
	SetFocus();
}

void UMenuWidget::SetCloseVisibility()
{
	SetVisibility(CloseVisibility);
}

bool UMenuWidget::Check() const
{
	return IsValid(PlayerController) && IsValid(MenuManager);
}
