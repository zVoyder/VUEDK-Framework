// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shooter/Data/ShootType.h"
#include "UObject/Object.h"
#include "ShootModeBase.generated.h"

class UShooterBehaviourBase;

DECLARE_DYNAMIC_DELEGATE_ThreeParams(
	FOnShootRequestHandled,
	UShootModeBase*, ShootMode,
	bool, bDeployShoot,
	bool, bSuccess
);

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class WEAPONSYSTEM_API UShootModeBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	FOnShootRequestHandled OnShootRequestHandled;
	
private:
	UPROPERTY()
	UShooterBehaviourBase* Behaviour;
	bool bIsProcessingRequest = false;

public:
	void Init(UShooterBehaviourBase* InBehaviour);

	virtual void Tick(float DeltaTime) override;
	
	virtual bool IsTickable() const override;
	
	virtual TStatId GetStatId() const override;

	bool RequestShoot(const EShootType ShootType);

	void EndSequence();

	bool IsProcessingRequest() const;

protected:
	/**
	 * Deploys the shoot of this related shooter behaviour.
	 */
	UFUNCTION(BlueprintCallable)
	void DeployShoot() const;

	UFUNCTION(BlueprintCallable)
	void CompleteProcessingRequest();
	
	UFUNCTION(BlueprintPure)
	UShooterBehaviourBase* GetRelatedBehaviour() const;
	
	UFUNCTION(BlueprintCallable)
	void FinishShootRequest(const bool bDeployShoot = true, const bool bSuccess = true, const bool bEndProcessing = true);
	
	UFUNCTION(BlueprintNativeEvent)
	bool OnRequestShoot(EShootType ShootType);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnProcessingRequest(const float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void OnEndSequence();

	bool Check() const;
};
