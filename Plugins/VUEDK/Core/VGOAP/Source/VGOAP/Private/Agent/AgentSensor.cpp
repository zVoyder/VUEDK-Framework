// Copyright VUEDK, Inc. All Rights Reserved.

#include "Agent/AgentSensor.h"

void UAgentSensor::Init(UAgentContext* InContext)
{
	Super::Init(InContext);
	TimeSinceLastTick = UpdateInterval; // Ensure the sensor updates immediately on the first tick
}

void UAgentSensor::UpdateSensor(const float DeltaTime)
{
	if (!bCanEverUpdate)
		return;

	if (UpdateInterval <= 0.f)
	{
		OnUpdateSensor(DeltaTime, DeltaTime, GetAgentPawn(), GetAgentController(), GetAgentBlackboard());
		return;
	}

	TimeSinceLastTick += DeltaTime;
	if (TimeSinceLastTick >= UpdateInterval)
	{
		OnUpdateSensor(DeltaTime, TimeSinceLastTick, GetAgentPawn(), GetAgentController(), GetAgentBlackboard());
		TimeSinceLastTick = 0.f;
	}
}

void UAgentSensor::OnUpdateSensor_Implementation(const float DeltaTime, const float FixedDeltaTime, APawn* Pawn, AAIController* Controller, UGOAPBlackboard* Blackboard)
{
}
