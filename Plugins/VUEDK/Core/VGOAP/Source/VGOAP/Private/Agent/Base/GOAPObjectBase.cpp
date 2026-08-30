// Copyright VUEDK, Inc. All Rights Reserved.

#include "Agent/Base/GOAPObjectBase.h"
#include "Agent/AgentContext.h"

void UGOAPObjectBase::Init(UAgentContext* InContext)
{
	AgentContext = InContext;
	OnInit();
}

UAgentContext* UGOAPObjectBase::GetAgentContext() const
{
	return AgentContext;
}

UGOAPAgent* UGOAPObjectBase::GetAgent() const
{
	return AgentContext ? AgentContext->Agent : nullptr;
}

AActor* UGOAPObjectBase::GetAgentActor() const
{
	return AgentContext ? AgentContext->OwnerActor : nullptr;
}

APawn* UGOAPObjectBase::GetAgentPawn() const
{
	return AgentContext ? AgentContext->OwnerPawn : nullptr;
}

AAIController* UGOAPObjectBase::GetAgentController() const
{
	return AgentContext ? AgentContext->OwnerController : nullptr;
}

UGOAPBlackboard* UGOAPObjectBase::GetAgentBlackboard() const
{
	return AgentContext ? AgentContext->Blackboard : nullptr;
}

UGOAPBlackboard* UGOAPObjectBase::GetAgentGlobalBlackboard() const
{
	return AgentContext ? AgentContext->GlobalBlackboard : nullptr;
}

#if WITH_EDITOR
bool UGOAPObjectBase::ImplementsGetWorld() const
{
	return true;
}
#endif

UWorld* UGOAPObjectBase::GetWorld() const
{
	return IsValid(AgentContext) && IsValid(AgentContext->Agent) ? AgentContext->Agent->GetWorld() : Super::GetWorld();
}

void UGOAPObjectBase::OnInit_Implementation()
{
}
