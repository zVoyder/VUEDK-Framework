// Copyright VUEDK, Inc. All Rights Reserved.

#include "Agent/AgentAction.h"
#include "VGOAP.h"
#include "Agent/AgentBelief.h"
#include "Agent/GOAPAgent.h"

void UAgentAction::Init(UAgentContext* InContext)
{
	Super::Init(InContext);

	ResetCost();
	if (IsValid(PriorityOperation))
		PriorityOperation->Init(InContext);

	if (IsValid(CostOperation))
		CostOperation->Init(InContext);
}

bool UAgentAction::RegisterPrecondition(const FGameplayTag PreconditionTag, const bool bDesiredValue)
{
	if (!IsValid(GetAgent()))
		return false;

	UAgentBelief* PreconditionBelief = GetAgent()->FindBelief(PreconditionTag);
	if (!IsValid(PreconditionBelief))
		return false;

	const FGOAPBeliefCondition BeliefCondition = FGOAPBeliefCondition(PreconditionBelief, bDesiredValue);
	Preconditions.Add(BeliefCondition);
	return true;
}

bool UAgentAction::RegisterEffect(const FGameplayTag EffectTag, const bool bDesiredValue)
{
	if (!IsValid(GetAgent()))
		return false;

	UAgentBelief* EffectBelief = GetAgent()->FindBelief(EffectTag);
	if (!IsValid(EffectBelief))
		return false;

	const FGOAPBeliefCondition BeliefCondition = FGOAPBeliefCondition(EffectBelief, bDesiredValue);
	Effects.Add(BeliefCondition);
	return true;
}

void UAgentAction::UnregisterPrecondition(const FGameplayTag PreconditionTag)
{
	if (!IsValid(GetAgent()))
		return;

	const UAgentBelief* EffectBelief = GetAgent()->FindBelief(PreconditionTag);
	if (!IsValid(EffectBelief))
		return;

	Preconditions.RemoveAll([EffectBelief](const FGOAPBeliefCondition& Condition)
	{
		return Condition.Belief == EffectBelief;
	});
}

void UAgentAction::UnregisterEffect(const FGameplayTag EffectTag)
{
	if (!IsValid(GetAgent()))
		return;

	const UAgentBelief* EffectBelief = GetAgent()->FindBelief(EffectTag);
	if (!IsValid(EffectBelief))
		return;

	Effects.RemoveAll([EffectBelief](const FGOAPBeliefCondition& Condition)
	{
		return Condition.Belief == EffectBelief;
	});
}

void UAgentAction::RegisterPreconditions(const TArray<FGOAPBeliefEntryData>& InPreconditions)
{
	for (const auto& Precondition : InPreconditions)
	{
		if (!RegisterPrecondition(Precondition.BeliefTag, Precondition.bValue))
			UE_LOG(LogVGOAP, Warning, TEXT("UAgentAction::RegisterPreconditions: Failed to add precondition '%s' to action '%s' of GOAP Agent '%s'."), *Precondition.BeliefTag.ToString(), *GetName(), *GetAgent()->GetOwner()->GetName());
	}
}

void UAgentAction::RegisterEffects(const TArray<FGOAPBeliefEntryData>& InEffects)
{
	for (const auto& Effect : InEffects)
	{
		if (!RegisterEffect(Effect.BeliefTag, Effect.bValue))
			UE_LOG(LogVGOAP, Warning, TEXT("UAgentAction::RegisterEffects: Failed to add effect '%s' to action '%s' of GOAP Agent '%s'."), *Effect.BeliefTag.ToString(), *GetName(), *GetAgent()->GetOwner()->GetName());
	}
}

void UAgentAction::SetPriority(const float NewPriority)
{
	CurrentPriority = ClampValueInRange(NewPriority, PriorityRange);
}

void UAgentAction::ResetPriority()
{
	CurrentPriority = ClampValueInRange(BasePriority, PriorityRange);
}

float UAgentAction::GetPriority() const
{
	const float DynamicPriority = IsValid(PriorityOperation) ? PriorityOperation->GetDynamicValue(CurrentPriority) : CurrentPriority;
	return ClampValueInRange(DynamicPriority, PriorityRange);
}

void UAgentAction::SetCost(const float NewCost)
{
	CurrentCost = ClampValueInRange(NewCost, CostRange);
}

void UAgentAction::ResetCost()
{
	CurrentCost = ClampValueInRange(BaseCost, CostRange);
}

float UAgentAction::GetCost() const
{
	const float DynamicCost = IsValid(CostOperation) ? CostOperation->GetDynamicValue(CurrentCost) : CurrentCost;
	return ClampValueInRange(DynamicCost, CostRange);
}

TArray<FGOAPBeliefCondition> UAgentAction::GetPreconditions() const
{
	return Preconditions;
}

TArray<FGOAPBeliefCondition> UAgentAction::GetEffects() const
{
	return Effects;
}

bool UAgentAction::ArePreconditionsMet() const
{
	for (const FGOAPBeliefCondition& Precondition : Preconditions)
	{
		if (!Precondition.CheckEvaluation()) // Preconditions are evaluated on each tick, so we use CheckEvaluation here to check the last known state
			return false;
	}

	return true;
}

bool UAgentAction::CanActionPerform_Implementation() const
{
	return true;
}

bool UAgentAction::IsCompleted() const
{
	return bIsCompleted;
}

bool UAgentAction::WasSuccessful() const
{
	return bWasSuccessful;
}

void UAgentAction::StartAction()
{
	ResetState();
	OnActionStart(GetAgentPawn(), GetAgentController(), GetAgentBlackboard());
	OnActionStarted.Broadcast(GetAgentContext(), this);

	if (bUseTimeout)
		StartTimeoutTimer();
}

void UAgentAction::PerformAction(const float DeltaTime)
{
	if (CanActionPerform())
		OnActionPerform(DeltaTime, GetAgentPawn(), GetAgentController(), GetAgentBlackboard());

	if (IsCompleted())
		return;

	for (const FGOAPBeliefCondition& Precondition : Preconditions)
		// ReSharper disable once CppExpressionWithoutSideEffects
		Precondition.Evaluate();
}

void UAgentAction::StopAction()
{
	ClearTimeoutTimer();
	OnActionStop(GetAgentPawn(), GetAgentController(), GetAgentBlackboard(), bWasSuccessful);
}

bool UAgentAction::TryGetPreconditionDesiredValue(const UAgentBelief* Belief, bool& OutDesiredValue) const
{
	for (const auto& Precondition : Preconditions)
	{
		if (Precondition.Belief == Belief)
		{
			OutDesiredValue = Precondition.bValue;
			return true;
		}
	}

	return false;
}

bool UAgentAction::TryGetEffectDesiredValue(const UAgentBelief* Belief, bool& OutDesiredValue) const
{
	for (const auto& Effect : Effects)
	{
		if (Effect.Belief == Belief)
		{
			OutDesiredValue = Effect.bValue;
			return true;
		}
	}

	return false;
}

void UAgentAction::FinishExecute(const bool bSuccess)
{
	ClearTimeoutTimer();
	
	UAgentAction* ThisAction = this;
	const auto CompleteAction = FTimerDelegate::CreateLambda([ThisAction, bSuccess]()
	{
		if (!IsValid(ThisAction))
			return;

		ThisAction->bIsCompleted = true;
		ThisAction->bWasSuccessful = bSuccess;
		ThisAction->OnActionFinished.Broadcast(ThisAction->GetAgentContext(), ThisAction, bSuccess);
	});

	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		CompleteAction.ExecuteIfBound();
		return;
	}

	// Use timer to delay completion to next tick to avoid issues with immediate state changes
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.SetTimerForNextTick(CompleteAction);
}

void UAgentAction::OnActionStart_Implementation(APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard)
{
}

void UAgentAction::OnActionPerform_Implementation(const float DeltaTime, APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard)
{
}

void UAgentAction::OnActionStop_Implementation(APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard, const bool bSuccess)
{
}

void UAgentAction::StartTimeoutTimer()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	World->GetTimerManager().SetTimer(TimeoutTimerHandle, this, &UAgentAction::FinishExecuteTimeout, TimeoutSeconds, false);
}

void UAgentAction::FinishExecuteTimeout()
{
	FinishExecute(false);
}

void UAgentAction::ClearTimeoutTimer()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;
	
	World->GetTimerManager().ClearTimer(TimeoutTimerHandle);
}

float UAgentAction::ClampValueInRange(const float InValue, const FFloatRange& InRange)
{
	float MinValue = FLT_MIN;
	float MaxValue = FLT_MAX;

	if (InRange.HasUpperBound())
		MaxValue = InRange.GetUpperBoundValue();

	if (InRange.HasLowerBound())
		MinValue = InRange.GetLowerBoundValue();

	return FMath::Clamp(InValue, MinValue, MaxValue);
}

void UAgentAction::ResetState()
{
	bIsCompleted = false;
	bWasSuccessful = true;
}
