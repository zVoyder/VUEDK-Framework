// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputCoreTypes.h"
#include "InputsHandlerSystemSettings.h"
#include "Data/InputModeType.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "InputsHandlerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnGampleayMappingContextSet
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnUIMappingContextSet
);

UCLASS(NotBlueprintable, BlueprintType)
class INPUTSHANDLERSYSTEM_API UInputsHandlerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnGampleayMappingContextSet OnGameplayMappingContextSet;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnUIMappingContextSet OnUIMappingContextSet;

protected:
	UPROPERTY(BlueprintReadOnly)
	APlayerController* PlayerController;

private:
	UPROPERTY()
	TArray<UInputMappingContext*> CommonMappingContexts;
	UPROPERTY()
	TArray<UInputMappingContext*> GameplayMappingContexts;
	UPROPERTY()
	TArray<UInputMappingContext*> UIMappingContexts;
	UPROPERTY()
	TArray<UInputMappingContext*> ExcludedContexts;
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;
	UPROPERTY()
	UInputDeviceSubsystem* InputDeviceSubsystem;
	UPROPERTY()
	const UInputsHandlerSystemSettings* InputsHandlerSystemSettings;
	EInputModeType CurrentInputModeType;
	bool bIsUsingGamepad;

public:
	UInputsHandlerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
	
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;

	UFUNCTION(BlueprintPure)
	FVector2D GetNormalizedMousePosition() const;

	UFUNCTION(BlueprintPure)
	UEnhancedInputUserSettings* GetEnhancedInputUserSettings() const;

	UFUNCTION(BlueprintCallable)
	void AddCommonMappingContext(UInputMappingContext* MappingContext);

	UFUNCTION(BlueprintCallable)
	void RemoveCommonMappingContext(UInputMappingContext* MappingContext);
	
	UFUNCTION(BlueprintCallable)
	void AddGameplayMappingContext(UInputMappingContext* MappingContext);

	UFUNCTION(BlueprintCallable)
	void RemoveGameplayMappingContext(UInputMappingContext* MappingContext);

	UFUNCTION(BlueprintCallable)
	void AddUIMappingContext(UInputMappingContext* MappingContext);

	UFUNCTION(BlueprintCallable)
	void RemoveUIMappingContext(UInputMappingContext* MappingContext);

	UFUNCTION(BlueprintCallable)
	void AddExcludedMappingContext(UInputMappingContext* MappingContext);

	UFUNCTION(BlueprintCallable)
	void RemoveExcludedMappingContext(UInputMappingContext* MappingContext);
	
	UFUNCTION(BlueprintCallable)
	void SetModeGameplay();
	
	UFUNCTION(BlueprintCallable)
	void SetModeUI();

	UFUNCTION(BlueprintPure)
	TArray<UInputMappingContext*> GetAllMappingContexts() const;

	UFUNCTION(BlueprintPure)
	TArray<UInputMappingContext*> GetCommonMappingContexts() const;
	
	UFUNCTION(BlueprintPure)
	TArray<UInputMappingContext*> GetGameplayMappingContexts() const;

	UFUNCTION(BlueprintPure)
	TArray<UInputMappingContext*> GetUIMappingContexts() const;
	
	UFUNCTION(BlueprintPure)
	UInputMappingContext* GetGameplayMappingContext(const int32 Index = 0) const;

	UFUNCTION(BlueprintPure)
	UInputMappingContext* GetUIMappingContext(const int32 Index = 0) const;

	UFUNCTION(BlueprintPure)
	bool IsUsingGamepad() const;

	UFUNCTION(BlueprintPure)
	bool IsInputModeGameplay() const;

	UFUNCTION(BlueprintPure)
	bool IsInputModeUI() const;

private:
	void SetInputModeGameplay() const;
	
	void SetInputModeUI() const;

	void AddCommonContexts() const;
	
	void AddGameplayContexts() const;

	void RemoveGameplayContexts() const;

	void AddUIContexts(int32 Priority) const;

	void RemoveUIContexts() const;

	void RemoveExcludedContexts() const;
	
	void BindCheckGamepad();

	void UnbindCheckGamepad();

	void CenterMouseOnViewport() const;

	void LoadMappingContexts();
	
	UFUNCTION()
	void CheckUsingGamepad(const FPlatformUserId UserId, const FInputDeviceId DeviceId);
	
	bool Check() const;
};
