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

	void Init(const FDismemberableLimbData& InLimbData, UDismemberer* InDismemberer, const FVector& Impulse, float LifeSpan);

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
	void ApplyLimbImpulse(const FVector& Impulse, const FName& BoneName) const;

	void IsolateLimb();

	void ExplodeLimb();

	void AlignLimbWithTargetBone();

	void CopyPoseFromSkeletalMesh();

	void HideNonLimbBones() const;

	void SetPhysicRootBehaviour() const;

	void CloneLimbCollision();

	void CreateSphereCollisions(const USkeletalBodySetup* BodySetup, const FTransform& BoneTransform);

	void CreateCapsuleCollisions(const USkeletalBodySetup* BodySetup, const FTransform& BoneTransform);

	void CreateBoxCollisions(const USkeletalBodySetup* BodySetup, const FTransform& BoneTransform);

	void RegisterCollision(UShapeComponent* Collision) const;

	void SpawnBloodExplosionFX(const FVector& HitLocation) const;

	void SpawnBloodSpillFX() const;

	void SpawnLimbExplosionFX() const;
	
	bool Check() const;
};
