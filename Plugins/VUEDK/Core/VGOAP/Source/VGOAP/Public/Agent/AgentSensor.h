// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/GOAPObjectBase.h"
#include "Base/GOAPTagObjectBase.h"
#include "AgentSensor.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class VGOAP_API UAgentSensor : public UGOAPTagObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanEverUpdate = true;
	/** The frequency in seconds at which the UpdateSensor function will be executed. If less than or equal to 0 then it will update every frame.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Units = "s", EditCondition = "bCanEverUpdate"))
	float UpdateInterval;

private:
	float TimeSinceLastTick = 0.f;

public:
	virtual void Init(UAgentContext* InContext) override;
	
	void UpdateSensor(const float DeltaTime);

protected:
	/**
	 * Called when the sensor is updated.
	 * @param DeltaTime Time elapsed since the last frame.
	 * @param FixedDeltaTime Time elapsed since the last sensor update.
	 * @param Pawn The pawn of the agent.
	 * @param Controller The AI controller of the agent.
	 * @param Blackboard The blackboard of the agent.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnUpdateSensor(const float DeltaTime, const float FixedDeltaTime, APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard);
};
