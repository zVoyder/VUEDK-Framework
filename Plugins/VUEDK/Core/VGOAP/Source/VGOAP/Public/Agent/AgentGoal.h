// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/GOAPTagObjectBase.h"
#include "Data/GOAPBeliefData.h"
#include "DynamicOperations/DynamicValueOperation.h"
#include "UObject/Object.h"
#include "AgentGoal.generated.h"

class UDynamicPriorityOperation;
class UAgentBelief;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnGoalAchieved,
	UAgentContext*, AgentContext,
	UAgentGoal*, Goal
);

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class VGOAP_API UAgentGoal : public UGOAPTagObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnGoalAchieved OnGoalAchieved;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Priority")
	float BasePriority = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Priority")
	FFloatRange PriorityRange = FFloatRange(0.0f, 1000.0f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Priority")
	TObjectPtr<UDynamicValueOperation> PriorityOperation;
	
private:
	UPROPERTY()
	TArray<FGOAPBeliefCondition> DesiredEffects;
	float CurrentPriority = 0.0f;
	float DynamicPriority = 0.0f;

public:
	virtual void Init(UAgentContext* InContext) override;

	bool RegisterDesiredEffect(const FGameplayTag EffectTag, const bool bDesiredValue);

	void UnregisterDesiredEffect(const FGameplayTag EffectTag);
	
	void RegisterDesiredEffects(const TArray<FGOAPBeliefEntryData>& InDesiredEffects);

	UFUNCTION(BlueprintCallable)
	void SetPriority(const float NewPriority);

	UFUNCTION(BlueprintCallable)
	void ResetPriority();

	UFUNCTION(BlueprintPure)
	float GetPriority();

 	/**
	 * Gets the cached dynamic priority of the goal.
	 * Each time GetPriority() is called, the dynamic priority is recalculated and cached.
	 * @return The cached dynamic priority of the goal.
	 */
	UFUNCTION(BlueprintPure)
	float GetDynamicPriority() const;
	
	UFUNCTION(BlueprintPure)
	TArray<FGOAPBeliefCondition> GetDesiredEffects() const;
	
	bool IsPursuable();
	
	bool IsSatisfied();
	
	void AchieveGoal();

protected:
	UFUNCTION(BlueprintNativeEvent)
	bool CanBeSatisfied();

	UFUNCTION(BlueprintNativeEvent)
	void OnGoalAchieve(APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard);

private:
	float ClampPriority(float InPriority) const;
	
	bool EvaluateDesiredEffects() const;
};
