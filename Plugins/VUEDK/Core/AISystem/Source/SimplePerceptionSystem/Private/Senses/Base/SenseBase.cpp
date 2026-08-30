// Copyright VUEDK, Inc. All Rights Reserved.

#include "Senses/Base/SenseBase.h"
#include "SimplePerceiver.h"
#include "SimplePerceptionSystem.h"

void USenseBase::Init(USimplePerceiver* InSimplePerceiver)
{
	Super::Init(InSimplePerceiver);
	InitSensors(InSimplePerceiver);
	InitValidators(InSimplePerceiver);
}

void USenseBase::Process(const float DeltaTime)
{
	ProcessSensors(DeltaTime);
}

void USenseBase::SenseActor(AActor* Actor)
{
	if (!FilterSensedActor(Actor))
		return;

	USimplePerceiver* Perceiver = GetPerceiver();
	if (!IsValid(Perceiver))
		return;
	
	Perceiver->SenseActor(Actor, this);
}

bool USenseBase::ValidateSensedActor(AActor* Actor) const
{
	return !Validators.IsEmpty() && FilterSensedActor(Actor);
}

void USenseBase::InitSensors(USimplePerceiver* InSimplePerceiver)
{
	for (USensorBase* Sensor : Sensors)
	{
		if (!IsValid(Sensor))
		{
			UE_LOG(LogSimplePerceptionSystem, Warning, TEXT("USenseBase::Init: Invalid Sensor in Sense %s."), *GetName());
			continue;
		}

		Sensor->SetupSense(this);
		Sensor->Init(InSimplePerceiver);
	}
}

void USenseBase::InitValidators(USimplePerceiver* InSimplePerceiver)
{
	for (USenseValidatorBase* Validator : Validators)
	{
		if (!IsValid(Validator))
		{
			UE_LOG(LogSimplePerceptionSystem, Warning, TEXT("USenseBase::Init: Invalid Validator in Sense %s."), *GetName());
			continue;
		}

		Validator->Init(InSimplePerceiver);
	}
}

void USenseBase::ProcessSensors(const float DeltaTime)
{
	for (USensorBase* Sensor : Sensors)
	{
		if (!IsValid(Sensor))
			continue;

		Sensor->Process(DeltaTime);
	}
}

bool USenseBase::FilterSensedActor(AActor* Actor) const
{
	if (!IsValid(Actor))
		return false;

	for (const USenseValidatorBase* Validator : Validators)
	{
		if (!IsValid(Validator))
			continue;

		if (!Validator->Validate(Actor))
			return false;
	}

	return true;
}
