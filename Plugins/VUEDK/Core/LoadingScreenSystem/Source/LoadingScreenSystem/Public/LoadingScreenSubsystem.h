// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LoadingScreenSystemSettings.h"
#include "MoviePlayer.h"
#include "Data/LoadingScreenData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoadingScreenSubsystem.generated.h"

UCLASS()
class LOADINGSCREENSYSTEM_API ULoadingScreenSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	const ULoadingScreenSystemSettings* Settings;
	FLoadingScreenAttributes LoadingScreenAttributes;
	bool bIsPendingResetToDefault = false;
	bool bHasEverLoaded;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
	
	void SetupLoadingScreen(const ULoadingScreenData* LoadingScreenData, const bool bAutoResetToDefault = true);
	
	void ResetToDefaultLoadingScreen();

	void ClearLoadingScreen(const bool bAutoResetToDefault = true);
	
private:
	void OnPreLoadMapWithContext(const FWorldContext& WorldContext, const FString& MapName);
	
	void OnPostLoadMapWithWorld(UWorld* World);
	
	bool Check() const;
};
