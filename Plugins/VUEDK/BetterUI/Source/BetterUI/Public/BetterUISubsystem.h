// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BetterUISettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BetterUISubsystem.generated.h"

UCLASS()
class BETTERUI_API UBetterUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UBetterUISettings* Settings;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	void SetSlateNavigationConfig() const;
};
