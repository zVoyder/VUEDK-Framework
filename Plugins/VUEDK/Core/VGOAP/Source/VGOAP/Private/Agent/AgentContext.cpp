// Copyright VUEDK, Inc. All Rights Reserved.

#include "Agent/AgentContext.h"
#include "Factories/GOAPFactory.h"

void UAgentContext::Init(UGOAPAgent* InAgent, UGOAPBlackboard* InBlackboard, UGOAPBlackboard* InGlobalBlackboard)
{
	Agent = InAgent;
	Blackboard = InBlackboard;
	GlobalBlackboard = InGlobalBlackboard;
	SetupContext(InAgent);
}

void UAgentContext::SetupContext(const UGOAPAgent* InAgent)
{
	AActor* Owner = InAgent->GetOwner();
	if (!IsValid(Owner))
	{
		OwnerActor = nullptr;
		return;
	}
	
	// In case the owner is an AIController
	if (Owner->IsA<AAIController>())
	{
		OwnerController = Cast<AAIController>(Owner);
		OwnerActor = OwnerController->GetPawn();
		OwnerPawn = Cast<APawn>(OwnerActor);
		OwnerController->GetOnNewPawnNotifier().AddUObject(this, &UAgentContext::PawnChanged);
		return;
	}

	// In case the owner is an Actor or Pawn
	OwnerActor = Owner;
	OwnerPawn = Cast<APawn>(OwnerActor);
	OwnerController = OwnerPawn ? Cast<AAIController>(OwnerPawn->GetController()) : nullptr;
}

void UAgentContext::PawnChanged(APawn* Pawn)
{
	if (!IsValid(Pawn))
		return;

	if (Pawn == OwnerPawn)
		return;

	OwnerController->GetOnNewPawnNotifier().RemoveAll(this);
	SetupContext(Agent);
}
