// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/HandlersFactory.h"

#include "Shooter/Behaviours/ShooterBehaviourBase.h"

URecoilHandler* UHandlersFactory::CreateRecoilHandler(UShooterBehaviourBase* InBehaviour, FShootData& ShootData)
{
	URecoilHandler* RecoilHandler = NewObject<URecoilHandler>(InBehaviour);
	RecoilHandler->Init(InBehaviour, ShootData);
	return RecoilHandler;
}

UCooldownHandler* UHandlersFactory::CreateCooldownHandler(UShooterBehaviourBase* InBehaviour, FShootData& ShootData)
{
	UCooldownHandler* CooldownHandler = NewObject<UCooldownHandler>(InBehaviour);
	CooldownHandler->Init(InBehaviour, ShootData);
	return CooldownHandler;
}

USpreadHandler* UHandlersFactory::CreateSpreadHandler(UShooterBehaviourBase* InBehaviour, FShootData& ShootData)
{
	USpreadHandler* SpreadHandler = NewObject<USpreadHandler>(InBehaviour);
	SpreadHandler->Init(InBehaviour, ShootData);
	return SpreadHandler;
}
