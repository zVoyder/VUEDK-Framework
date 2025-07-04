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

	void Init(UMenuManager* InMenuManager);

	void NativeOpen();

	void NativeClose();

	UFUNCTION(BlueprintCallable)
	void Close();

	UFUNCTION(BlueprintPure)
	bool IsOpen() const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnOpen();

	UFUNCTION(BlueprintNativeEvent)
	void OnClose();

private:
	void SetOpenVisibility();

	void SetCloseVisibility();

	bool Check() const;
};
