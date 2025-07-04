// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShootBarrel.h"
#include "Behaviours/ShooterBehaviourBase.h"
#include "Components/ActorComponent.h"
#include "Data/ShootData.h"
#include "Shooter.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooter, All, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UShooter : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	UShooterBehaviourBase* ShooterBehaviour;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FShootData ShootData;
	
public:
	UShooter();

	UFUNCTION(BlueprintCallable)
	void Init(UShootBarrel* InShootBarrel);

	UFUNCTION(BlueprintCallable)
	void SetOwner(APawn* InOwner) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool Shoot() const;
	
	UFUNCTION(BlueprintPure)
	FShootData GetShootData() const;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool Check() const;
};
