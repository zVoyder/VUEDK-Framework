// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimplePerceiver.h"
#include "Base/SimplePerceiverObjectBase.h"
#include "SenseValidators/Base/SenseValidatorBase.h"
#include "Sensors/Base/SensorBase.h"
#include "SenseBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class SIMPLEPERCEPTIONSYSTEM_API USenseBase : public USimplePerceiverObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense")
	TEnumAsByte<ESensedStatus> DefaultSensedStatus = ESensedStatus::Direct;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Sense")
	TArray<USensorBase*> Sensors;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Sense")
	TArray<USenseValidatorBase*> Validators;

public:
	virtual void Init(USimplePerceiver* InSimplePerceiver) override;

	void Process(const float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SenseActor(AActor* Actor);
	
	bool ValidateSensedActor(AActor* Actor) const;

private:
	void InitSensors(USimplePerceiver* InSimplePerceiver);

	void InitValidators(USimplePerceiver* InSimplePerceiver);

	void ProcessSensors(const float DeltaTime);

	/**
	 * @brief Determines whether the actor can be sensed.
	 * @param Actor The actor to check.
	 * @return True if the actor can be sensed; otherwise, false.
	 */
	bool FilterSensedActor(AActor* Actor) const;
};
