// Copyright VUEDK, Inc. All Rights Reserved.

#include "Sensors/Base/SensorBase.h"
#include "Senses/Base/SenseBase.h"

void USensorBase::SetupSense(USenseBase* InSense)
{
	Sense = InSense;
}

void USensorBase::Process(const float DeltaTime)
{
	ReceiveProcess(DeltaTime);
}

void USensorBase::ReceiveProcess_Implementation(const float DeltaTime)
{
}

void USensorBase::SenseActor(AActor* Actor) const
{
	if (!IsValid(Sense))
		return;

	Sense->SenseActor(Actor);
}
