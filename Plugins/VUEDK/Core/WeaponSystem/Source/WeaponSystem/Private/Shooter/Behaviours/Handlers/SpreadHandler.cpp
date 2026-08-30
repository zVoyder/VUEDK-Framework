// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Behaviours/Handlers/SpreadHandler.h"
#include "Shooter/Behaviours/ShooterBehaviourBase.h"

void USpreadHandler::TickHandler(float DeltaTime)
{
	ProcessSpread(DeltaTime);

	switch (CurrentDynamicSpreadState)
	{
	case DynamicAddState:
		ProcessAddDynamicSpread(DeltaTime);
		break;

	case DynamicRecoverState:
		ProcessRecoverDynamicSpread(DeltaTime);
		break;
	default: ;
	}

	if (IsProcessingSpread())
		OnProcessingSpread.Broadcast(GetSpread());
}

void USpreadHandler::AddDynamicSpread(float AddSpread, const float ChangeRate, const float RecoveryRate)
{
	const UShootData* ShootData = GetShootData();
	if (!IsValid(ShootData))
		return;
	
	if (!ShootData->bHasSpread)
		return;
	
	if (BaseSpread + DynamicSpread + AddSpread >= ShootData->MaxSpread)
		AddSpread = ShootData->MaxSpread - (BaseSpread + DynamicSpread);

	if (AddSpread <= 0.f)
		return;

	TargetDynamicSpread = DynamicSpread + AddSpread;
	DynamicSpreadChangeRate = ChangeRate;
	DynamicSpreadRecoveryRate = RecoveryRate;
	DynamicSpreadDir = TargetDynamicSpread > DynamicSpread ? 1.f : -1.f;
	TransitionToDynamicAddState();
	OnStartedChangeSpread.Broadcast(GetDefaultSpread(), GetSpread());
}

void USpreadHandler::AddDynamicSpreadWithCurve()
{
	const UShootData* ShootData = GetShootData();
	if (!IsValid(ShootData))
		return;
	
	if (!ShootData->bHasSpread)
		return;
	
	if (!IsValid(ShootData->SpreadCurve))
		return;
	
	AddDynamicSpread( 
		ShootData->SpreadCurve->GetFloatValue(Behaviour->GetShootHandledRequestsCount()),
		ShootData->SpreadChangeRate,
		ShootData->SpreadRecoveryRate
	);
}

void USpreadHandler::InstantSetSpread(const float InSpread, const bool bOverrideDefault)
{
	const UShootData* ShootData = GetShootData();
	if (!IsValid(ShootData))
		return;
	
	if (!ShootData->bHasSpread)
		return;
	
	if (bOverrideDefault)
		Behaviour->SetDefaultSpread(InSpread);

	BaseSpread = InSpread;
}

void USpreadHandler::SetSpread(float InSpread, const float ChangeRate, const bool bOverrideDefault)
{
	const UShootData* ShootData = GetShootData();
	if (!IsValid(ShootData))
		return;
	
	if (!ShootData->bHasSpread)
		return;
	
	if (InSpread + DynamicSpread >= ShootData->MaxSpread)
		InSpread = ShootData->MaxSpread - DynamicSpread;

	bOverrideDefaultSpread = bOverrideDefault;
	TargetSpread = InSpread;
	SpreadChangeRate = ChangeRate;
	SpreadDir = TargetSpread > BaseSpread ? 1.f : -1.f;
	bIsProcessingSpread = true;
	OnStartedChangeSpread.Broadcast(GetDefaultSpread(), GetSpread());
}

void USpreadHandler::ResetSpread(const float ChangeRate)
{
	if (!Check())
		return;
	
	SetSpread(Behaviour->GetDefaultSpread(), ChangeRate);
}

float USpreadHandler::GetSpread() const
{
	if (!Check())
		return 0.f;

	const UShootData* ShootData = GetShootData();
	if (!ShootData->bHasSpread)
		return 0.f;
	
	return FMath::Clamp(BaseSpread + DynamicSpread, 0.f, ShootData->MaxSpread);
}

bool USpreadHandler::IsProcessingSpread() const
{
	return bIsProcessingSpread || CurrentDynamicSpreadState != DynamicIdleState;
}

void USpreadHandler::OnInit()
{
	const UShootData* ShootData = GetShootData();
	if (!IsValid(ShootData))
		return;
	
	BaseSpread = ShootData->DefaultSpread;
}

void USpreadHandler::SetDefaultSpread(float NewDefaultSpread) const
{
	if (!Check())
		return;

	Behaviour->SetDefaultSpread(BaseSpread);
}

float USpreadHandler::GetDefaultSpread() const
{
	if (!Check())
		return 0.f;

	return Behaviour->GetDefaultSpread();
}

void USpreadHandler::ProcessSpread(const float DeltaTime)
{
	if (!bIsProcessingSpread)
		return;

	const float Step = SpreadChangeRate * DeltaTime;
	BaseSpread += Step * SpreadDir;

	if (bOverrideDefaultSpread)
		SetDefaultSpread(BaseSpread);

	if (SpreadDir > 0.f)
	{
		if (BaseSpread >= TargetSpread)
			EndSpreadChange();
	}
	else
	{
		if (BaseSpread <= TargetSpread)
			EndSpreadChange();
	}
}

void USpreadHandler::ProcessAddDynamicSpread(const float DeltaTime)
{
	const float Step = DynamicSpreadChangeRate * DeltaTime;
	DynamicSpread += Step * DynamicSpreadDir;

	if (DynamicSpreadDir > 0.f)
	{
		if (DynamicSpread >= TargetDynamicSpread)
			EndDynamicAddSpreadChange();
	}
	else
	{
		if (DynamicSpread <= TargetDynamicSpread)
			EndDynamicAddSpreadChange();
	}
}

void USpreadHandler::ProcessRecoverDynamicSpread(const float DeltaTime)
{
	const float Step = DynamicSpreadRecoveryRate * DeltaTime;
	DynamicSpread -= Step * DynamicSpreadDir;
	
	if (DynamicSpreadDir > 0.f)
	{
		if (DynamicSpread <= 0.f)
			EndDynamicRecoverSpreadChange();
	}
	else
	{
		if (DynamicSpread >= 0.f)
			EndDynamicRecoverSpreadChange();
	}
}

void USpreadHandler::EndSpreadChange()
{
	BaseSpread = TargetSpread;
	bIsProcessingSpread = false;

	if (bOverrideDefaultSpread)
	{
		SetDefaultSpread(BaseSpread);
		bOverrideDefaultSpread = false;
	}
	
	OnCompletedChangeSpread.Broadcast(GetDefaultSpread(), GetSpread());
}

void USpreadHandler::EndDynamicAddSpreadChange()
{
	DynamicSpread = TargetDynamicSpread;
	TransitionToDynamicRecoverState();
}

void USpreadHandler::EndDynamicRecoverSpreadChange()
{
	DynamicSpread = 0.f;
	TransitionToDynamicIdleState();
	OnCompletedChangeSpread.Broadcast(GetDefaultSpread(), GetSpread());
}

void USpreadHandler::TransitionToDynamicIdleState()
{
	CurrentDynamicSpreadState = DynamicIdleState;
}

void USpreadHandler::TransitionToDynamicAddState()
{
	CurrentDynamicSpreadState = DynamicAddState;
}

void USpreadHandler::TransitionToDynamicRecoverState()
{
	CurrentDynamicSpreadState = DynamicRecoverState;
}
