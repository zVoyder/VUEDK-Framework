// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Shooter/Data/ShootData.h"
#include "ShooterHandlerBase.generated.h"

class UShootBarrel;
class UShooterBehaviourBase;

UCLASS(Abstract, NotBlueprintable)
class WEAPONSYSTEM_API UShooterHandlerBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UShooterBehaviourBase* Behaviour;
	FShootData* ShootData;

private:
	bool bIsInitialized = false;

public:
	void Init(UShooterBehaviourBase* InBehaviour, FShootData& InShootData);

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;

protected:
	virtual void OnInit();

	virtual void BindEvents();
	
	UFUNCTION()
	virtual void OnBehaviourEnabled();
	
	UFUNCTION()
	virtual void OnBehaviourDisabled();
	
	UFUNCTION()
	virtual void OnBehaviourShootSuccess(UShootBarrel* ShootBarrel, int32 ShotIndex);

	UFUNCTION()
	virtual void OnBehaviourShootFail();

	virtual bool Check() const;
};
