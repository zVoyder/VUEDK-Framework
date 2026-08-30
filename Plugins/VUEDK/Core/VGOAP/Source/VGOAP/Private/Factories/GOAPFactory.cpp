// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/GOAPFactory.h"

#include "VGOAP.h"
#include "Agent/AgentBelief.h"
#include "Agent/GOAPAgent.h"
#include "Agent/AgentContext.h"
#include "Utility/VGOAPUtility.h"

UAgentSensor* UGOAPFactory::CreateAgentSensor(UGOAPAgent* InAgent, const TSubclassOf<UAgentSensor> InSensorClass)
{
	if (!IsValid(InAgent) || !InSensorClass)
		return nullptr;

	UAgentSensor* NewSensor = NewObject<UAgentSensor>(InAgent, InSensorClass);
	if (!IsValid(NewSensor))
		return nullptr;

	NewSensor->Init(InAgent->GetContext());
	return NewSensor;
}

UAgentBelief* UGOAPFactory::CreateAgentBelief(UGOAPAgent* InAgent, const TSubclassOf<UAgentBelief> InBeliefClass)
{
	if (!IsValid(InAgent) || !InBeliefClass)
		return nullptr;

	UAgentBelief* NewBelief = NewObject<UAgentBelief>(InAgent, InBeliefClass);
	if (!IsValid(NewBelief))
		return nullptr;

	NewBelief->Init(InAgent->GetContext());
	return NewBelief;
}

UAgentAction* UGOAPFactory::CreateAgentAction(UGOAPAgent* InAgent, const TSubclassOf<UAgentAction> InActionClass, const TArray<FGOAPBeliefEntryData>& InPreconditions, const TArray<FGOAPBeliefEntryData>& InEffects)
{
	if (!IsValid(InAgent) || !InActionClass)
		return nullptr;

	UAgentAction* NewAction = NewObject<UAgentAction>(InAgent, InActionClass);
	if (!IsValid(NewAction))
		return nullptr;

	NewAction->Init(InAgent->GetContext());
	NewAction->RegisterPreconditions(InPreconditions);
	NewAction->RegisterEffects(InEffects);
	return NewAction;
}

UAgentGoal* UGOAPFactory::CreateAgentGoal(UGOAPAgent* InAgent, const TSubclassOf<UAgentGoal> InGoalClass, const TArray<FGOAPBeliefEntryData>& InDesiredEffects)
{
	if (!IsValid(InAgent) || !InGoalClass)
		return nullptr;

	UAgentGoal* NewGoal = NewObject<UAgentGoal>(InAgent, InGoalClass);
	if (!IsValid(NewGoal))
		return nullptr;

	NewGoal->Init(InAgent->GetContext());
	NewGoal->RegisterDesiredEffects(InDesiredEffects);
	return NewGoal;
}

UGOAPBlackboard* UGOAPFactory::CreateBlackboard(UGOAPBlackboardData* InBlackboardData)
{
	if (!IsValid(InBlackboardData))
		return nullptr;

	UGOAPBlackboard* NewBlackboard = NewObject<UGOAPBlackboard>(GetTransientPackage());
	if (!IsValid(NewBlackboard))
		return nullptr;

	NewBlackboard->Init(InBlackboardData);
	return NewBlackboard;
}

UAgentContext* UGOAPFactory::CreateAgentContext(UGOAPAgent* InAgent)
{
	UAgentContext* NewContext = NewObject<UAgentContext>(InAgent);
	if (!IsValid(NewContext))
		return nullptr;

	UGOAPBlackboard* Blackboard = CreateBlackboard(InAgent->BlackboardData);
	UGOAPBlackboard* GlobalBlackboard = UVGOAPUtility::GetGlobalBlackboard(InAgent->GlobalBlackboard);
	NewContext->Init(InAgent, Blackboard, GlobalBlackboard);
	return NewContext;
}

UAgentSensor* UGOAPFactory::CreateSensorWithTemplate(UGOAPAgent* InAgent, const UAgentSensor* InSensorTemplate)
{
	UAgentSensor* NewSensor = DuplicateObject<UAgentSensor>(InSensorTemplate, InAgent);
	if (!IsValid(NewSensor))
		return nullptr;

	NewSensor->Init(InAgent->GetContext());
	return NewSensor;
}

UAgentBelief* UGOAPFactory::CreateBeliefWithTemplate(UGOAPAgent* InAgent, const UAgentBelief* InBeliefTemplate)
{
	UAgentBelief* NewBelief = DuplicateObject<UAgentBelief>(InBeliefTemplate, InAgent);
	if (!IsValid(NewBelief))
		return nullptr;

	NewBelief->Init(InAgent->GetContext());
	return NewBelief;
}

UAgentAction* UGOAPFactory::CreateActionWithTemplate(UGOAPAgent* InAgent, const FGOAPActionCreationData& InActionData)
{
	const UAgentAction* ActionTemplate = InActionData.Action;
	if (!IsValid(ActionTemplate))
		return nullptr;

	UAgentAction* NewAction = DuplicateObject<UAgentAction>(ActionTemplate, InAgent);
	if (!IsValid(NewAction))
		return nullptr;

	NewAction->Init(InAgent->GetContext());
	NewAction->RegisterPreconditions(InActionData.Preconditions);
	NewAction->RegisterEffects(InActionData.Effects);
	return NewAction;
}

UAgentGoal* UGOAPFactory::CreateGoalWithTemplate(UGOAPAgent* InAgent, const FGOAPGoalCreationData& InGoalData)
{
	const UAgentGoal* GoalTemplate = InGoalData.Goal;
	if (!IsValid(GoalTemplate))
		return nullptr;

	UAgentGoal* NewGoal = DuplicateObject<UAgentGoal>(GoalTemplate, InAgent);
	if (!IsValid(NewGoal))
		return nullptr;

	NewGoal->Init(InAgent->GetContext());
	NewGoal->RegisterDesiredEffects(InGoalData.DesiredEffects);
	return NewGoal;
}
