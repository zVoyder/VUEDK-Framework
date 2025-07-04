// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "ShootPoint.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), NotBlueprintable)
class WEAPONSYSTEM_API UShootPoint : public USceneComponent
{
	GENERATED_BODY()

private:
	float SpreadRad;
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif

public:
	UShootPoint();

	void SetSpreadDegree(const float NewSpreadDegree);
	
	void GenerateSpreadDegree(const float MaxSpreadDegree);

	UFUNCTION(BlueprintPure)
	float GetSpreadRad() const;
	
	UFUNCTION(BlueprintPure)
	FVector GetShootPointLocation() const;

	UFUNCTION(BlueprintPure)
	FVector GetShootPointRelativeLocation() const;

	UFUNCTION(BlueprintPure)
	FVector GetShootPointDirection() const;

	UFUNCTION(BlueprintPure)
	FVector GetShootPointRelativeDirection() const;

	UFUNCTION(BlueprintPure)
	FVector GetShootPointSpreadedDirection() const;

	UFUNCTION(BlueprintPure)
	FVector GetShootPointSpreadedRelativeDirection() const;

protected:
	virtual void OnRegister() override;
};
