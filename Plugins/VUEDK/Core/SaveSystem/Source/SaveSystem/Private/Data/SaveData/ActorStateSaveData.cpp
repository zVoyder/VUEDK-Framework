// Copyright VUEDK, Inc. All Rights Reserved.

#include "Data/SaveData/ActorStateSaveData.h"
#include "SaveSystem.h"

void UActorStateSaveData::Init(UActorStateSaveBehaviour* InBehaviour)
{
	Behaviour = InBehaviour;
}

bool UActorStateSaveData::SaveObjectData(UObject* ObjectToSave)
{
	if (!Check())
		return false;

	const AActor* Actor = Cast<AActor>(ObjectToSave);
	if (!IsValid(Actor))
	{
		UE_LOG(LogSaveSystem, Warning, TEXT("UActorStateSaveData::SaveObjectData: ObjectToSave is not a valid Actor."));
		return false;
	}

	if (Behaviour->bSaveLocation)
		SaveLocation(Actor->GetActorLocation());

	if (Behaviour->bSaveRotation)
		SaveRotation(Actor->GetActorRotation());

	if (Behaviour->bSaveScale)
		SaveScale(Actor->GetActorScale3D());

	UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
	if (!IsValid(PrimComp) || !PrimComp->IsSimulatingPhysics())
		return true;

	if (Behaviour->bSaveLinearVelocity)
		SaveLinearVelocity(PrimComp->GetPhysicsLinearVelocity());

	if (Behaviour->bSaveAngularVelocity)
		SaveAngularVelocity(PrimComp->GetPhysicsAngularVelocityInDegrees());

	if (Behaviour->bSaveMass)
		SaveMass(PrimComp->GetMass());

	return Super::SaveObjectData(ObjectToSave);
}

bool UActorStateSaveData::LoadObjectData(UObject* ObjectToLoad)
{
	if (!Check())
		return false;

	AActor* Actor = Cast<AActor>(ObjectToLoad);
	if (!IsValid(Actor))
	{
		UE_LOG(LogSaveSystem, Warning, TEXT("UActorStateSaveData::LoadObjectDatatNative: ObjectToLoad is not a valid Actor."));
		return false;
	}

	if (Behaviour->bSaveLocation && HasLocation())
		Actor->SetActorLocation(GetLocation());

	if (Behaviour->bSaveRotation && HasRotation())
		Actor->SetActorRotation(GetRotation());

	if (Behaviour->bSaveScale && HasScale())
		Actor->SetActorScale3D(GetScale());

	UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
	if (!IsValid(PrimComp) || !PrimComp->IsSimulatingPhysics())
		return true;

	if (Behaviour->bSaveLinearVelocity && HasLinearVelocity())
		PrimComp->SetPhysicsLinearVelocity(GetLinearVelocity());

	if (Behaviour->bSaveAngularVelocity && HasAngularVelocity())
		PrimComp->SetPhysicsAngularVelocityInDegrees(GetAngularVelocity());

	if (Behaviour->bSaveMass && HasMass())
		PrimComp->SetMassOverrideInKg(NAME_None, GetMass(), true);

	return Super::LoadObjectData(ObjectToLoad);
}

void UActorStateSaveData::SaveLocation(const FVector& InLocation)
{
	SavedVectors.Add(ActorSaveKeys::Location, InLocation);
}

void UActorStateSaveData::SaveRotation(const FRotator& InRotation)
{
	SavedVectors.Add(ActorSaveKeys::Rotation, InRotation.Vector());
}

void UActorStateSaveData::SaveScale(const FVector& InScale)
{
	SavedVectors.Add(ActorSaveKeys::Scale, InScale);
}

void UActorStateSaveData::SaveLinearVelocity(const FVector& InVelocity)
{
	SavedVectors.Add(ActorSaveKeys::LinearVelocity, InVelocity);
}

void UActorStateSaveData::SaveAngularVelocity(const FVector& InVelocity)
{
	SavedVectors.Add(ActorSaveKeys::AngularVelocity, InVelocity);
}

void UActorStateSaveData::SaveMass(float InMass)
{
	SavedValues.Add(ActorSaveKeys::Mass, InMass);
}

FVector UActorStateSaveData::GetLocation() const
{
	if (const FVector* Loc = SavedVectors.Find(ActorSaveKeys::Location))
		return *Loc;

	return FVector::ZeroVector;
}

FRotator UActorStateSaveData::GetRotation() const
{
	if (const FVector* Rot = SavedVectors.Find(ActorSaveKeys::Rotation))
		return Rot->Rotation();

	return FRotator::ZeroRotator;
}

FVector UActorStateSaveData::GetScale() const
{
	if (const FVector* Scale = SavedVectors.Find(ActorSaveKeys::Scale))
		return *Scale;

	return FVector::OneVector;
}

FVector UActorStateSaveData::GetLinearVelocity() const
{
	if (const FVector* Vel = SavedVectors.Find(ActorSaveKeys::LinearVelocity))
		return *Vel;

	return FVector::ZeroVector;
}

FVector UActorStateSaveData::GetAngularVelocity() const
{
	if (const FVector* Vel = SavedVectors.Find(ActorSaveKeys::AngularVelocity))
		return *Vel;

	return FVector::ZeroVector;
}

float UActorStateSaveData::GetMass() const
{
	if (const float* Mass = SavedValues.Find(ActorSaveKeys::Mass))
		return *Mass;

	return 0.f;
}

bool UActorStateSaveData::HasLocation() const
{
	return SavedVectors.Contains(ActorSaveKeys::Location);
}

bool UActorStateSaveData::HasRotation() const
{
	return SavedVectors.Contains(ActorSaveKeys::Rotation);
}

bool UActorStateSaveData::HasScale() const
{
	return SavedVectors.Contains(ActorSaveKeys::Scale);
}

bool UActorStateSaveData::HasLinearVelocity() const
{
	return SavedVectors.Contains(ActorSaveKeys::LinearVelocity);
}

bool UActorStateSaveData::HasAngularVelocity() const
{
	return SavedVectors.Contains(ActorSaveKeys::AngularVelocity);
}

bool UActorStateSaveData::HasMass() const
{
	return SavedValues.Contains(ActorSaveKeys::Mass);
}

bool UActorStateSaveData::Check() const
{
	return IsValid(Behaviour);
}
