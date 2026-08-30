// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/PerceptionConfigData.h"
#include "SimplePerceiver.generated.h"

class USenseBase;

UENUM(BlueprintType)
enum ESensedStatus : uint8
{
	None UMETA(DisplayName = "None"),
	Direct UMETA(DisplayName = "Direct"),
	Memory UMETA(DisplayName = "Memory")
};

USTRUCT(BlueprintType)
struct FSensedActorInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USenseBase> SensedBy;
	UPROPERTY(BlueprintReadOnly)
	FTransform SensedTransform;
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<ESensedStatus> SensedStatus;
	float SensedTime;

	FSensedActorInfo() : SensedBy(nullptr),
	                     SensedTransform(FTransform::Identity),
	                     SensedStatus(ESensedStatus::None),
	                     SensedTime(0.f)
	{
	}

	FSensedActorInfo(USenseBase* InSensedBy, const float InSensedTime, const FTransform& InSensedTransform, const ESensedStatus InSensedStatus) : SensedBy(InSensedBy),
	                                                                                                                                              SensedTransform(InSensedTransform),
	                                                                                                                                              SensedStatus(InSensedStatus),
	                                                                                                                                              SensedTime(InSensedTime)
	{
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPerceivedNewActor,
	AActor*, Actor,
	FSensedActorInfo, SensedActorInfo
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPerceivedActor,
	AActor*, Actor,
	FSensedActorInfo, SensedActorInfo
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPerceivedActorLost,
	AActor*, Actor,
	FSensedActorInfo, SensedActorInfo
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPerceivedActorForgotten,
	AActor*, Actor,
	FSensedActorInfo, SensedActorInfo
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnClearPerception
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMPLEPERCEPTIONSYSTEM_API USimplePerceiver : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPerceivedNewActor OnPerceivedNewActor;
	UPROPERTY(BlueprintAssignable)
	FOnPerceivedActor OnPerceivedActor;
	UPROPERTY(BlueprintAssignable)
	FOnPerceivedActorLost OnPerceivedActorLost;
	UPROPERTY(BlueprintAssignable)
	FOnPerceivedActorLost OnPerceivedActorForgotten;
	UPROPERTY(BlueprintAssignable)
	FOnClearPerception OnClearPerception;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception",
		meta = (ToolTip = "Offset from the actor's location to use as the sight location for perception."))
	FVector SightLocationOffset = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception")
	UPerceptionConfigData* PerceptionConfigData;

protected:
	TMap<TWeakObjectPtr<AActor>, FSensedActorInfo> PerceivedActors;

private:
	UPROPERTY()
	TArray<USenseBase*> Senses;

public:
	USimplePerceiver();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Deactivate() override;

	UFUNCTION(BlueprintCallable)
	void ClearPerception();

	UFUNCTION(BlueprintCallable)
	void PerceiveActor(AActor* Actor, const ESensedStatus SensedStatus);

	void SenseActor(AActor* Actor, USenseBase* SensedBy);

	UFUNCTION(BlueprintPure)
	bool IsPerceived(AActor* Actor) const;

	UFUNCTION(BlueprintPure)
	FSensedActorInfo GetPerceivedActorInfo(AActor* Actor) const;

	UFUNCTION(BlueprintPure)
	TMap<AActor*, FSensedActorInfo> GetPerceivedActors() const;

protected:
	virtual void BeginPlay() override;

private:
	void SetupSenses();

	void ProcessSenses(const float DeltaTime);

	void UpdatePerceivedActors();

	bool Check() const;

#if !UE_BUILD_SHIPPING
	void DrawDebugSensedActors() const;
#endif
};
