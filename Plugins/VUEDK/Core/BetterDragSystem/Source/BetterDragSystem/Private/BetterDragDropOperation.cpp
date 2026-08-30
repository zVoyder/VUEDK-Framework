// Copyright VUEDK, Inc. All Rights Reserved.

#include "BetterDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetComponent.h"
#include "Utility/BetterDragDropUtility.h"
#include "Kismet/GameplayStatics.h"

UBetterDragDropOperation::UBetterDragDropOperation() : OwnerWidget(nullptr),
                                                       DragVisual(nullptr),
                                                       Payload(nullptr),
                                                       PlayerController(nullptr),
                                                       DropReceiver(nullptr),
                                                       OwnerWidgetVisibility(ESlateVisibility::Hidden),
                                                       CachedOwnerWidgetVisibility(ESlateVisibility::Hidden),
                                                       bIsDragging(false)
{
	AddToRoot();
}

void UBetterDragDropOperation::Init(UUserWidget* InOwnerWidget, UUserWidget* InDragVisual, UObject* InPayload, const ESlateVisibility InOwnerWidgetVisibility, const bool bRemoveOnDrag)
{
	OwnerWidget = InOwnerWidget;
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	DragVisual = InDragVisual;
	Payload = InPayload;
	OwnerWidgetVisibility = InOwnerWidgetVisibility;
	UBetterDragDropUtility::SetDragDropOperation(this);
	bRemoveOwnerWidgetOnDrag = bRemoveOnDrag;
	BeginDrag();
}

void UBetterDragDropOperation::SetDropKeys(const TArray<FKey>& InDropKeys)
{
	DropKeys = InDropKeys;
	BindDropKeysEvent();
}

void UBetterDragDropOperation::EnterReceiver(UUserWidget* InDropReceiver)
{
	if (!Check()) return;

	if (IsValid(InDropReceiver) && InDropReceiver->Implements<UBetterDragDropReceiver>())
	{
		OnDragEnter(InDropReceiver);
		IBetterDragDropReceiver::Execute_OnBetterDragEnter(InDropReceiver, this, OwnerWidget, DragVisual, Payload);
	}

	DropReceiver = InDropReceiver;
}

void UBetterDragDropOperation::LeaveReceiver()
{
	if (IsValid(DropReceiver) && DropReceiver->Implements<UBetterDragDropReceiver>())
	{
		OnDragLeave(DropReceiver);
		IBetterDragDropReceiver::Execute_OnBetterDragLeave(DropReceiver, this, OwnerWidget, DragVisual, Payload);
	}

	DropReceiver = nullptr;
}

void UBetterDragDropOperation::Tick(float DeltaTime)
{
	UpdateDrag(CachedSize);
}

bool UBetterDragDropOperation::IsTickable() const
{
	return bIsDragging;
}

TStatId UBetterDragDropOperation::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBetterDragDropOperation, STATGROUP_Tickables);
}

ESlateVisibility UBetterDragDropOperation::GetOwnerWidgetVisibility() const
{
	return OwnerWidgetVisibility;
}

bool UBetterDragDropOperation::HasRemoveOwnerWidgetOnDrag() const
{
	return bRemoveOwnerWidgetOnDrag;
}

TArray<FKey> UBetterDragDropOperation::GetDropKeys() const
{
	return DropKeys;
}

bool UBetterDragDropOperation::CheckEndDrag()
{
	if (!bIsDragging)
		return false;

	EndDrag();
	return true;
}

bool UBetterDragDropOperation::CheckEndDragIfReleased(FKey Key)
{
	if (!DropKeys.Contains(Key))
		return false;

	return CheckEndDrag();
}

void UBetterDragDropOperation::UpdateDrag_Implementation(const FVector2D Size)
{
	if (!Check()) return;

	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(PlayerController);
	MousePosition -= Size * .5f;
	SetDragVisualPosition(MousePosition);
}

void UBetterDragDropOperation::SetDragVisualPosition_Implementation(const FVector2D PositionInViewport) const
{
	DragVisual->SetPositionInViewport(PositionInViewport, false);
}

void UBetterDragDropOperation::OnBeginDrag_Implementation()
{
}

void UBetterDragDropOperation::OnEndDrag_Implementation()
{
}

void UBetterDragDropOperation::OnDragEnter_Implementation(UUserWidget* EnteredWidget)
{
}

void UBetterDragDropOperation::OnDragLeave_Implementation(UUserWidget* LeftWidget)
{
}

bool UBetterDragDropOperation::OnDragDrop_Implementation()
{
	return true;
}

void UBetterDragDropOperation::OnDragCancelled_Implementation()
{
}

UWorld* UBetterDragDropOperation::GetWorld() const
{
	return OwnerWidget->GetWorld();
}

#if WITH_EDITOR
bool UBetterDragDropOperation::ImplementsGetWorld() const
{
	return true;
}
#endif

void UBetterDragDropOperation::BeginDrag()
{
	if (!Check())
		return;

	int32 ZOrder = 100;
	FSlateApplication::Get().ReleaseAllPointerCapture();
	CachedSize = OwnerWidget->GetDesiredSize();
	CachedOwnerWidgetVisibility = OwnerWidget->GetVisibility();
	OwnerWidget->SetVisibility(OwnerWidgetVisibility);

	if (const UCanvasPanelSlot* Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(OwnerWidget))
	{
		CachedSize = Slot->GetSize();
		ZOrder = Slot->GetZOrder() + 1;
	}

	DragVisual->SetDesiredSizeInViewport(CachedSize);
	DragVisual->SetVisibility(ESlateVisibility::HitTestInvisible);
	DragVisual->AddToViewport(ZOrder);
	bIsDragging = true;
	OnBeginDrag();

	if (bRemoveOwnerWidgetOnDrag)
		OwnerWidget->RemoveFromParent();
}

void UBetterDragDropOperation::EndDrag()
{
	if (!Check())
		return;

	if (IsValid(DropReceiver))
	{
		if (OnDragDrop())
			IBetterDragDropReceiver::Execute_OnBetterDragDrop(DropReceiver, this, OwnerWidget, DragVisual, Payload);
	}
	else
	{
		OnDragCancelled();
	}

	OwnerWidget->SetVisibility(CachedOwnerWidgetVisibility);
	DragVisual->RemoveFromParent();

	bIsDragging = false;

	OnEndDrag();
	UBetterDragDropUtility::ClearDragDropOperation();
	UnbindDropKeysEvent();
	RemoveFromRoot();
}

void UBetterDragDropOperation::CheckEndDragKeyEvent(FKey Key)
{
	CheckEndDragIfReleased(Key);
}

void UBetterDragDropOperation::BindDropKeysEvent()
{
	if (DropKeys.IsEmpty())
		return;

	for (auto Key : DropKeys)
	{
		if (!Key.IsValid())
			continue;

		FInputKeyBinding& KeyBinding = PlayerController->InputComponent->BindKey(Key, IE_Released, this, &UBetterDragDropOperation::CheckEndDragKeyEvent);
		DropKeysBindings.Add(KeyBinding);
	}
}

void UBetterDragDropOperation::UnbindDropKeysEvent()
{
	if (DropKeys.IsEmpty())
		return;

	for (auto KeyBinding : DropKeysBindings)
		KeyBinding.KeyDelegate.Unbind();

	DropKeysBindings.Empty();
}

bool UBetterDragDropOperation::Check() const
{
	return IsValid(PlayerController) && IsValid(OwnerWidget) && IsValid(DragVisual);
}
