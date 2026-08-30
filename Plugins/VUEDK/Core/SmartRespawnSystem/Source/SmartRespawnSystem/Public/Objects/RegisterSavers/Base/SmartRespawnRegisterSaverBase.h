// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Base/SmartRespawnObjectBase.h"
#include "Data/SmartRespawnRegister.h"
#include "UObject/Object.h"
#include "SmartRespawnRegisterSaverBase.generated.h"

DECLARE_DELEGATE_OneParam(
	FOnRegisterRestored,
	const bool /* bSuccess */
);

UCLASS(Abstract, Blueprintable, BlueprintType)
class SMARTRESPAWNSYSTEM_API USmartRespawnRegisterSaverBase : public USmartRespawnObjectBase
{
	GENERATED_BODY()

public:
	FOnRegisterRestored OnRegisterRestored;

private:
	bool bIsRestored = false;
	bool bIsRestoring = false;
	
public:
	void SaveRegister(const FSmartRespawnRegister& RespawnsRegister);

	void RestoreRegister();
	
	UFUNCTION(BlueprintPure)
	bool IsRestored() const;

	UFUNCTION(BlueprintPure)
	bool IsRestoring() const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnSaveRegister(const FSmartRespawnRegister& RespawnsRegister);

	UFUNCTION(BlueprintNativeEvent)
	void OnRestoreRegister();

	UFUNCTION(BlueprintNativeEvent)
	void OnRestoreFinish(const bool bSuccess);
	
	UFUNCTION(BlueprintCallable)
	void FinishRestore(const bool bSuccess);

	UFUNCTION(BlueprintCallable)
	void SetRegister(const FSmartRespawnRegister& RespawnsRegister);
};
