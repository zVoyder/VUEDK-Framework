// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputModifierSettingsBase.h"
#include "InputModifiersSettingsNegate.generated.h"

UCLASS()
class CUSTOMSETTINGSSYSTEM_API UInputModifiersSettingsNegate : public UInputModifierSettingsBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (EditCondition = "bCanInvertXAxis"))
	FGameplayTag XAxisTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (EditCondition = "bCanInvertYAxis"))
	FGameplayTag YAxisTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (EditCondition = "bCanInvertZAxis"))
	FGameplayTag ZAxisTag;

private:
	UPROPERTY()
	bool bCanInvertXAxis = false;
	UPROPERTY()
	bool bCanInvertYAxis = false;
	UPROPERTY()
	bool bCanInvertZAxis = false;
	bool bIsXAxisInverted = false;
	bool bIsYAxisInverted = false;
	bool bIsZAxisInverted = false;

protected:
	virtual void Init() override;
	
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
	
	void ApplyNegate(FInputActionValue& CurrentValue) const;

private:
	void SetNegateValues();
};
