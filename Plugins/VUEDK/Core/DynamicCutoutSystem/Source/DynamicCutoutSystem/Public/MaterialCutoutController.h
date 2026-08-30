// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MaterialCutoutController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnTransitionStarted
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnTransitionCompleted
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnTransitionStopped
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnTransitionResumed
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnTransitionPaused
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DYNAMICCUTOUTSYSTEM_API UMaterialCutoutController : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnTransitionStarted OnTransitionStarted;
	UPROPERTY(BlueprintAssignable)
	FOnTransitionCompleted OnTransitionCompleted;
	UPROPERTY(BlueprintAssignable)
	FOnTransitionStopped OnTransitionStopped;
	UPROPERTY(BlueprintAssignable)
	FOnTransitionResumed OnTransitionResumed;
	UPROPERTY(BlueprintAssignable)
	FOnTransitionPaused OnTransitionPaused;

	UPROPERTY(EditDefaultsOnly, Category = "Material Cutout Controller")
	bool bAutoSetDynamicMaterials = true;
	UPROPERTY(EditDefaultsOnly, Category = "Material Cutout Controller", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DefaultCutoutAmount = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Material Cutout Controller|Advanced")
	FMaterialParameterInfo CutoutParameterInfo;

private:
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;
	float CurrentCutoutAmount = 0.f;
	float StartTransitionCutoutAmount = 0.f;
	float TargetTransitionCutoutAmount = 0.f;
	float TransitionDuration = 0.0f;
	float TransitionTimer = 0.0f;
	bool bIsPerformingTransition = false;

public:
	UMaterialCutoutController();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartTransition(const float Duration, const float FromCutoutAmount, const float ToCutoutAmount);

	UFUNCTION(BlueprintCallable)
	void StopTransition();

	UFUNCTION(BlueprintCallable)
	void ResumeTransition();

	UFUNCTION(BlueprintCallable)
	void PauseTransition();

	UFUNCTION(BlueprintCallable)
	void SetMaterials(const TArray<UMaterialInstanceDynamic*>& InMaterials);

	UFUNCTION(BlueprintCallable)
	void AddMaterial(UMaterialInstanceDynamic* InMaterial);

	UFUNCTION(BlueprintCallable)
	void RemoveMaterial(UMaterialInstanceDynamic* InMaterial);

	UFUNCTION(BlueprintCallable)
	void ClearMaterials();

	UFUNCTION(BlueprintCallable)
	void SetCutoutAmount(const float InCutoutAmount);

	UFUNCTION(BlueprintCallable)
	void ModifyCutoutAmount(const float InCutoutAmount);

	UFUNCTION(BlueprintPure)
	float GetCurrentCutoutAmount() const;
	
	UFUNCTION(BlueprintPure)
	TArray<UMaterialInstanceDynamic*> GetMaterials() const;

private:
	void Init();

	void AutoCreateAndSetDynamicMaterials();

	void PerformTransition(const float DeltaTime);
};
