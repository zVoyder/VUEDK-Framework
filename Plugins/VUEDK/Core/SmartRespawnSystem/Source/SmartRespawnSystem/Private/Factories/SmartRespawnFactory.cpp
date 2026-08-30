// Copyright VUEDK, Inc. All Rights Reserved.

#include "Factories/SmartRespawnFactory.h"
#include "SmartRespawn.h"
#include "SmartRespawnSystem.h"
#include "SmartRespawnSystemSettings.h"

USmartRespawnRegisterSaverBase* USmartRespawnFactory::CreateRegisterSaver(USmartRespawnSubsystem* SmartRespawnSubsystem)
{
	const UClass* SaveClass = GetDefault<USmartRespawnSystemSettings>()->RegisterSaverClass.LoadSynchronous();
	if (!IsValid(SaveClass))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnFactory::CreateRegisterSaver: Invalid RegisterSaverClass in settings. Using default USmartRespawnRegisterSlotSaver."));
		SaveClass = USmartRespawnRegisterSlotSaver::StaticClass();
	}

	USmartRespawnRegisterSaverBase* Saver = NewObject<USmartRespawnRegisterSaverBase>(SmartRespawnSubsystem, SaveClass);
	if (!IsValid(Saver))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnFactory::CreateRegisterSaver: Failed to create instance of RegisterSaverClass."));
		return nullptr;
	}

	Saver->Init(SmartRespawnSubsystem);
	return Saver;
}

USmartRespawnBehaviour* USmartRespawnFactory::CreateRespawnBehaviour(USmartRespawnSubsystem* SmartRespawnSubsystem, const FSmartRespawnSnapshot& RespawnSnapshot)
{
	if (!IsValid(SmartRespawnSubsystem))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnFactory::CreateRespawnBehaviour: Invalid SmartRespawnSubsystem reference."));
		return nullptr;
	}

	// Create custom behaviour from snapshot if specified
	const USmartRespawn* SmartRespawn = RespawnSnapshot.SmartRespawn.LoadSynchronous();
	if (IsValid(SmartRespawn) && SmartRespawn->bUseCustomRespawn && IsValid(SmartRespawn->CustomRespawnBehaviour))
	{
		USmartRespawnBehaviour* CustomBehaviour = DuplicateObject<USmartRespawnBehaviour>(SmartRespawn->CustomRespawnBehaviour, SmartRespawnSubsystem);
		if (!IsValid(CustomBehaviour))
		{
			UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnFactory::CreateRespawnBehaviour: Failed to duplicate custom respawn behaviour from SmartRespawn %s."), *SmartRespawn->GetName());
			return nullptr;
		}
		
		CustomBehaviour->Init(SmartRespawnSubsystem);
		return CustomBehaviour;
	}

	// Create default behaviour from settings
	const UClass* RespawnClass = GetDefault<USmartRespawnSystemSettings>()->RespawnBehaviourClass.LoadSynchronous();
	if (!IsValid(RespawnClass))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnFactory::CreateRespawnBehaviour: Invalid RespawnBehaviourClass in settings. Using default USmartRespawnTranslationBehaviour."));
		RespawnClass = USmartRespawnTranslationBehaviour::StaticClass();
	}
	
	USmartRespawnBehaviour* RespawnBehaviour = NewObject<USmartRespawnBehaviour>(SmartRespawnSubsystem, RespawnClass);
	if (!IsValid(RespawnBehaviour))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnFactory::CreateRespawnBehaviour: Failed to create instance of RespawnBehaviourClass."));
		return nullptr;
	}
	
	RespawnBehaviour->Init(SmartRespawnSubsystem);
	return RespawnBehaviour;
}

USmartRespawnSelectorBase* USmartRespawnFactory::CreateRespawnSelector(USmartRespawnSubsystem* SmartRespawnSubsystem, TSubclassOf<USmartRespawnSelectorBase> SelectorClass)
{
	if (!IsValid(SelectorClass))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnFactory::CreateRespawnSelector: Invalid SelectorClass provided. Using default USmartRespawnSelectorMostRecent."));
		SelectorClass = USmartRespawnSelectorMostRecent::StaticClass();
	}

	USmartRespawnSelectorBase* Selector = NewObject<USmartRespawnSelectorBase>(SmartRespawnSubsystem, SelectorClass);
	if (!IsValid(Selector))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnFactory::CreateRespawnSelector: Failed to create instance of RespawnSelectorClass."));
		return nullptr;
	}

	return Selector;
}

ARespawnStreamingSource* USmartRespawnFactory::SpawnRespawnStreamingSource(const USmartRespawnSubsystem* SmartRespawnSubsystem, APlayerController* PlayerController, const FGameplayTag& RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot)
{
	if (!IsValid(SmartRespawnSubsystem))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnFactory::SpawnRespawnStreamingSource: Invalid SmartRespawnSubsystem reference."));
		return nullptr;
	}

	UWorld* World = SmartRespawnSubsystem->GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnFactory::SpawnRespawnStreamingSource: SmartRespawnSubsystem has no valid world reference."));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ARespawnStreamingSource* StreamingSource = World->SpawnActor<ARespawnStreamingSource>(ARespawnStreamingSource::StaticClass(), RespawnSnapshot.RespawnTransform.GetLocation(), RespawnSnapshot.RespawnTransform.GetRotation().Rotator(), SpawnParams);
	if (!IsValid(StreamingSource))
	{
		UE_LOG(LogSmartRespawnSystem, Warning, TEXT("USmartRespawnFactory::SpawnRespawnStreamingSource: Failed to spawn ARespawnStreamingSource."));
		return nullptr;
	}

	StreamingSource->Init(PlayerController, RespawnTag, RespawnSnapshot);
	return StreamingSource;
}
