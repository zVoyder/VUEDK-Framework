// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraDataInterfaceExport.h"
#include "Data/DismemberableAvatarData.h"
#include "Data/DismemberableLimbData.h"
#include "Data/DismembererBloodsData.h"
#include "Dismemberer.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogDismembermentSystem, All, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DISMEMBERERSYSTEM_API UDismemberer : public UActorComponent
{
	GENERATED_BODY()

	friend class ADismemberedLimb;
	friend class UBloodParticlesNiagaraCallbackHandler;

public:
	// -- Data --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Data")
	UDismemberableAvatarData* DismemberableAvatarData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Data")
	UDismembererBloodsData* DismembererBloodsData;

	// -- Limb Physics --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Physics")
	bool bLimbApplyImpulseOnDamage = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Physics", meta = (EditCondition = "bLimbApplyImpulseOnDamage", EditConditionHides))
	float LimbImpulseMultiplier = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Physics")
	float LimbLinearDamping = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Physics")
	float LimbAngularDamping = 0.5f;

	// -- Blood Effects --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Blood")
	bool bLimbReceiveDecals = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Blood", meta = (ToolTip = "Socket name to orient blood spill effect. The rotation is calculated from the vector between the bone location and this socket location."))
	FName BloodSpillDirectionSource = FName("neck_01");
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Blood")
	float BloodSpillFXDepth = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Blood", meta = (ToolTip = "Rotation offset for blood spill effect when it is not attached to a socket."))
	FRotator BloodSpillFXRotationOffset = FRotator(-90.f, 0.f, 0.f);

private:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY()
	TMap<FName, UNiagaraComponent*> SpawnedBloodSpills;
	// Map of dismembered limbs with their bone root names as keys
	UPROPERTY()
	TMap<FName, ADismemberedLimb*> SpawnedDismemberedLimbs;
	UPROPERTY()
	UBloodParticlesNiagaraCallbackHandler* BloodParticlesNiagaraCallbackHandler;
	TArray<FName> DismemberedBonesNames;
	TArray<FName> DismemberablaLimbsNames;

public:
	UDismemberer();
	
	UFUNCTION(BlueprintCallable)
	void Init(USkeletalMeshComponent* InSkeletalMeshComponent);

	UFUNCTION(BlueprintCallable)
	void ReconstructLimb(const FName LimbRootBoneName, const bool bStopBloodSpill = true, const bool bDestroySpawnedLimb = true);

	UFUNCTION(BlueprintCallable)
	void ReconstructLimbs(TArray<FName> BoneNames, const bool bStopBloodSpill = true, const bool bDestroySpawnedLimb = true);

	UFUNCTION(BlueprintCallable)
	void ReconstructAllLimbs(const bool bStopBloodSpill = true, const bool bDestroySpawnedLimb = true);

	UFUNCTION(BlueprintCallable)
	void DismemberLimb(const FName BoneName, const FVector Impulse, const float Damage);

	UFUNCTION(BlueprintCallable)
	void DismemberLimbs(const TArray<FName> BoneNames, const FVector Impulse, const float Damage);

	UFUNCTION(BlueprintCallable)
	void DismemberRandomLimbs(int32 LimbsCount, const FVector Impulse, const float Damage);

	UFUNCTION(BlueprintCallable)
	void DismemberAllLimbs(const FVector Impulse, const float Damage);

	UFUNCTION(BlueprintCallable)
	void StopBloodSpill(const FName SpillBoneName);

	UFUNCTION(BlueprintCallable)
	void StopBloodSpills(const TArray<FName> SpillBoneNames);

	UFUNCTION(BlueprintCallable)
	void StopAllBloodSpills();

	UFUNCTION(BlueprintPure)
	bool IsLimbDismembered(const FName BoneName, const bool bCheckOnlyRootBone = false) const;

	UFUNCTION(BlueprintPure)
	bool IsLimbDismemberable(const FName BoneName) const;

	UFUNCTION(BlueprintPure)
	TArray<FName> GetDismemberedBonesNames() const;

	UFUNCTION(BlueprintPure)
	TArray<FName> GetDismemberedRootBonesNames() const;

	UFUNCTION(BlueprintPure)
	TArray<ADismemberedLimb*> GetDismemberedLimbs() const;

	FVector GetBloodSpillDirectionSourceLocation() const;

	USkeletalMeshComponent* GetSkeletalMeshComponent() const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

private:
	TArray<FName> SortBonesNames(const TArray<FName>& Bones) const;
	
	TArray<FName> GetChildBoneNames(const FName& BoneName) const;
	
	void InitDismemberableBones();

	void HideLimb(const FName& BoneName) const;

	void DetachLimb(const FDismemberableLimbData& LimbData, const FVector& Impulse);

	void ExplodeLimb(const FDismemberableLimbData& LimbData);

	bool FindLimb(const FName& BoneName, FDismemberableLimbData& OutLimbData) const;

	void SpawnLimbExplosionFX(const FDismemberableLimbData& LimbData) const;

	void SpawnBloodExplosionFX(const FVector& HitLocation) const;

	void SpawnBloodSpillFX(const FDismemberableLimbData& LimbData);

	UNiagaraSystem* GetRandomBloodSpillFX() const;

	UNiagaraSystem* GetRandomBloodExplosionFX() const;

	UMaterialInterface* GetRandomBloodStainDecal() const;

	void SpawnDisemberedLimb(const FDismemberableLimbData& LimbData, const FVector& Impulse);

	bool Check() const;
};
