// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/GOAPTagObjectBase.h"
#include "Data/GOAPBeliefData.h"
#include "DynamicOperations/DynamicValueOperation.h"
#include "AgentAction.generated.h"

class UAgentBelief;
class AAIController;
class UDynamicCostOperation;
struct FGOAPActionCreationData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnActionStarted,
	UAgentContext*, AgentContext,
	UAgentAction*, Action
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnActionFinished,
	UAgentContext*, AgentContext,
	UAgentAction*, Action,
	bool, bSuccess
);

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class VGOAP_API UAgentAction : public UGOAPTagObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnActionStarted OnActionStarted;
	UPROPERTY(BlueprintAssignable)
	FOnActionFinished OnActionFinished;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Action", meta = (Tooltip = "If true the planner will try to find a better plan while this action is being executed."))
	bool bAllowsReplanning;
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bUseTimeout = false;
	UPROPERTY(EditDefaultsOnly, Category = "Action",
		meta = (EditCondition="bUseTimeout", EditConditionHides, ToolTip = "Seconds after which the action will be considered failed if not completed."))
	float TimeoutSeconds = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,
		meta = (ToolTip = "Used to prioritize actions after planning. Higher values are executed first."), Category = "Priority")
	float BasePriority = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Priority")
	FFloatRange PriorityRange = FFloatRange(0.0f, 1000.0f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Priority")
	TObjectPtr<UDynamicValueOperation> PriorityOperation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cost")
	float BaseCost = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
	FFloatRange CostRange = FFloatRange(0.0f, 1000.0f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Cost")
	TObjectPtr<UDynamicValueOperation> CostOperation;
	
private:
	UPROPERTY()
	TArray<FGOAPBeliefCondition> Preconditions;
	UPROPERTY()
	TArray<FGOAPBeliefCondition> Effects;
	float CurrentPriority;
	float CurrentCost;
	bool bIsCompleted;
	bool bWasSuccessful = true;
	FTimerHandle TimeoutTimerHandle;
	
public:
	virtual void Init(UAgentContext* InContext) override;

	bool RegisterPrecondition(const FGameplayTag PreconditionTag, const bool bDesiredValue);

	bool RegisterEffect(const FGameplayTag EffectTag, const bool bDesiredValue);

	void UnregisterPrecondition(const FGameplayTag PreconditionTag);

	void UnregisterEffect(const FGameplayTag EffectTag);

	void RegisterPreconditions(const TArray<FGOAPBeliefEntryData>& InPreconditions);

	void RegisterEffects(const TArray<FGOAPBeliefEntryData>& InEffects);

	UFUNCTION(BlueprintCallable)
	void SetPriority(const float NewPriority);

	UFUNCTION(BlueprintCallable)
	void ResetPriority();

	UFUNCTION(BlueprintPure)
	float GetPriority() const;
	
	UFUNCTION(BlueprintCallable)
	void SetCost(const float NewCost);

	UFUNCTION(BlueprintCallable)
	void ResetCost();

	UFUNCTION(BlueprintPure)
	float GetCost() const;

	UFUNCTION(BlueprintPure)
	TArray<FGOAPBeliefCondition> GetPreconditions() const;

	UFUNCTION(BlueprintPure)
	TArray<FGOAPBeliefCondition> GetEffects() const;
	
	UFUNCTION(BlueprintPure)
	bool ArePreconditionsMet() const;
	
	UFUNCTION(BlueprintNativeEvent)
	bool CanActionPerform() const;
	
	UFUNCTION(BlueprintPure)
	bool IsCompleted() const;

	UFUNCTION(BlueprintPure)
	bool WasSuccessful() const;

	void StartAction();

	void PerformAction(const float DeltaTime);

	void StopAction();
	
	bool TryGetPreconditionDesiredValue(const UAgentBelief* Belief, bool& OutDesiredValue) const;

	bool TryGetEffectDesiredValue(const UAgentBelief* Belief, bool& OutDesiredValue) const;

protected:
	UFUNCTION(BlueprintCallable)
	void FinishExecute(const bool bSuccess = true);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnActionStart(APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard);

	UFUNCTION(BlueprintNativeEvent)
	void OnActionStop(APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard, const bool bSuccess);

	UFUNCTION(BlueprintNativeEvent)
	void OnActionPerform(const float DeltaTime, APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard);

private:
	void StartTimeoutTimer();

	void FinishExecuteTimeout();

	void ClearTimeoutTimer();
	
	static float ClampValueInRange(const float InValue, const FFloatRange& InRange);
	
	void ResetState();
};
