// Copyright VUEDK, Inc. All Rights Reserved.

#include "Agent/AgentGoal.h"
#include "Agent/AgentBelief.h"
#include "Agent/GOAPAgent.h"
#include "VGOAP.h"

void UAgentGoal::Init(UAgentContext* InContext)
{
	Super::Init(InContext);
	ResetPriority();
	if (IsValid(PriorityOperation))
		PriorityOperation->Init(InContext);
}

bool UAgentGoal::RegisterDesiredEffect(const FGameplayTag EffectTag, const bool bDesiredValue)
{
	if (!IsValid(GetAgent()))
		return false;

	UAgentBelief* EffectBelief = GetAgent()->FindBelief(EffectTag);
	if (!IsValid(EffectBelief))
		return false;

	const FGOAPBeliefCondition BeliefCondition = FGOAPBeliefCondition(EffectBelief, bDesiredValue);
	DesiredEffects.Add(BeliefCondition);
	return true;
}

void UAgentGoal::UnregisterDesiredEffect(const FGameplayTag EffectTag)
{
	if (!IsValid(GetAgent()))
		return;

	const UAgentBelief* EffectBelief = GetAgent()->FindBelief(EffectTag);
	if (!IsValid(EffectBelief))
		return;

	DesiredEffects.RemoveAll([EffectBelief](const FGOAPBeliefCondition& Condition)
	{
		return Condition.Belief == EffectBelief;
	});
}

void UAgentGoal::RegisterDesiredEffects(const TArray<FGOAPBeliefEntryData>& InDesiredEffects)
{
	for (const auto& Effect : InDesiredEffects)
	{
		if (!RegisterDesiredEffect(Effect.BeliefTag, Effect.bValue))
			UE_LOG(LogVGOAP, Warning, TEXT("UAgentGoal::RegisterDesiredEffects: GOAP Agent '%s' failed to add desired effect '%s' to goal '%s'."), *GetAgent()->GetOwner()->GetName(), *Effect.BeliefTag.ToString(), *GetName());
	}
}

void UAgentGoal::SetPriority(const float NewPriority)
{
	CurrentPriority = ClampPriority(NewPriority);
}

void UAgentGoal::ResetPriority()
{
	CurrentPriority = ClampPriority(BasePriority);
	DynamicPriority = CurrentPriority;
}

float UAgentGoal::GetPriority()
{
	const float Priority = IsValid(PriorityOperation) ? PriorityOperation->GetDynamicValue(CurrentPriority) : CurrentPriority;
	DynamicPriority = ClampPriority(Priority);
	return DynamicPriority;
}

float UAgentGoal::GetDynamicPriority() const
{
	return DynamicPriority;
}

TArray<FGOAPBeliefCondition> UAgentGoal::GetDesiredEffects() const
{
	return DesiredEffects;
}

bool UAgentGoal::IsPursuable()
{
	if (!CanBeSatisfied())
		return false;

	return !EvaluateDesiredEffects();
}

bool UAgentGoal::IsSatisfied()
{
	if (!CanBeSatisfied())
		return false;

	return EvaluateDesiredEffects();
}

void UAgentGoal::AchieveGoal()
{
	if (!IsSatisfied())
		return;
	
	OnGoalAchieve(GetAgentPawn(), GetAgentController(), GetAgentBlackboard());
	OnGoalAchieved.Broadcast(GetAgentContext(), this);
}

bool UAgentGoal::CanBeSatisfied_Implementation()
{
	return true;
}

void UAgentGoal::OnGoalAchieve_Implementation(APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard)
{
}

float UAgentGoal::ClampPriority(const float InPriority) const
{
	float MinCost = FLT_MIN;
	float MaxCost = FLT_MAX;
	
	if (PriorityRange.HasUpperBound())
		MaxCost = PriorityRange.GetUpperBoundValue();

	if (PriorityRange.HasLowerBound())
		MinCost = PriorityRange.GetLowerBoundValue();
	
	return FMath::Clamp(InPriority, MinCost, MaxCost);
}

bool UAgentGoal::EvaluateDesiredEffects() const
{
	for (const FGOAPBeliefCondition& DesiredEffect : DesiredEffects)
	{
		if (!DesiredEffect.Evaluate())
			return false;
	}

	return true;
}
