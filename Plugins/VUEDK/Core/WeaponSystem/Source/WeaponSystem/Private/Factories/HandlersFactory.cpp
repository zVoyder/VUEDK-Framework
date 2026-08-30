// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/HandlersFactory.h"
#include "WeaponSystem.h"
#include "Shooter/Behaviours/ShooterBehaviourBase.h"
#include "Shooter/Behaviours/Handlers/Base/ShooterHandlerBase.h"
#include "Shooter/Behaviours/Handlers/CooldownHandler.h"
#include "Shooter/Behaviours/Handlers/RecoilHandler.h"
#include "Shooter/Behaviours/Handlers/SpreadHandler.h"
#include "Shooter/Behaviours/Handlers/ShootModesHandler.h"

template <typename T>
T* UHandlersFactory::CreateHandler(UShooterBehaviourBase* InBehaviour)
{
	static_assert(TIsDerivedFrom<T, UShooterHandlerBase>::Value, "T must derive from UShooterHandlerBase");

	T* Handler = NewObject<T>(InBehaviour);
	if (!Handler)
	{
		UE_LOG(LogShooter, Error, TEXT("UHandlersFactory::CreateHandler: Failed to create handler."));
		return nullptr;
	}

	Handler->Init(InBehaviour);
	return Handler;
}

template UCooldownHandler* UHandlersFactory::CreateHandler<UCooldownHandler>(UShooterBehaviourBase* InBehaviour);

template URecoilHandler* UHandlersFactory::CreateHandler<URecoilHandler>(UShooterBehaviourBase* InBehaviour);

template USpreadHandler* UHandlersFactory::CreateHandler<USpreadHandler>(UShooterBehaviourBase* InBehaviour);

template UShootModesHandler* UHandlersFactory::CreateHandler<UShootModesHandler>(UShooterBehaviourBase* InBehaviour);
