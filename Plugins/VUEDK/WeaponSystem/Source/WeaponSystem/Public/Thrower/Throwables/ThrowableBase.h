// Copyright zVoyder, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PooledActorBase.h"
#include "Thrower/Data/EThrowableSpinAxis.h"
#include "ThrowableBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class WEAPONSYSTEM_API AThrowableBase : public APooledActorBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwable")
	float LifeTime = 5.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Throwable")
	TEnumAsByte<EThrowableSpinAxis> ThrowableSpinAxis;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY()
	AActor* ThrowableInstigator;
	FTimerHandle LifeSpanHandle;

public:
	AThrowableBase();

	void Init(AActor* InInstigator);
	
	UStaticMeshComponent* GetMeshComponent() const;
	
	FVector GetSpinAxis() const;

	UFUNCTION(BlueprintNativeEvent)
	FVector GetCustomAxisSpin() const;
	
protected:
	virtual void OnPooledActorBeginPlay_Implementation() override;

	virtual void ClearPooledActor_Implementation() override;

	void InvalidateLifeSpanTimer();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnThrowableLifeSpanEnd();

private:
	UFUNCTION()
	void EndThrowableLifeSpan();
	
	void SetLifeSpan(const float LifeSpan);
};
