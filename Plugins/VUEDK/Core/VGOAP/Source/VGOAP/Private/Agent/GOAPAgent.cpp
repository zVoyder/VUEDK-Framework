// Copyright VUEDK, Inc. All Rights Reserved.

#include "Agent/GOAPAgent.h"
#include "VGOAP.h"
#include "Agent/AgentContext.h"
#include "Factories/GOAPFactory.h"
#include "HAL/IConsoleManager.h"

UGOAPAgent::UGOAPAgent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGOAPAgent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ProcessBehaviour(DeltaTime);
}

void UGOAPAgent::RunBehaviour()
{
	if (GetOwnerRole() != ROLE_Authority)
		return;

	if (!Check())
		return;

	Reset();
	SetupSensors();
	SetupBeliefs();
	SetupActions();
	SetupGoals();
	SetupBlackboard();
	
	bIsProcessingBehaviour = true;
	OnRunBehaviour.Broadcast();
}

void UGOAPAgent::StopBehaviour(const bool bClearData)
{
	Reset();

	if (bClearData)
	{
		BlackboardData = nullptr;
		BehaviourData = nullptr;
	}
	
	OnStopBehaviour.Broadcast();
}

void UGOAPAgent::ResumeBehaviour()
{
	bIsProcessingBehaviour = true;
	OnResumeBehaviour.Broadcast();
}

void UGOAPAgent::PauseBehaviour()
{
	bIsProcessingBehaviour = false;
	OnPauseBehaviour.Broadcast();
}

void UGOAPAgent::ChangeBehaviour(UGOAPAgentBehaviourData* NewBehaviourData, const bool bRunImmediately)
{
	if (!IsValid(NewBehaviourData))
		return;
	
	Reset();
	BehaviourData = NewBehaviourData;
	if (bRunImmediately)
		RunBehaviour();
}

void UGOAPAgent::ChangeBlackboard(UGOAPBlackboardData* NewBlackboardData) const
{
	if (!IsValid(AgentContext))
		return;

	UGOAPBlackboard* Blackboard = UGOAPFactory::CreateBlackboard(NewBlackboardData);
	AgentContext->Blackboard = Blackboard;
}

bool UGOAPAgent::IsRunning() const
{
	return bIsProcessingBehaviour;
}

bool UGOAPAgent::TryAddSensor(const FGameplayTag SensorTag, UAgentSensor* Sensor, const bool bReplan)
{
	if (!IsValid(Sensor) || HasSensor(SensorTag))
		return false;

	if (Sensor->GetAgent() != this)
	{
		UE_LOG(LogVGOAP, Warning, TEXT("UGOAPAgent::TryAddSensor: Sensor %s does not belong to this agent."), *SensorTag.ToString());
		return false;
	}

	AddSensor(SensorTag, Sensor);
	if (bReplan)
		ClearPlan();
	
	return true;
}

bool UGOAPAgent::TryAddBelief(const FGameplayTag BeliefTag, UAgentBelief* Belief, const bool bReplan)
{
	if (!IsValid(Belief) || HasBelief(BeliefTag))
		return false;

	if (Belief->GetAgent() != this)
	{
		UE_LOG(LogVGOAP, Warning, TEXT("UGOAPAgent::TryAddBelief: Belief %s does not belong to this agent."), *BeliefTag.ToString());
		return false;
	}

	AddBelief(BeliefTag, Belief);
	if (bReplan)
		ClearPlan();
	return true;
}

bool UGOAPAgent::TryAddAction(const FGameplayTag ActionTag, UAgentAction* Action, const bool bReplan)
{
	if (!IsValid(Action) || HasAction(ActionTag))
		return false;

	if (Action->GetAgent() != this)
	{
		UE_LOG(LogVGOAP, Warning, TEXT("UGOAPAgent::TryAddAction: Action %s does not belong to this agent."), *ActionTag.ToString());
		return false;
	}

	AddAction(ActionTag, Action);
	if (bReplan)
		ClearPlan();
	return true;
}

bool UGOAPAgent::TryAddGoal(const FGameplayTag GoalTag, UAgentGoal* Goal, const bool bReplan)
{
	if (!IsValid(Goal) || HasGoal(GoalTag))
		return false;

	if (Goal->GetAgent() != this)
	{
		UE_LOG(LogVGOAP, Warning, TEXT("UGOAPAgent::TryAddGoal: Goal %s does not belong to this agent."), *GoalTag.ToString());
		return false;
	}

	AddGoal(GoalTag, Goal);
	if (bReplan)
		ClearPlan();
	return true;
}

UAgentSensor* UGOAPAgent::RemoveSensor(const FGameplayTag SensorTag)
{
	UAgentSensor* Sensor = Sensors.FindRef(SensorTag);
	if (!IsValid(Sensor))
		return nullptr;

	Sensors.Remove(SensorTag);
	Sensor->ClearTag();
	return Sensor;
}

UAgentBelief* UGOAPAgent::RemoveBelief(const FGameplayTag BeliefTag)
{
	UAgentBelief* Belief = Beliefs.FindRef(BeliefTag);
	if (!IsValid(Belief))
		return nullptr;

	// Unregister the belief from all actions and goals, could have used events but this is safer
	for (const auto& ActionPair : Actions)
	{
		UAgentAction* Action = ActionPair.Value;
		if (!IsValid(Action))
			continue;

		Action->UnregisterEffect(BeliefTag);
		Action->UnregisterPrecondition(BeliefTag);
	}

	for (const auto& GoalPair : Goals)
	{
		UAgentGoal* Goal = GoalPair.Value;
		if (!IsValid(Goal))
			continue;

		Goal->UnregisterDesiredEffect(BeliefTag);
	}

	Beliefs.Remove(BeliefTag);
	Belief->ClearTag();
	return Belief;
}

UAgentAction* UGOAPAgent::RemoveAction(const FGameplayTag ActionTag)
{
	UAgentAction* Action = FindAction(ActionTag);
	if (!IsValid(Action))
		return nullptr;

	Actions.Remove(ActionTag);
	Action->ClearTag();
	return Action;
}

UAgentGoal* UGOAPAgent::RemoveGoal(const FGameplayTag GoalTag)
{
	UAgentGoal* Goal = FindGoal(GoalTag);
	if (!IsValid(Goal))
		return nullptr;

	Goals.Remove(GoalTag);
	Goal->ClearTag();
	return Goal;
}

TMap<FGameplayTag, UAgentSensor*> UGOAPAgent::GetSensors() const
{
	return Sensors;
}

TMap<FGameplayTag, UAgentBelief*> UGOAPAgent::GetBeliefs() const
{
	return Beliefs;
}

TMap<FGameplayTag, UAgentAction*> UGOAPAgent::GetActions() const
{
	return Actions;
}

TArray<UAgentAction*> UGOAPAgent::GetActionsStackInPlan() const
{
	return CurrentPlan.IsValidPlan() ? CurrentPlan.Stack : TArray<UAgentAction*>();
}

TMap<FGameplayTag, UAgentAction*> UGOAPAgent::GetActionsMapInPlan() const
{
	return CurrentPlan.IsValidPlan() ? CurrentPlan.ActionsMap : TMap<FGameplayTag, UAgentAction*>();
}

TMap<FGameplayTag, UAgentGoal*> UGOAPAgent::GetGoals() const
{
	return Goals;
}

void UGOAPAgent::OnPreInit_Implementation()
{
}

void UGOAPAgent::OnSetupSensors_Implementation()
{
}

void UGOAPAgent::OnSetupBeliefs_Implementation()
{
}

void UGOAPAgent::OnSetupActions_Implementation()
{
}

void UGOAPAgent::OnSetupGoals_Implementation()
{
}

void UGOAPAgent::OnInit_Implementation()
{
}

bool UGOAPAgent::HasSensor(const FGameplayTag SensorTag) const
{
	return Sensors.Contains(SensorTag);
}

bool UGOAPAgent::HasBelief(const FGameplayTag BeliefTag) const
{
	return Beliefs.Contains(BeliefTag);
}

bool UGOAPAgent::HasBeliefs(const FGameplayTagContainer BeliefTags) const
{
	for (const FGameplayTag& BeliefTag : BeliefTags)
	{
		if (!HasBelief(BeliefTag))
			return false;
	}

	return true;
}

bool UGOAPAgent::HasAnyBelief(const FGameplayTagContainer BeliefTags) const
{
	for (const FGameplayTag& BeliefTag : BeliefTags)
	{
		if (HasBelief(BeliefTag))
			return true;
	}

	return false;
}

bool UGOAPAgent::HasActionInStack(const FGameplayTag ActionTag) const
{
	if (!CurrentPlan.IsValidPlan())
		return false;

	return CurrentPlan.ActionsMap.Contains(ActionTag);
}

bool UGOAPAgent::HasActionsInStack(const FGameplayTagContainer ActionTags) const
{
	for (const FGameplayTag& ActionTag : ActionTags)
	{
		if (!HasActionInStack(ActionTag))
			return false;
	}

	return true;
}

bool UGOAPAgent::HasAnyActionInStack(const FGameplayTagContainer ActionTags) const
{
	for (const FGameplayTag& ActionTag : ActionTags)
	{
		if (HasActionInStack(ActionTag))
			return true;
	}

	return false;
}

bool UGOAPAgent::HasAction(const FGameplayTag ActionTag) const
{
	return Actions.Contains(ActionTag);
}

bool UGOAPAgent::HasActions(const FGameplayTagContainer ActionTags) const
{
	for (const FGameplayTag& ActionTag : ActionTags)
	{
		if (!HasAction(ActionTag))
			return false;
	}

	return true;
}

bool UGOAPAgent::HasAnyAction(const FGameplayTagContainer ActionTags) const
{
	for (const FGameplayTag& ActionTag : ActionTags)
	{
		if (HasAction(ActionTag))
			return true;
	}

	return false;
}

bool UGOAPAgent::HasGoal(const FGameplayTag GoalTag) const
{
	return Goals.Contains(GoalTag);
}

bool UGOAPAgent::HasGoals(const FGameplayTagContainer GoalTags) const
{
	for (const FGameplayTag& GoalTag : GoalTags)
	{
		if (!HasGoal(GoalTag))
			return false;
	}

	return true;
}

bool UGOAPAgent::HasAnyGoal(const FGameplayTagContainer GoalTags) const
{
	for (const FGameplayTag& GoalTag : GoalTags)
	{
		if (HasGoal(GoalTag))
			return true;
	}

	return false;
}

UAgentSensor* UGOAPAgent::FindSensor(const FGameplayTag SensorTag) const
{
	return Sensors.FindRef(SensorTag);
}

UAgentBelief* UGOAPAgent::FindBelief(const FGameplayTag BeliefTag) const
{
	return Beliefs.FindRef(BeliefTag);
}

UAgentAction* UGOAPAgent::FindAction(const FGameplayTag ActionTag) const
{
	return Actions.FindRef(ActionTag);
}

UAgentGoal* UGOAPAgent::FindGoal(const FGameplayTag GoalTag) const
{
	return Goals.FindRef(GoalTag);
}

UAgentContext* UGOAPAgent::GetContext() const
{
	return AgentContext;
}

AActor* UGOAPAgent::GetAgentActor() const
{
	return IsValid(AgentContext) ? AgentContext->OwnerActor : nullptr;
}

APawn* UGOAPAgent::GetAgentPawn() const
{
	return IsValid(AgentContext) ? AgentContext->OwnerPawn : nullptr;
}

AAIController* UGOAPAgent::GetAgentController() const
{
	return IsValid(AgentContext) ? AgentContext->OwnerController : nullptr;
}

UGOAPBlackboard* UGOAPAgent::GetAgentBlackboard() const
{
	return IsValid(AgentContext) ? AgentContext->Blackboard : nullptr;
}

UGOAPBlackboard* UGOAPAgent::GetAgentGlobalBlackboard() const
{
	return IsValid(AgentContext) ? AgentContext->GlobalBlackboard : nullptr;
}

void UGOAPAgent::ClearPlan()
{
	CurrentGoal = nullptr;
	CurrentPlan.Clear();
	LastAction = nullptr;
	if (IsValid(CurrentAction))
		CurrentAction->StopAction();
	CurrentAction = nullptr;
	LastGoal = nullptr;
}

void UGOAPAgent::ApplyRequestedPlan(const FPlan& NewPlan)
{
	if (!NewPlan.IsValidPlan())
	{
		OnPlanFailed.Broadcast(AgentContext);
		return;
	}

	if (CurrentPlan.IsValidPlan())
	{
		const UAgentAction* NewAction = NewPlan.Peek();

		if (IsValid(CurrentAction) && IsValid(NewAction) && CurrentAction->GetTag() == NewAction->GetTag())
		{
			CurrentPlan = NewPlan;
			return;
		}

		if (IsValid(CurrentAction))
		{
			if (!CurrentAction->bAllowsReplanning)
				return;

			CurrentAction->StopAction();
		}
	}

	CurrentPlan = NewPlan;
	if (!CurrentPlan.IsCompleted())
	{
		CurrentGoal = CurrentPlan.AgentGoal;
		CurrentAction = CurrentPlan.Advance();

		if (CurrentAction->ArePreconditionsMet())
		{
			CurrentAction->StartAction();
		}
		else
		{
			CurrentAction = nullptr;
			CurrentGoal = nullptr;
		}
	}

	OnPlanCreated.Broadcast(AgentContext, CurrentPlan.AgentGoal);
}

void UGOAPAgent::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void UGOAPAgent::BeginDestroy()
{
	Super::BeginDestroy();
	UnbindEvents();
}

void UGOAPAgent::SetupSensors()
{
	for (const TPair<FGameplayTag, UAgentSensor*>& SensorPair : BehaviourData->Sensors)
	{
		if (HasSensor(SensorPair.Key))
			continue;

		UAgentSensor* Sensor = UGOAPFactory::CreateSensorWithTemplate(this, SensorPair.Value);
		if (!IsValid(Sensor))
		{
			UE_LOG(LogVGOAP, Warning, TEXT("UGOAPAgent::SetupSensors: GOAP Agent '%s' failed to create sensor '%s'."), *GetOwner()->GetName(), *SensorPair.Key.ToString());
			continue;
		}

		AddSensor(SensorPair.Key, Sensor);
	}

	OnSetupSensors();
}

void UGOAPAgent::SetupBeliefs()
{
	for (const TPair<FGameplayTag, UAgentBelief*>& BeliefPair : BehaviourData->Beliefs)
	{
		if (HasBelief(BeliefPair.Key))
			continue;

		UAgentBelief* Belief = UGOAPFactory::CreateBeliefWithTemplate(this, BeliefPair.Value);
		if (!IsValid(Belief))
		{
			UE_LOG(LogVGOAP, Warning, TEXT("UGOAPAgent::SetupBeliefs: GOAP Agent '%s' failed to create belief '%s'."), *GetOwner()->GetName(), *BeliefPair.Key.ToString());
			continue;
		}

		AddBelief(BeliefPair.Key, Belief);
	}

	OnSetupBeliefs();
}

void UGOAPAgent::SetupActions()
{
	for (const auto& ActionEntry : BehaviourData->Actions)
	{
		if (HasAction(ActionEntry.Key))
			continue;

		UAgentAction* Action = UGOAPFactory::CreateActionWithTemplate(this, ActionEntry.Value);
		if (!IsValid(Action))
		{
			UE_LOG(LogVGOAP, Warning, TEXT("UGOAPAgent::SetupActions: GOAP Agent '%s' failed to create action '%s'."), *GetOwner()->GetName(), *ActionEntry.Key.ToString());
			continue;
		}

		AddAction(ActionEntry.Key, Action);
	}

	OnSetupActions();
}

void UGOAPAgent::SetupGoals()
{
	for (const auto& GoalEntry : BehaviourData->Goals)
	{
		if (HasGoal(GoalEntry.Key))
			continue;

		UAgentGoal* Goal = UGOAPFactory::CreateGoalWithTemplate(this, GoalEntry.Value);
		if (!IsValid(Goal))
		{
			UE_LOG(LogVGOAP, Warning, TEXT("UGOAPAgent::SetupGoals: GOAP Agent '%s' failed to create goal '%s'."), *GetOwner()->GetName(), *GoalEntry.Key.ToString());
			continue;
		}

		AddGoal(GoalEntry.Key, Goal);
	}

	OnSetupGoals();
}

void UGOAPAgent::Init()
{
	OnPreInit();
	CreateContext();

	if (bAutoRunBehaviour)
		AutoRunBehaviour();

	OnInit();

#if !UE_BUILD_SHIPPING
	DrawDebugMentalState();
#endif
}

void UGOAPAgent::AutoRunBehaviour()
{
	const UWorld* World = GetWorld();

	if (AutoRunDelay <= 0.f || !IsValid(World))
	{
		RunBehaviour();
		return;
	}

	FTimerHandle TimerHandle;
	World->GetTimerManager().SetTimer(TimerHandle, this, &UGOAPAgent::RunBehaviour, AutoRunDelay, false);
}

void UGOAPAgent::Reset()
{
	bIsProcessingBehaviour = false;
	ClearStates();
	ClearPlan();
}

void UGOAPAgent::ClearStates()
{
	Sensors.Empty();
	Beliefs.Empty();
	Actions.Empty();
	Goals.Empty();
}

void UGOAPAgent::ProcessBehaviour(const float DeltaTime)
{
	if (!bIsProcessingBehaviour)
		return;

	UpdateSensors(DeltaTime);
	UpdatePlanIfNeeded();
	UpdateCurrentAction(DeltaTime);
}

void UGOAPAgent::UpdatePlanIfNeeded()
{
	if (!IsValid(CurrentAction) || !CurrentAction->ArePreconditionsMet())
	{
		RequestNewPlan();
		return;
	}

	if (CanReplan())
		RequestNewPlan();
}

void UGOAPAgent::UpdateCurrentAction(const float DeltaTime)
{
	if (!IsValid(CurrentAction) || !CurrentPlan.IsValidPlan())
		return;

	CurrentAction->PerformAction(DeltaTime);
	if (CurrentAction && CurrentAction->IsCompleted()) // Check if is valid again in case the action was stopped during PerformAction
	{
		CurrentAction->StopAction();
		LastAction = CurrentAction;
		CurrentAction = nullptr;

		if (CurrentPlan.IsCompleted())
		{
			if (IsValid(CurrentGoal))
				CurrentGoal->AchieveGoal();

			LastGoal = CurrentGoal;
			CurrentGoal = nullptr;
		}
	}
}

void UGOAPAgent::UpdateSensors(const float DeltaTime)
{
	for (const TPair<FGameplayTag, UAgentSensor*>& SensorPair : Sensors)
	{
		UAgentSensor* Sensor = SensorPair.Value;
		if (!IsValid(Sensor))
			continue;

		Sensor->UpdateSensor(DeltaTime);
	}
}

void UGOAPAgent::RequestNewPlan()
{
	Planner.RequestPlan(this, CurrentGoal, LastGoal, bAsyncPlanComputation);
}

void UGOAPAgent::CreateContext()
{
	AgentContext = UGOAPFactory::CreateAgentContext(this);
}

void UGOAPAgent::AddSensor(const FGameplayTag SensorTag, UAgentSensor* Sensor)
{
	Sensor->AssignTag(SensorTag);
	Sensors.Add(SensorTag, Sensor);
}

void UGOAPAgent::AddBelief(const FGameplayTag BeliefTag, UAgentBelief* Belief)
{
	Belief->AssignTag(BeliefTag);
	Beliefs.Add(BeliefTag, Belief);
	Belief->OnBeliefStateChanged.AddUniqueDynamic(this, &UGOAPAgent::OnBeliefStateChanged);
}

void UGOAPAgent::AddAction(const FGameplayTag ActionTag, UAgentAction* Action)
{
	Action->AssignTag(ActionTag);
	Actions.Add(ActionTag, Action);
	Action->OnActionStarted.AddUniqueDynamic(this, &UGOAPAgent::OnActionStarted);
	Action->OnActionFinished.AddUniqueDynamic(this, &UGOAPAgent::OnActionFinished);
}

void UGOAPAgent::AddGoal(const FGameplayTag GoalTag, UAgentGoal* Goal)
{
	Goal->AssignTag(GoalTag);
	Goals.Add(GoalTag, Goal);
	Goal->OnGoalAchieved.AddUniqueDynamic(this, &UGOAPAgent::OnGoalAchieved);
}

void UGOAPAgent::SetupBlackboard() const
{
	UGOAPBlackboard* Blackboard = UGOAPFactory::CreateBlackboard(BlackboardData);
	if (IsValid(AgentContext))
		AgentContext->Blackboard = Blackboard;
}

void UGOAPAgent::UnbindEvents()
{
	for (const TPair<FGameplayTag, UAgentBelief*>& BeliefPair : Beliefs)
	{
		if (IsValid(BeliefPair.Value))
			BeliefPair.Value->OnBeliefStateChanged.RemoveAll(this);
	}

	for (const auto& ActionPair : Actions)
	{
		UAgentAction* Action = ActionPair.Value;
		if (IsValid(Action))
		{
			Action->OnActionStarted.RemoveAll(this);
			Action->OnActionFinished.RemoveAll(this);
		}
	}

	for (const auto& GoalPair : Goals)
	{
		UAgentGoal* Goal = GoalPair.Value;
		if (IsValid(Goal))
			Goal->OnGoalAchieved.RemoveAll(this);
	}
}

bool UGOAPAgent::CanReplan() const
{
	return bCanEverReplan && (IsValid(CurrentAction) ? CurrentAction->bAllowsReplanning : true);
}

void UGOAPAgent::OnBeliefStateChanged(UAgentBelief* Belief, bool bNewState)
{
	OnAnyBeliefStateChanged.Broadcast(Belief, bNewState);
}

void UGOAPAgent::OnActionStarted(UAgentContext* InAgentContext, UAgentAction* Action)
{
	OnAnyActionStarted.Broadcast(InAgentContext, Action);
}

void UGOAPAgent::OnActionFinished(UAgentContext* InAgentContext, UAgentAction* Action, bool bSuccess)
{
	OnAnyActionFinished.Broadcast(InAgentContext, Action, bSuccess);
}

void UGOAPAgent::OnGoalAchieved(UAgentContext* InAgentContext, UAgentGoal* Goal)
{
	OnAnyGoalAchieved.Broadcast(InAgentContext, Goal);
}

bool UGOAPAgent::Check() const
{
	return IsValid(BehaviourData);
}

#if !UE_BUILD_SHIPPING
void UGOAPAgent::DrawDebugMentalState() const
{
	const bool bDebug = FVGOAPModule::CVarVGOAPDebug.GetValueOnGameThread();

	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	World->GetTimerManager().SetTimerForNextTick(this, &UGOAPAgent::DrawDebugMentalState);
	if (!bDebug || !IsRunning())
		return;

	const AActor* Actor = GetAgentActor();
	if (!IsValid(Actor) || Actor->IsHidden())
		return;

	FString DebugString;
	// Header
	DebugString += TEXT("[ GOAP AGENT ]\n");
	DebugString += TEXT("===============================\n");

	// Goal
	DebugString += TEXT("GOAL\n");
	DebugString += FString::Printf(
		TEXT("  Current : %s\n"),
		IsValid(CurrentGoal) ? *CurrentGoal->GetTag().ToString() : TEXT("None")
	);
	DebugString += FString::Printf(
		TEXT("  Last    : %s\n\n"),
		IsValid(LastGoal) ? *LastGoal->GetTag().ToString() : TEXT("None")
	);

	// Action
	DebugString += TEXT("ACTION\n");
	DebugString += FString::Printf(
		TEXT("  Current : %s\n"),
		IsValid(CurrentAction) ? *CurrentAction->GetTag().ToString() : TEXT("None")
	);
	DebugString += FString::Printf(
		TEXT("  Last    : %s\n\n"),
		IsValid(LastAction) ? *LastAction->GetTag().ToString() : TEXT("None")
	);

	// Plan stack
	DebugString += TEXT("PLAN STACK\n");

	const TArray<UAgentAction*> Stack = GetActionsStackInPlan();
	if (Stack.IsEmpty())
	{
		DebugString += TEXT("  <empty>\n");
	}
	else
	{
		for (int32 i = 0; i < Stack.Num(); ++i)
		{
			const UAgentAction* Action = Stack[i];
			if (!IsValid(Action))
				continue;

			DebugString += FString::Printf(
				TEXT("  [%d] %s\n"),
				i,
				*Action->GetTag().ToString()
			);
		}
	}

	DebugString += TEXT("===============================");

	DrawDebugString(
		World,
		FVector(0.f, 0.f, 120.f), // Offset above actor
		DebugString,
		GetAgentActor(),
		FColor::White,
		0.f,
		true,
		0.8f
	);
}
#endif
