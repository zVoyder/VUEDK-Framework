// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BloodDecalSpawnParams.h"
#include "NiagaraSystem.h"
#include "Engine/DataAsset.h"
#include "DismembererBloodsData.generated.h"

UCLASS()
class DISMEMBERERSYSTEM_API UDismembererBloodsData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|VFXs")
	TArray<UNiagaraSystem*> HitEffects;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|VFXs")
	TArray<UNiagaraSystem*> BloodSpillEffects;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay, Category = "Dismember|VFXs")
	FName NiagaraCallbackName = FName("User.Callback");
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Decals")
	TArray<UMaterialInterface*> BloodStainsDecals;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Decals")
	FBloodDecalSpawnParams BloodDecalSpawnParams;
};
