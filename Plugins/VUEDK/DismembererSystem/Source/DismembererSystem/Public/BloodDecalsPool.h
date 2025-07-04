// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BloodDecal.h"
#include "DismembererSystemSettings.h"
#include "Subsystems/WorldSubsystem.h"
#include "BloodDecalsPool.generated.h"

UCLASS()
class DISMEMBERERSYSTEM_API UBloodDecalsPool : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<TObjectPtr<ABloodDecal>> AvailableDecals;
	UPROPERTY()
	TArray<TObjectPtr<ABloodDecal>> SpawnedDecals;
	UPROPERTY()
	const UDismembererSystemSettings* DismembererSystemSettings;

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	void DestroyPool();
	
	ABloodDecal* AcquireDecal();
	
	void ReleaseDecal(ABloodDecal* InActor);

private:
	void SpawnInstances();

	void SpawnInstance();

	void PushDecal(ABloodDecal* Actor);

	ABloodDecal* PopDecal();

	bool IsEmpty() const;

	void SetDecalEnabled(ABloodDecal* Actor, bool bIsEnabled) const;

	static void ClearDecal(ABloodDecal* Actor);
};
