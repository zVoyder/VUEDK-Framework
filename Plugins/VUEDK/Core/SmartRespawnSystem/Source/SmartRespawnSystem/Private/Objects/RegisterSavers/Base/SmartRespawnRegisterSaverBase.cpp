// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/RegisterSavers/Base/SmartRespawnRegisterSaverBase.h"
#include "SmartRespawnSubsystem.h"
#include "SmartRespawnSystem.h"
#include "Utility/SmartRespawnUtility.h"

void USmartRespawnRegisterSaverBase::SaveRegister(const FSmartRespawnRegister& RespawnsRegister)
{
	OnSaveRegister(RespawnsRegister);
}

void USmartRespawnRegisterSaverBase::RestoreRegister()
{
	bIsRestoring = true;
	OnRestoreRegister();
}

bool USmartRespawnRegisterSaverBase::IsRestored() const
{
	return bIsRestored;
}

bool USmartRespawnRegisterSaverBase::IsRestoring() const
{
	return bIsRestoring;
}

void USmartRespawnRegisterSaverBase::OnSaveRegister_Implementation(const FSmartRespawnRegister& RespawnsRegister)
{
}

void USmartRespawnRegisterSaverBase::OnRestoreRegister_Implementation()
{
	FinishRestore(true);
}

void USmartRespawnRegisterSaverBase::OnRestoreFinish_Implementation(const bool bSuccess)
{
}

void USmartRespawnRegisterSaverBase::FinishRestore(const bool bSuccess)
{
	bIsRestored = bSuccess;
	bIsRestoring = false;
	OnRestoreFinish(bSuccess);
	OnRegisterRestored.ExecuteIfBound(bSuccess);
}

void USmartRespawnRegisterSaverBase::SetRegister(const FSmartRespawnRegister& RespawnsRegister)
{
	USmartRespawnUtility::SetRespawnsRegister(RespawnsRegister);
}
