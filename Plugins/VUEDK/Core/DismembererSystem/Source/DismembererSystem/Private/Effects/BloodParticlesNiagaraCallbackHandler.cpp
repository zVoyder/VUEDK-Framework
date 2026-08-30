// Copyright VUEDK, Inc. All Rights Reserved.

#include "Effects/BloodParticlesNiagaraCallbackHandler.h"
#include "DismembererSystem.h"
#include "Factories/DismembererFactory.h"

UBloodParticlesNiagaraCallbackHandler::UBloodParticlesNiagaraCallbackHandler(): Dismemberer(nullptr)
{
}

void UBloodParticlesNiagaraCallbackHandler::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);

	if (!Check())
		return;
	
	if (bIsDecalInCooldown || !IsValid(Dismemberer->DismembererBloodsData) || Dismemberer->DismembererBloodsData->BloodStainsDecals.IsEmpty())
		return;

	const UWorld* World = Dismemberer->GetWorld();

	if (!IsValid(World))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("UDismemberer::ReceiveParticleData_Implementation: Invalid world."));
		return;
	}

	UDismembererFactory::SpawnBloodDecal(
		World,
		Dismemberer->GetRandomBloodStainDecal(),
		Data[0].Position,
		Dismemberer->DismembererBloodsData->BloodDecalSpawnParams
	);

	bIsDecalInCooldown = true;
	FTimerHandle BloodDecalTimerHandle;
	World->GetTimerManager().SetTimer(BloodDecalTimerHandle, this, &UBloodParticlesNiagaraCallbackHandler::ResetDecalSpawnCooldown, UDismembererFactory::GetDismembererSettings()->DecalSpawnRate, false);
}

void UBloodParticlesNiagaraCallbackHandler::Init(UDismemberer* InDismemberer)
{
	Dismemberer = InDismemberer;
}

FName UBloodParticlesNiagaraCallbackHandler::GetNiagaraCallbackName() const
{
	if (!Check())
		return NAME_None;

	return Dismemberer->DismembererBloodsData->NiagaraCallbackName;
}	

void UBloodParticlesNiagaraCallbackHandler::ResetDecalSpawnCooldown()
{
	bIsDecalInCooldown = false;
}

bool UBloodParticlesNiagaraCallbackHandler::Check() const
{
	return IsValid(Dismemberer) && IsValid(Dismemberer->DismembererBloodsData) && !Dismemberer->DismembererBloodsData->BloodStainsDecals.IsEmpty();
}
