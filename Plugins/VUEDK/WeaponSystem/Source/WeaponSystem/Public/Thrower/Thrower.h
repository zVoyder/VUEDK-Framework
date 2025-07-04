// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ArrowComponent.h"
#include "Throwables/ThrowableBase.h"
#include "Components/SceneComponent.h"
#include "Thrower.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UThrower : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Thrower")
	FGameplayTag ThrowableTag;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif
	
public:
	UThrower();

	UFUNCTION(BlueprintCallable)
	void Throw(const float Force, const float AngularSpeed = 0.0f) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void ThrowAtDirection(const float Force, const float AngularSpeed, const FVector& Direction) const;
	
protected:
	AThrowableBase* GetThrowable() const;
};
