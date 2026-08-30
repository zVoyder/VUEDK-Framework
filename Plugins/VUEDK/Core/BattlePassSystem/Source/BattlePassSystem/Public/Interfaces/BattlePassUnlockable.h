// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BattlePassUnlockable.generated.h"

UINTERFACE(NotBlueprintable)
class UBattlePassUnlockable : public UInterface
{
	GENERATED_BODY()
};

class BATTLEPASSSYSTEM_API IBattlePassUnlockable
{
	GENERATED_BODY()

public:
	virtual void Unlock() = 0;

	virtual void Lock() = 0;
	
	virtual void SetLockedState(const bool bLocked, const bool bNotify = true) = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Battle Pass Unlockable")
	virtual bool IsUnlocked() const = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Battle Pass Unlockable")
	virtual bool IsLocked() const = 0;
};
