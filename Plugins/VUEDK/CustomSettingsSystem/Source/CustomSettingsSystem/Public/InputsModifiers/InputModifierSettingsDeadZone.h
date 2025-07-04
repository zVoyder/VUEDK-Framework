// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputModifiers.h"
#include "InputModifierSettingsBase.h"
#include "InputModifierSettingsDeadZone.generated.h"

UCLASS()
class CUSTOMSETTINGSSYSTEM_API UInputModifierSettingsDeadZone : public UInputModifierSettingsBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	FGameplayTag DeadZoneTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float DefaultTreshold = 0.2f;

private:
	float Treshold = 0.2f;
	
protected:
	virtual void Init() override;

	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

	virtual void OnSettingsApplied() override;

private:
	void ApplyDeadZone(FInputActionValue& CurrentValue) const;
	
	void SetTresholdValue();
};
