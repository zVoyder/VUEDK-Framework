// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/BattlePassUnlockable.h"
#include "UObject/Object.h"
#include "UnlockableBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnUnlockableStateChanged,
	const bool, bIsLocked
);

UCLASS(Abstract, NotBlueprintable, BlueprintType)
class BATTLEPASSSYSTEM_API UUnlockableBase : public UObject, public IBattlePassUnlockable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnUnlockableStateChanged OnUnlockableStateChanged;

private:
	bool bIsLocked = true;

public:
	virtual void Unlock() override;

	virtual void Lock() override;

	virtual void SetLockedState(const bool bLocked, const bool bNotify = true) override;

	UFUNCTION(BlueprintPure)
	virtual bool IsUnlocked() const override;

	UFUNCTION(BlueprintPure)
	virtual bool IsLocked() const override;

protected:
	virtual bool CanChangeState() const;

	void NotifyState() const;
};
