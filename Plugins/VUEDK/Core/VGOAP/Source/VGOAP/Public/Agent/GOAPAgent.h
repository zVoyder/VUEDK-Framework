// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AgentSensor.h"
#include "AgentBelief.h"
#include "AgentAction.h"
#include "AgentGoal.h"
#include "Data/GOAPAgentBehaviourData.h"
#include "Data/GOAPBlackboardData.h"
#include "Planner/Plan.h"
#include "Planner/Planner.h"
#include "GOAPAgent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnRunBehaviour
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnStopBehaviour
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnPauseBehaviour
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnResumeBehaviour
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnAnyBeliefStateChanged,
	UAgentBelief*, Belief,
	bool, NewState
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnAnyActionStarted,
	UAgentContext*, AgentContext,
	UAgentAction*, Action
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnAnyActionFinished,
	UAgentContext*, AgentContext,
	UAgentAction*, Action,
	bool, bSuccess
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnAnyGoalAchieved,
	UAgentContext*, AgentContext,
	UAgentGoal*, Goal
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPlanCreated,
	UAgentContext*, AgentContext,
	UAgentGoal*, Goal
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPlanFailed,
	UAgentContext*, AgentContext
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VGOAP_API UGOAPAgent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnRunBehaviour OnRunBehaviour;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnStopBehaviour OnStopBehaviour;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnPauseBehaviour OnPauseBehaviour;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnResumeBehaviour OnResumeBehaviour;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyBeliefStateChanged OnAnyBeliefStateChanged;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyActionStarted OnAnyActionStarted;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyActionFinished OnAnyActionFinished;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnAnyGoalAchieved OnAnyGoalAchieved;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnPlanCreated OnPlanCreated;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnPlanFailed OnPlanFailed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Agent|Behaviour")
	UGOAPAgentBehaviourData* BehaviourData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Agent|Behaviour")
	bool bCanEverReplan = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent|Behaviour", meta = (Tooltip = "If true, the agent will automatically start its behaviour after BeginPlay."))
	bool bAutoRunBehaviour = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent|Behaviour", meta = (EditCondition = "bAutoRunBehaviour", UIMin = "0.0", ClampMin = "0.0", Tooltip = "Delay in seconds before the agent starts its behaviour after BeginPlay."))
	float AutoRunDelay = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Agent|Blackboard")
	TObjectPtr<UGOAPBlackboardData> BlackboardData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Agent|Blackboard", meta = (Tooltip = "Tag to identify a global blackboard to be used by this agent. If you need multiple global blackboards, use VGOAPUtility::GetGlobalBlackboard function instead."))
	FGameplayTag GlobalBlackboard;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay, Category = "Agent",
		meta = (Tooltip = "If true, the agent will compute its plan asynchronously. This can improve performance for complex plans, but may introduce a delay before the plan is available."))
	bool bAsyncPlanComputation = true;

private:
	FPlan CurrentPlan;
	FPlanner Planner;
	UPROPERTY()
	TObjectPtr<UAgentGoal> CurrentGoal;
	UPROPERTY()
	TObjectPtr<UAgentAction> CurrentAction;
	UPROPERTY()
	TObjectPtr<UAgentGoal> LastGoal;
	UPROPERTY()
	TObjectPtr<UAgentAction> LastAction;
	UPROPERTY()
	TObjectPtr<UAgentContext> AgentContext;
	UPROPERTY()
	TMap<FGameplayTag, UAgentSensor*> Sensors;
	UPROPERTY()
	TMap<FGameplayTag, UAgentBelief*> Beliefs;
	UPROPERTY()
	TMap<FGameplayTag, UAgentAction*> Actions;
	UPROPERTY()
	TMap<FGameplayTag, UAgentGoal*> Goals;
	bool bIsProcessingBehaviour = false;

public:
	UGOAPAgent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	 * Runs the GOAP behaviour of the agent.
	 */
	UFUNCTION(BlueprintCallable)
	void RunBehaviour();

	/**
	 * Stops the GOAP behaviour of the agent, clearing the current plan and states.
	 * @param bClearData If true, the agent will clear its behaviour and blackboard data, requiring new data to be set before running the behaviour again.
	 */
	UFUNCTION(BlueprintCallable)
	void StopBehaviour(const bool bClearData = false);

	UFUNCTION(BlueprintCallable)
	void ResumeBehaviour();

	/**
	 * Pauses the GOAP behaviour of the agent.
	 */
	UFUNCTION(BlueprintCallable)
	void PauseBehaviour();

	/**
	 * Changes the GOAP behaviour of the agent to a new behaviour data.
	 * @param NewBehaviourData The new behaviour data to set.
	 * @param bRunImmediately If true, the behaviour will be run immediately after changing.
	 */
	UFUNCTION(BlueprintCallable)
	void ChangeBehaviour(UGOAPAgentBehaviourData* NewBehaviourData, const bool bRunImmediately = true);

	UFUNCTION(BlueprintCallable)
	void ChangeBlackboard(UGOAPBlackboardData* NewBlackboardData) const;

	UFUNCTION(BlueprintPure)
	bool IsRunning() const;

	/**
	 * Tries to add a sensor to the agent's sensors.
	 * @param SensorTag The tag identifying the sensor.
	 * @param Sensor The sensor to add.
	 * @param bReplan If true, the agent will replan its actions after adding the sensor.
	 * @return True if the sensor was added successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryAddSensor(const FGameplayTag SensorTag, UAgentSensor* Sensor, const bool bReplan = false);

	/**
	 * Tries to add a belief to the agent's mental state.
	 * @param BeliefTag The tag identifying the belief.
	 * @param Belief The belief to add.
	 * @param bReplan If true, the agent will replan its actions after adding the belief.
	 * @return True if the belief was added successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryAddBelief(const FGameplayTag BeliefTag, UAgentBelief* Belief, const bool bReplan = false);

	/**
	 * Tries to add an action to the agent's action list.
	 * @param ActionTag The tag identifying the action.
	 * @param Action The action to add.
	 * @param bReplan If true, the agent will replan its actions after adding the action.
	 * @return True if the action was added successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryAddAction(const FGameplayTag ActionTag, UAgentAction* Action, const bool bReplan = false);

	/**
	 * Tries to add a goal to the agent's goal list.
	 * @param GoalTag The tag identifying the goal.
	 * @param Goal The goal to add.
	 * @param bReplan If true, the agent will replan its actions after adding the goal.
	 * @return True if the goal was added successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool TryAddGoal(const FGameplayTag GoalTag, UAgentGoal* Goal, const bool bReplan = false);

	/**
	 * Removes a sensor from the agent's sensors.
	 * @param SensorTag The tag identifying the sensor to remove.
	 * @return The removed sensor, or nullptr if it was not found.
	 */
	UFUNCTION(BlueprintCallable)
	UAgentSensor* RemoveSensor(const FGameplayTag SensorTag);

	/**
	 * Removes a belief from the agent's mental state and unregisters it from all actions and goals.
	 * @param BeliefTag The tag identifying the belief to remove.
	 * @return The removed belief, or nullptr if it was not found.
	 */
	UFUNCTION(BlueprintCallable)
	UAgentBelief* RemoveBelief(const FGameplayTag BeliefTag);

	/**
	 * Removes an action from the agent's action list.
	 * @param ActionTag The tag identifying the action to remove.
	 * @return The removed action, or nullptr if it was not found.
	 */
	UFUNCTION(BlueprintCallable)
	UAgentAction* RemoveAction(const FGameplayTag ActionTag);

	/**
	 * Removes a goal from the agent's goal list.
	 * @param GoalTag The tag identifying the goal to remove.
	 * @return The removed goal, or nullptr if it was not found.
	 */
	UFUNCTION(BlueprintCallable)
	UAgentGoal* RemoveGoal(const FGameplayTag GoalTag);

	/**
	 * Gets the agent's sensors.
	 * @return A map of the agent's sensors.
	 */
	UFUNCTION(BlueprintPure)
	TMap<FGameplayTag, UAgentSensor*> GetSensors() const;

	/**
	 * Gets the agent's beliefs.
	 * @return A map of the agent's beliefs.
	 */
	UFUNCTION(BlueprintPure)
	TMap<FGameplayTag, UAgentBelief*> GetBeliefs() const;

	/**
	 * Gets the agent's actions.
	 * @return A map of the agent's actions.
	 */
	UFUNCTION(BlueprintPure)
	TMap<FGameplayTag, UAgentAction*> GetActions() const;

	/**
	 * Gets the actions currently in the agent's plan stack.
	 * @return An array of the actions in the plan stack.
	 */
	UFUNCTION(BlueprintPure)
	TArray<UAgentAction*> GetActionsStackInPlan() const;

	/**
	 * Gets the actions map currently in the agent's plan.
	 * Does not guarantee the order of actions in the stack, use GetActionsStackInPlan() for ordered actions.
	 * @return A map of the actions in the plan.
	 */
	UFUNCTION(BlueprintPure)
	TMap<FGameplayTag, UAgentAction*> GetActionsMapInPlan() const;

	/**
	 * Gets the agent's goals.
	 * @return A map of the agent's goals.
	 */
	UFUNCTION(BlueprintPure)
	TMap<FGameplayTag, UAgentGoal*> GetGoals() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnPreInit();

	UFUNCTION(BlueprintNativeEvent)
	void OnSetupSensors();

	UFUNCTION(BlueprintNativeEvent)
	void OnSetupBeliefs();

	UFUNCTION(BlueprintNativeEvent)
	void OnSetupActions();

	UFUNCTION(BlueprintNativeEvent)
	void OnSetupGoals();

	UFUNCTION(BlueprintNativeEvent)
	void OnInit();

	/**
	 * Checks if the agent has a sensor with the specified tag.
	 * @param SensorTag The tag identifying the sensor.
	 * @return True if the sensor exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasSensor(const FGameplayTag SensorTag) const;

	/**
	 * Checks if the agent has a belief with the specified tag.
	 * @param BeliefTag The tag identifying the belief.
	 * @return True if the belief exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasBelief(const FGameplayTag BeliefTag) const;

	/**
	 * Checks if the agent has all beliefs with the specified tags.
	 * @param BeliefTags The tags identifying the beliefs.
	 * @return True if all beliefs exist, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasBeliefs(const FGameplayTagContainer BeliefTags) const;

	/**
	 * Checks if the agent has any belief with the specified tags.
	 * @param BeliefTags The tags identifying the beliefs.
	 * @return True if any belief exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasAnyBelief(const FGameplayTagContainer BeliefTags) const;

	/**
	 * Checks if the agent has an action with the specified tag in the current plan stack.
	 * @param ActionTag The tag identifying the action.
	 * @return True if the action exists in the stack, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasActionInStack(const FGameplayTag ActionTag) const;

	/**
	 * Checks if the agent has all actions with the specified tags in the current plan stack.
	 * @param ActionTags The tags identifying the actions.
	 * @return True if all actions exist in the stack, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasActionsInStack(const FGameplayTagContainer ActionTags) const;

	/**
	 * Checks if the agent has any action with the specified tags in the current plan stack.
	 * @param ActionTags The tags identifying the actions.
	 * @return True if any action exists in the stack, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasAnyActionInStack(const FGameplayTagContainer ActionTags) const;

	/**
	 * Checks if the agent has an action with the specified tag.
	 * @param ActionTag The tag identifying the action.
	 * @return True if the action exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasAction(const FGameplayTag ActionTag) const;

	/**
	 * Checks if the agent has all actions with the specified tags.
	 * @param ActionTags The tags identifying the actions.
	 * @return True if all actions exist, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasActions(const FGameplayTagContainer ActionTags) const;

	/**
	 * Checks if the agent has any action with the specified tags.
	 * @param ActionTags The tags identifying the actions.
	 * @return True if any action exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasAnyAction(const FGameplayTagContainer ActionTags) const;

	/**
	 * Checks if the agent has a goal with the specified tag.
	 * @param GoalTag The tag identifying the goal.
	 * @return True if the goal exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasGoal(const FGameplayTag GoalTag) const;

	/**
	 * Checks if the agent has all goals with the specified tags.
	 * @param GoalTags The tags identifying the goals.
	 * @return True if all goals exist, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasGoals(const FGameplayTagContainer GoalTags) const;

	/**
	 * Checks if the agent has any goal with the specified tags.
	 * @param GoalTags The tags identifying the goals.
	 * @return True if any goal exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool HasAnyGoal(const FGameplayTagContainer GoalTags) const;

	/**
	 * Finds and returns a sensor with the specified tag.
	 * @param SensorTag The tag identifying the sensor.
	 * @return The sensor if found, nullptr otherwise.
	 */
	UAgentSensor* FindSensor(const FGameplayTag SensorTag) const;

	/**
	 * Finds and returns a belief with the specified tag.
	 * @param BeliefTag The tag identifying the belief.
	 * @return The belief if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintPure)
	UAgentBelief* FindBelief(const FGameplayTag BeliefTag) const;

	/**
	 * Finds and returns an action with the specified tag.
	 * @param ActionTag The tag identifying the action.
	 * @return The action if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintPure)
	UAgentAction* FindAction(const FGameplayTag ActionTag) const;

	/**
	 * Finds and returns a goal with the specified tag.
	 * @param GoalTag The tag identifying the goal.
	 * @return The goal if found, nullptr otherwise.
	 */
	UFUNCTION(BlueprintPure)
	UAgentGoal* FindGoal(const FGameplayTag GoalTag) const;

	UAgentContext* GetContext() const;

	/**
	 * Gets the actor that owns this GOAP agent.
	 * @return The owning actor.
	 */
	UFUNCTION(BlueprintPure)
	AActor* GetAgentActor() const;

	/**
	 * Gets the pawn that owns this GOAP agent.
	 * @return The owning pawn.
	 */
	UFUNCTION(BlueprintPure)
	APawn* GetAgentPawn() const;

	/**
	 * Gets the controller of the pawn that owns this GOAP agent.
	 * @return The owning controller.
	 */
	UFUNCTION(BlueprintPure)
	AAIController* GetAgentController() const;

	/**
	 * Gets the blackboard of the agent.
	 * @return The agent's blackboard.
	 */
	UFUNCTION(BlueprintPure)
	UGOAPBlackboard* GetAgentBlackboard() const;

	/**
	 * Gets the team blackboard of the agent.
	 * @return The agent's global blackboard.
	 */
	UFUNCTION(BlueprintPure)
	UGOAPBlackboard* GetAgentGlobalBlackboard() const;

	/**
	 * Clears the current plan of the agent if the agent allows replanning,
	 * allowing the agent to evaluate and create a new plan.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearPlan();

	void ApplyRequestedPlan(const FPlan& NewPlan);

protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	virtual void SetupSensors();

	virtual void SetupBeliefs();

	virtual void SetupActions();

	virtual void SetupGoals();

private:
	void Init();

	void AutoRunBehaviour();

	void Reset();

	void ClearStates();

	void ProcessBehaviour(const float DeltaTime);

	void UpdatePlanIfNeeded();

	void UpdateCurrentAction(const float DeltaTime);

	void UpdateSensors(const float DeltaTime);

	void RequestNewPlan();

	void CreateContext();

	void AddSensor(const FGameplayTag SensorTag, UAgentSensor* Sensor);

	void AddBelief(const FGameplayTag BeliefTag, UAgentBelief* Belief);

	void AddAction(const FGameplayTag ActionTag, UAgentAction* Action);

	void AddGoal(const FGameplayTag GoalTag, UAgentGoal* Goal);

	void SetupBlackboard() const;

	void UnbindEvents();

	bool CanReplan() const;

	UFUNCTION()
	void OnBeliefStateChanged(UAgentBelief* Belief, bool bNewState);

	UFUNCTION()
	void OnActionStarted(UAgentContext* InAgentContext, UAgentAction* Action);

	UFUNCTION()
	void OnActionFinished(UAgentContext* InAgentContext, UAgentAction* Action, bool bSuccess);

	UFUNCTION()
	void OnGoalAchieved(UAgentContext* InAgentContext, UAgentGoal* Goal);

	bool Check() const;

#if !UE_BUILD_SHIPPING
	void DrawDebugMentalState() const;
#endif
};
