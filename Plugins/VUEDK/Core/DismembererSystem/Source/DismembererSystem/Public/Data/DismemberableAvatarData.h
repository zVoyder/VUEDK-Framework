// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DismemberableLimbData.h"
#include "Engine/DataAsset.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "DismemberableAvatarData.generated.h"

UCLASS()
class DISMEMBERERSYSTEM_API UDismemberableAvatarData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Physics asset used to fill the dismemberable limbs list (Editor only)"), Category = "Avatar|Editor")
	UPhysicsAsset* PhysicsAsset;
#endif

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Avatar")
	TSet<FDismemberableLimbData> DismemberableLimbs;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Avatar")
	float LimbsScaleFactor = 1.f;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
