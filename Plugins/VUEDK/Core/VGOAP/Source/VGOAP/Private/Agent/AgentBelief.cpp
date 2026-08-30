// Copyright VUEDK, Inc. All Rights Reserved.

#include "Agent/AgentBelief.h"

void UAgentBelief::Init(UAgentContext* InContext)
{
	Super::Init(InContext);
	SetupInitialState();
}

bool UAgentBelief::Evaluate()
{
	const bool bBaseEvaluation = OnEvaluate(GetAgentPawn(), GetAgentController(), GetAgentBlackboard());
	const bool NewState = bBaseEvaluation == bValue;
	CheckState(NewState);
	bLastState = NewState;
	return bLastState;
}

bool UAgentBelief::GetLastState() const
{
	return bLastState;
}

bool UAgentBelief::OnEvaluate_Implementation(APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard)
{
	return false;
}

void UAgentBelief::SetupInitialState()
{
	const bool InitialState = OnEvaluate(GetAgentPawn(), GetAgentController(), GetAgentBlackboard()) == bValue;
	bLastState = InitialState;
}

void UAgentBelief::CheckState(const bool NewState)
{
	if (bLastState != NewState)
		OnBeliefStateChanged.Broadcast(this, NewState);
}
