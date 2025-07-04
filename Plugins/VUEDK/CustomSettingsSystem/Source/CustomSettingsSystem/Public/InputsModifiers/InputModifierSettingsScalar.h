// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputModifiers.h"
#include "InputModifierSettingsBase.h"
#include "InputModifierSettingsScalar.generated.h"

UCLASS()
class CUSTOMSETTINGSSYSTEM_API UInputModifierSettingsScalar : public UInputModifierSettingsBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (EditCondition = "bCanScaleXAxis"))
	FGameplayTag XTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (EditCondition = "bCanScaleYAxis"))
	FGameplayTag YTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (EditCondition = "bCanScaleZAxis"))
	FGameplayTag ZTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	FVector DefaultScalar = FVector::OneVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	FVector Multiplier = FVector::OneVector;

private:
	UPROPERTY()
	bool bCanScaleXAxis = false;
	UPROPERTY()
	bool bCanScaleYAxis = false;
	UPROPERTY()
	bool bCanScaleZAxis = false;
	FVector Scalar;
	
protected:
	virtual void Init() override;
	
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

	virtual void OnSettingsApplied() override;
	
private:
	UFUNCTION()
	void SetScalarValue();

	void ApplyScale(FInputActionValue& CurrentValue) const;
};
