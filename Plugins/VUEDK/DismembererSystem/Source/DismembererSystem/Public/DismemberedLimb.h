// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraDataInterfaceExport.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Data/DismemberableLimbData.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/SkeletalBodySetup.h"
#include "Dismemberer.h"
#include "DismemberedLimb.generated.h"

constexpr float HideBoneScale = 0.00001f;

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class DISMEMBERERSYSTEM_API ADismemberedLimb : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* PhysicRootComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPoseableMeshComponent* PoseableMesh;

private:
	UPROPERTY()
	UDismemberer* Dismemberer;
	UPROPERTY()
	USkeletalMeshComponent* TargetSkelatalMeshComponent;
	FName TargetBoneName;
	FDismemberableLimbData LimbData;
	TArray<FName> LimbBoneNames;
	bool bApplyImpulseOnDamage;

public:
	ADismemberedLimb();

	/**
	 * Initializes the dismembered limb with the provided data.
	 * @param InLimbData The data for the dismembered limb, including the skeletal mesh and bone names.
	 * @param InDismemberer The dismemberer that manages this limb.
	 * @param Impulse The impulse to apply to the limb when it is created.
	 * @param LifeSpan The lifespan of the dismembered limb in seconds. If set to 0, the limb will not be destroyed automatically.
	 */
	void Init(const FDismemberableLimbData& InLimbData, UDismemberer* InDismemberer, const FVector& Impulse, float LifeSpan);

	/**
	 * Returns the target skeletal mesh component that this limb is associated with.
	 * @return The skeletal mesh component of the target character.
	 */
	TArray<FName> GetLimbBoneNames() const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnPointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
	void OnRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

private:
	/**
	 * Applies an impulse to the limb at the specified bone.
	 * @param Impulse The impulse vector to apply to the limb.
	 * @param BoneName The name of the bone to which the impulse will be applied.
	 */
	void ApplyLimbImpulse(const FVector& Impulse, const FName& BoneName) const;

	/**
	 * Isolates the limb from the target skeletal mesh, making it a separate entity.
	 */
	void IsolateLimb();

	/**
	 * Destroys the limb and spawns its explosion effect.
	 */
	void ExplodeLimb();

	/**
	 * Aligns the dismembered limb with the target bone's location and rotation.
	 */
	void AlignLimbWithTargetBone();

	/**
	 * Copies the pose from the target skeletal mesh to the poseable mesh component.
	 */
	void CopyPoseFromSkeletalMesh();

	/**
	 * Hides all bones that are not part of the dismembered limb.
	 */
	void HideNonLimbBones() const;

	/**
	 * Sets the physical properties of the root component for the dismembered limb.
	 */
	void SetPhysicRootBehaviour() const;

	/**
	 * Clones the limb's collision shapes from the target skeletal mesh component.
	 */
	void CloneLimbCollision();

	/**
	 * Creates sphere collisions for the limb based on the body setup and bone transform.
	 * @param BodySetup The body setup containing the collision data.
	 * @param BoneTransform The transform of the bone to which the collision will be applied.
	 */
	void CreateSphereCollisions(const USkeletalBodySetup* BodySetup, const FTransform& BoneTransform);

	/**
	 * Creates capsule collisions for the limb based on the body setup and bone transform.
	 * @param BodySetup The body setup containing the collision data.
	 * @param BoneTransform The transform of the bone to which the collision will be applied.
	 */
	void CreateCapsuleCollisions(const USkeletalBodySetup* BodySetup, const FTransform& BoneTransform);

	/**
	 * Creates box collisions for the limb based on the body setup and bone transform.
	 * @param BodySetup The body setup containing the collision data.
	 * @param BoneTransform The transform of the bone to which the collision will be applied.
	 */
	void CreateBoxCollisions(const USkeletalBodySetup* BodySetup, const FTransform& BoneTransform);

	/**
	 * Registers a collision component for the limb.
	 * @param Collision The collision component to register.
	 */
	void RegisterCollision(UShapeComponent* Collision) const;

	/**
	 * Spawns a blood explosion effect at the specified hit location.
	 * @param HitLocation The location where the blood explosion effect will be spawned.
	 */
	void SpawnBloodExplosionFX(const FVector& HitLocation) const;

	/**
	 * Spawns a blood spill effect for the dismembered limb.
	 */
	void SpawnBloodSpillFX() const;

	/**
	 * Spawns a limb explosion effect.
	 */
	void SpawnLimbExplosionFX() const;

	/**
	* Checks if the dismembered limb is valid and ready for operations.
	* @return True if the limb is valid, false otherwise.
	*/
	bool Check() const;
};
