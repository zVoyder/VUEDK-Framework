// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraDataInterfaceExport.h"
#include "Data/DismemberableAvatarData.h"
#include "Data/DismemberableLimbData.h"
#include "Data/DismembererBloodsData.h"
#include "Dismemberer.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DISMEMBERERSYSTEM_API UDismemberer : public UActorComponent
{
	GENERATED_BODY()

	friend class ADismemberedLimb;
	friend class UBloodParticlesNiagaraCallbackHandler;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Data")
	UDismemberableAvatarData* DismemberableAvatarData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Data")
	UDismembererBloodsData* DismembererBloodsData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Physics",
		meta = (ToolTip = "Defines the physics body operation to perform on the skeletal mesh component when a limb is hidden."))
	TEnumAsByte<EPhysBodyOp> LimbPhysicsBodyOp = EPhysBodyOp::PBO_None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Physics")
	bool bLimbApplyImpulseOnDamage = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Physics", meta = (EditCondition = "bLimbApplyImpulseOnDamage", EditConditionHides))
	float LimbImpulseMultiplier = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Physics")
	float LimbLinearDamping = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Physics")
	float LimbAngularDamping = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Blood")
	bool bLimbReceiveDecals = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Blood")
	float BloodSpillDepth = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dismember|Blood", meta = (ToolTip = "Rotation offset for blood spill effect."))
	FRotator BloodSpillRotationOffset = FRotator(-90.f, 0.f, 0.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay, Category = "Dismember"
		,meta = (ToolTip = "If true, the dismemberer will reset its mesh on reconstruction of limbs. This is useful for certain skeletal meshes that require a hard reset to properly update their state after dismemberment and reconstruction."))
	bool bHardReset = false;

private:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY()
	TMap<FName, UNiagaraComponent*> SpawnedBloodSpills;
	UPROPERTY()
	TMap<FName, ADismemberedLimb*> SpawnedDismemberedLimbs;	// Map of dismembered limbs with their bone root names as keys
	UPROPERTY()
	UBloodParticlesNiagaraCallbackHandler* BloodParticlesNiagaraCallbackHandler;
	TArray<FName> DismemberedBonesNames;
	TArray<FName> DismemberableLimbsNames;

public:
	UDismemberer();

	/**
	 * Initializes the dismemberer component with the skeletal mesh component.
	 * @param InSkeletalMeshComponent 
	 */
	UFUNCTION(BlueprintCallable)
	void Init(USkeletalMeshComponent* InSkeletalMeshComponent);

	/**
	 * Reconstructs a dismembered limb by its root bone name.
	 * @param LimbRootBoneName Root bone name of the limb to reconstruct.
	 * @param bStopBloodSpill If true, stops the blood spill effect for the limb.
	 * @param bDestroySpawnedLimb If true, destroys the spawned dismembered limb actor.
	 */
	UFUNCTION(BlueprintCallable)
	void ReconstructLimb(const FName LimbRootBoneName, const bool bStopBloodSpill = true, const bool bDestroySpawnedLimb = true);

	/**
	 * Reconstructs multiple dismembered limbs by their root bone names.
	 * @param BoneNames Array of root bone names of the limbs to reconstruct.
	 * @param bStopBloodSpill If true, stops the blood spill effect for the limbs.
	 * @param bDestroySpawnedLimb If true, destroys the spawned dismembered limb actors.
	 */
	UFUNCTION(BlueprintCallable)
	void ReconstructLimbs(TArray<FName> BoneNames, const bool bStopBloodSpill = true, const bool bDestroySpawnedLimb = true);

	/**
	 * Reconstructs all dismembered limbs.
	 * @param bStopBloodSpill If true, stops the blood spill effect for all limbs.
	 * @param bDestroySpawnedLimb If true, destroys all spawned dismembered limb actors.
	 */
	UFUNCTION(BlueprintCallable)
	void ReconstructAllLimbs(const bool bStopBloodSpill = true, const bool bDestroySpawnedLimb = true);

	/**
	 * Dismembers a limb by its bone name.
	 * @param BoneName Name of the bone to dismember.
	 * @param Impulse Direction of the impulse to apply to the dismembered limb.
	 * @param Damage Amount of damage to apply to the limb.
	 */
	UFUNCTION(BlueprintCallable)
	void DismemberLimb(const FName BoneName, const FVector Impulse, const float Damage);

	/**
	 * Dismembers multiple limbs by their bone names.
	 * @param BoneNames Array of bone names to dismember.
	 * @param Impulse Direction of the impulse to apply to the dismembered limbs.
	 * @param Damage Amount of damage to apply to the limbs.
	 */
	UFUNCTION(BlueprintCallable)
	void DismemberLimbs(const TArray<FName> BoneNames, const FVector Impulse, const float Damage);

	/**
	 * Dismembers a random number of limbs from the dismemberable bones.
	 * @param LimbsCount Number of limbs to dismember randomly.
	 * @param Impulse Direction of the impulse to apply to the dismembered limbs.
	 * @param Damage Amount of damage to apply to the limbs.
	 */
	UFUNCTION(BlueprintCallable)
	void DismemberRandomLimbs(int32 LimbsCount, const FVector Impulse, const float Damage);

	/**
	 * Dismembers all dismemberable limbs.
	 * @param Impulse Direction of the impulse to apply to the dismembered limbs.
	 * @param Damage Amount of damage to apply to the limbs.
	 */
	UFUNCTION(BlueprintCallable)
	void DismemberAllLimbs(const FVector Impulse, const float Damage);

	/**
	 * Stops the blood spill effect for a specific bone.
	 * @param SpillBoneName Name of the bone for which to stop the blood spill effect.
	 */
	UFUNCTION(BlueprintCallable)
	void StopBloodSpill(const FName SpillBoneName);

	/**
	 * Stops the blood spill effects for multiple bones.
	 * @param SpillBoneNames Array of bone names for which to stop the blood spill effects.
	 */
	UFUNCTION(BlueprintCallable)
	void StopBloodSpills(const TArray<FName> SpillBoneNames);

	/**
	 * Stops all blood spill effects.
	 */
	UFUNCTION(BlueprintCallable)
	void StopAllBloodSpills();

	/**
	 * Checks if a limb is dismembered by its bone name.
	 * @param BoneName Name of the bone to check.
	 * @param bCheckOnlyRootBone If true, checks only the root bone of the limb.
	 * @return True if the limb is dismembered, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsLimbDismembered(const FName BoneName, const bool bCheckOnlyRootBone = false) const;

	/**
	 * Checks if a limb is dismemberable by its bone name.
	 * @param BoneName Name of the bone to check.
	 * @return True if the limb is dismemberable, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsLimbDismemberable(const FName BoneName) const;

	/**
	 * Gets the names of all dismembered bones.
	 * @return Array of names of dismembered bones.
	 */
	UFUNCTION(BlueprintPure)
	TArray<FName> GetDismemberedBonesNames() const;

	/**
	 * Gets the names of all dismemberable bones.
	 * @return Array of names of dismemberable bones.
	 */
	UFUNCTION(BlueprintPure)
	TArray<FName> GetDismemberedRootBonesNames() const;

	/**
	 * Gets the names of all dismemberable limbs.
	 * @return Array of names of dismemberable limbs.
	 */
	UFUNCTION(BlueprintPure)
	TArray<ADismemberedLimb*> GetDismemberedLimbs() const;
	
	/**
	 * Gets the skeletal mesh component associated with this dismemberer.
	 * @return Pointer to the skeletal mesh component.
	 */
	USkeletalMeshComponent* GetSkeletalMeshComponent() const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

private:
	/**
	 * Resets the skeletal mesh component to its original state.
	 */
	void ResetMesh() const;

	/**
	 * Resets a limb by its root bone name.
	 * @param LimbRootBoneName Root bone name of the limb to reset.
	 * @param bStopBloodSpill If true, stops the blood spill effect for the limb.
	 * @param bDestroySpawnedLimb If true, destroys the spawned dismembered limb actor.
	 */
	void ResetLimb(FName LimbRootBoneName, bool bStopBloodSpill, bool bDestroySpawnedLimb);
	
	/**
	 * Sorts the bone names in a specific order.
	 * @param Bones Array of bone names to sort.
	 * @return Sorted array of bone names.
	 */
	TArray<FName> SortBonesNames(const TArray<FName>& Bones) const;

	/**
	 * Gets the child bone names of a specified bone.
	 * @param BoneName Name of the bone to get child names for.
	 * @return Array of child bone names.
	 */
	TArray<FName> GetChildBoneNames(const FName& BoneName) const;

	/**
	 * Initializes the dismemberable bones and limbs from the skeletal mesh component.
	 */
	void InitDismemberableBones();

	/**
	 * Hides a limb by its bone name.
	 * @param BoneName Name of the bone to hide.
	 */
	void HideLimb(const FName& BoneName) const;

	/**
	 * Detaches a limb from the skeletal mesh component.
	 * @param LimbData Data of the limb to detach.
	 * @param Impulse Direction of the impulse to apply to the detached limb.
	 */
	void DetachLimb(const FDismemberableLimbData& LimbData, const FVector& Impulse);

	/**
	 * Explodes a limb by its data.
	 * @param LimbData Data of the limb to explode.
	 */
	void ExplodeLimb(const FDismemberableLimbData& LimbData);

	/**
	 * Finds a dismemberable limb by its bone name.
	 * @param BoneName Name of the bone to find.
	 * @param OutLimbData Output parameter to store the found limb data.
	 * @return True if the limb was found, false otherwise.
	 */
	bool FindLimb(const FName& BoneName, FDismemberableLimbData& OutLimbData) const;

	/**
	 * Spawns a limb explosion effect.
	 * @param LimbData Data of the limb for which to spawn the explosion effect.
	 */
	void SpawnLimbExplosionFX(const FDismemberableLimbData& LimbData) const;

	/**
	 * Spawns a blood explosion effect at a specified location.
	 * @param HitLocation Location where the blood explosion effect should be spawned.
	 */
	void SpawnBloodExplosionFX(const FVector& HitLocation) const;

	/**
	 * Spawns a blood spill effect for a specified limb.
	 * @param LimbData Data of the limb for which to spawn the blood spill effect.
	 */
	void SpawnBloodSpillFX(const FDismemberableLimbData& LimbData);

	/**
	 * Gets a random blood spill effect from the available effects.
	 * @return A pointer to a random blood spill Niagara system.
	 */
	UNiagaraSystem* GetRandomBloodSpillFX() const;

	/**
	 * Gets a random blood explosion effect from the available effects.
	 * @return A pointer to a random blood explosion Niagara system.
	 */
	UNiagaraSystem* GetRandomBloodExplosionFX() const;

	/**
	 * Gets a random blood stain decal from the available decals.
	 * @return A pointer to a random blood stain decal material.
	 */
	UMaterialInterface* GetRandomBloodStainDecal() const;

	/**
	 * Spawns a dismembered limb actor.
	 * @param LimbData Data of the limb to spawn.
	 * @param Impulse Direction of the impulse to apply to the spawned limb.
	 */
	void SpawnDismemberedLimb(const FDismemberableLimbData& LimbData, const FVector& Impulse);

	/**
	 * Checks if the dismemberer component is properly initialized.
	 * @return True if the dismemberer is initialized correctly, false otherwise.
	 */
	bool Check() const;
};
