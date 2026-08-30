// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Behaviours/ShootModes/Base/ShootModeBase.h"
#include "Shooter/Behaviours/ShooterBehaviourBase.h"

void UShootModeBase::Init(UShooterBehaviourBase* InBehaviour)
{
	Behaviour = InBehaviour;
}

void UShootModeBase::Tick(float DeltaTime)
{
	OnProcessingRequest(DeltaTime);
}

bool UShootModeBase::IsTickable() const
{
	return IsProcessingRequest();
}

TStatId UShootModeBase::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UShootMode, STATGROUP_Tickables);
}

bool UShootModeBase::RequestShoot(const EShootType ShootType)
{
	if (bIsProcessingRequest)
		return false;
	
	bIsProcessingRequest = true;
	return OnRequestShoot(ShootType);
}

void UShootModeBase::EndSequence()
{
	if (!bIsProcessingRequest)
		return;
	
	OnEndSequence();
}

bool UShootModeBase::IsProcessingRequest() const
{
	return bIsProcessingRequest;
}

void UShootModeBase::DeployShoot() const
{
	UShooterBehaviourBase* RelatedBehaviour = GetRelatedBehaviour();
	if (!IsValid(RelatedBehaviour))
		return;

	RelatedBehaviour->DeployShootOfType();
}

void UShootModeBase::CompleteProcessingRequest()
{
	bIsProcessingRequest = false;
}

UShooterBehaviourBase* UShootModeBase::GetRelatedBehaviour() const
{
	return Behaviour;
}

void UShootModeBase::FinishShootRequest(const bool bDeployShoot, const bool bSuccess, const bool bEndProcessing)
{
	if (bEndProcessing)
		CompleteProcessingRequest();

	OnShootRequestHandled.ExecuteIfBound(this, bDeployShoot, bSuccess);
}

bool UShootModeBase::OnRequestShoot_Implementation(EShootType ShootType)
{
	return true;
}

void UShootModeBase::OnProcessingRequest_Implementation(const float DeltaTime)
{
}

void UShootModeBase::OnEndSequence_Implementation()
{
}

bool UShootModeBase::Check() const
{
	return IsValid(Behaviour);
}
