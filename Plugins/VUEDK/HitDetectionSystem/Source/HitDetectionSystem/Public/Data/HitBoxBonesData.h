// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Engine/DataAsset.h"
#include "HitBoxBonesData.generated.h"

UCLASS()
class HITDETECTIONSYSTEM_API UHitBoxBonesData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Physics asset used to fill the bones damage multipliers map (Editor only)"), Category = "HitBox|Editor")
	UPhysicsAsset* PhysicsAsset;
#endif

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitBox")
	TMap<FName, float> BonesDamageMultipliers;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
