// Copyright VUEDK, Inc. All Rights Reserved.

#include "DismemberedLimb.h"

#include "BloodParticlesNiagaraCallbackHandler.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceExport.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Utility/DismembererFactory.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "PhysicsEngine/SkeletalBodySetup.h"
#include "PhysicsEngine/SphereElem.h"

class USkeletalBodySetup;

ADismemberedLimb::ADismemberedLimb()
{
	PrimaryActorTick.bCanEverTick = false;
	PhysicRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PhysicRootComponent"));
	PhysicRootComponent->SetSphereRadius(0.f);
	PhysicRootComponent->bApplyImpulseOnDamage = false;
	RootComponent = PhysicRootComponent;
	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	PoseableMesh->SetupAttachment(PhysicRootComponent);
}

void ADismemberedLimb::Init(const FDismemberableLimbData& InLimbData, UDismemberer* InDismemberer, const FVector& Impulse, float LifeSpan)
{
	LimbData = InLimbData;
	TargetBoneName = LimbData.BoneName;
	Dismemberer = InDismemberer;
	PoseableMesh->SetReceivesDecals(Dismemberer->bLimbReceiveDecals);
	SetLifeSpan(LifeSpan);

	if (!Check())
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("DismemberedLimb::Init: Invalid Check."));
		Destroy();
		return;
	}

	TargetSkelatalMeshComponent = Dismemberer->GetSkeletalMeshComponent();
	bApplyImpulseOnDamage = Dismemberer->bLimbApplyImpulseOnDamage;

	PoseableMesh->SetSkinnedAssetAndUpdate(TargetSkelatalMeshComponent->GetSkeletalMeshAsset());
	PhysicRootComponent->SetAllMassScale(LimbData.LimbMass);
	IsolateLimb();
	SpawnBloodSpillFX();
	CloneLimbCollision();
	ApplyLimbImpulse(Impulse, TargetBoneName);

#if WITH_EDITOR
	SetActorLabel(TargetBoneName.ToString());
#endif
}

TArray<FName> ADismemberedLimb::GetLimbBoneNames() const
{
	return LimbBoneNames;
}

void ADismemberedLimb::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &ADismemberedLimb::OnAnyDamage);
	OnTakePointDamage.AddDynamic(this, &ADismemberedLimb::OnPointDamage);
	OnTakeRadialDamage.AddDynamic(this, &ADismemberedLimb::OnRadialDamage);
}

void ADismemberedLimb::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OnTakeAnyDamage.RemoveDynamic(this, &ADismemberedLimb::OnAnyDamage);
	OnTakePointDamage.RemoveDynamic(this, &ADismemberedLimb::OnPointDamage);
	OnTakeRadialDamage.RemoveDynamic(this, &ADismemberedLimb::OnRadialDamage);
}

void ADismemberedLimb::OnAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (LimbData.IsGoingToExplode(Damage))
		ExplodeLimb();
}

void ADismemberedLimb::OnPointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	SpawnBloodExplosionFX(HitLocation);
	ApplyLimbImpulse(-ShotFromDirection * Damage, BoneName);
}

void ADismemberedLimb::OnRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	SpawnBloodExplosionFX(HitInfo.ImpactPoint);
	ApplyLimbImpulse((Origin - GetActorLocation()).GetSafeNormal() * Damage, NAME_None);
}

void ADismemberedLimb::ApplyLimbImpulse(const FVector& Impulse, const FName& BoneName) const
{
	if (!bApplyImpulseOnDamage)
		return;

	PhysicRootComponent->AddImpulse(Impulse * Dismemberer->LimbImpulseMultiplier, BoneName);
}

void ADismemberedLimb::IsolateLimb()
{
	CopyPoseFromSkeletalMesh();
	AlignLimbWithTargetBone();
	HideNonLimbBones();
}

void ADismemberedLimb::ExplodeLimb()
{
	SpawnLimbExplosionFX();
	Destroy();
}

void ADismemberedLimb::AlignLimbWithTargetBone()
{
	const FVector WorldLoc = TargetSkelatalMeshComponent->GetBoneLocation(TargetBoneName, EBoneSpaces::WorldSpace);
	const FRotator WorldRot = TargetSkelatalMeshComponent->GetComponentRotation();
	SetActorLocationAndRotation(WorldLoc, WorldRot);
}

void ADismemberedLimb::CopyPoseFromSkeletalMesh()
{
	LimbBoneNames.Add(TargetBoneName);
	const FVector BoneLoc = PoseableMesh->GetBoneLocationByName(TargetBoneName, EBoneSpaces::ComponentSpace);
	for (int32 i = 1; i < PoseableMesh->GetNumBones(); i++) // Skip the root bone, copy rotation and location of each bone
	{
		const FName CurrentBoneName = TargetSkelatalMeshComponent->GetBoneName(i);
		const FRotator CurrentBoneRot = TargetSkelatalMeshComponent->GetBoneQuaternion(CurrentBoneName, EBoneSpaces::ComponentSpace).Rotator();
		PoseableMesh->SetBoneRotationByName(CurrentBoneName, CurrentBoneRot, EBoneSpaces::ComponentSpace);

		if (PoseableMesh->BoneIsChildOf(CurrentBoneName, TargetBoneName))
			LimbBoneNames.Add(CurrentBoneName);
		else
			PoseableMesh->SetBoneLocationByName(CurrentBoneName, BoneLoc, EBoneSpaces::ComponentSpace);
	}
}

void ADismemberedLimb::HideNonLimbBones() const
{
	PoseableMesh->SetBoneScaleByName(PoseableMesh->GetBoneName(0), FVector::One() * HideBoneScale, EBoneSpaces::ComponentSpace);
	PoseableMesh->SetBoneScaleByName(TargetBoneName, FVector::One(), EBoneSpaces::WorldSpace);

	for (const FName& BoneName : Dismemberer->GetDismemberedBonesNames())
		PoseableMesh->HideBoneByName(BoneName, EPhysBodyOp::PBO_Term);
}

void ADismemberedLimb::SetPhysicRootBehaviour() const
{
	PhysicRootComponent->SetLinearDamping(Dismemberer->LimbLinearDamping);
	PhysicRootComponent->SetAngularDamping(Dismemberer->LimbAngularDamping);
}

void ADismemberedLimb::CloneLimbCollision()
{
	SetPhysicRootBehaviour();

	UPhysicsAsset* PhysicsAsset = TargetSkelatalMeshComponent->GetPhysicsAsset();

	if (!IsValid(PhysicsAsset))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("DismemberedLimb::CreateApproximateCollision: PhysicsAsset is nullptr."));
		return;
	}

	for (const USkeletalBodySetup* BodySetup : PhysicsAsset->SkeletalBodySetups)
	{
		if (!GetLimbBoneNames().Contains(BodySetup->BoneName) || Dismemberer->GetDismemberedBonesNames().Contains(BodySetup->BoneName))
			continue;

		const FTransform BoneTransform = TargetSkelatalMeshComponent->GetSocketTransform(BodySetup->BoneName, RTS_World);
		CreateSphereCollisions(BodySetup, BoneTransform);
		CreateCapsuleCollisions(BodySetup, BoneTransform);
		CreateBoxCollisions(BodySetup, BoneTransform);
	}

	PhysicRootComponent->SetCollisionProfileName(UDismembererFactory::GetDismembererSettings()->LimbCollisionProfile.Name);
	PhysicRootComponent->SetSimulatePhysics(true);
}

void ADismemberedLimb::CreateSphereCollisions(const USkeletalBodySetup* BodySetup, const FTransform& BoneTransform)
{
	for (const FKSphereElem& SphereElem : BodySetup->AggGeom.SphereElems)
	{
		UCapsuleComponent* CapsuleCollision = NewObject<UCapsuleComponent>(this);
		CapsuleCollision->SetupAttachment(PhysicRootComponent);
		CapsuleCollision->SetCapsuleSize(SphereElem.Radius, SphereElem.Radius * 2.0f);
		CapsuleCollision->SetWorldTransform(SphereElem.GetTransform() * BoneTransform);
		RegisterCollision(CapsuleCollision);
	}
}

void ADismemberedLimb::CreateCapsuleCollisions(const USkeletalBodySetup* BodySetup, const FTransform& BoneTransform)
{
	for (const FKSphylElem& SphylElem : BodySetup->AggGeom.SphylElems)
	{
		UCapsuleComponent* CapsuleCollision = NewObject<UCapsuleComponent>(this);
		CapsuleCollision->SetupAttachment(PhysicRootComponent);
		CapsuleCollision->SetCapsuleSize(SphylElem.Radius, SphylElem.Length);
		CapsuleCollision->SetWorldTransform(SphylElem.GetTransform() * BoneTransform);
		RegisterCollision(CapsuleCollision);
	}
}

void ADismemberedLimb::CreateBoxCollisions(const USkeletalBodySetup* BodySetup, const FTransform& BoneTransform)
{
	for (const FKBoxElem& BoxElem : BodySetup->AggGeom.BoxElems)
	{
		UBoxComponent* BoxCollision = NewObject<UBoxComponent>(this);
		BoxCollision->SetupAttachment(PhysicRootComponent);
		BoxCollision->SetBoxExtent(FVector(BoxElem.X, BoxElem.Y, BoxElem.Z) * 0.5f);
		BoxCollision->SetWorldTransform(BoxElem.GetTransform() * BoneTransform);
		RegisterCollision(BoxCollision);
	}
}

void ADismemberedLimb::RegisterCollision(UShapeComponent* Collision) const
{
	Collision->bApplyImpulseOnDamage = false;
	Collision->SetCollisionProfileName(UDismembererFactory::GetDismembererSettings()->LimbCollisionProfile.Name);
	Collision->RegisterComponent();
	Collision->Activate();
}

void ADismemberedLimb::SpawnBloodExplosionFX(const FVector& HitLocation) const
{
	if (!Check() || !IsValid(Dismemberer->DismembererBloodsData))
		return;

	UDismembererFactory::SpawnDismemberExplosionFX(GetWorld(), Dismemberer->BloodParticlesNiagaraCallbackHandler, Dismemberer->GetRandomBloodExplosionFX(), HitLocation);
}

void ADismemberedLimb::SpawnBloodSpillFX() const
{
	if (!Check() || !IsValid(Dismemberer->DismembererBloodsData))
		return;

	const FVector From = Dismemberer->GetBloodSpillDirectionSourceLocation();
	const FVector To = PoseableMesh->GetBoneLocation(LimbData.BoneName);
	UDismembererFactory::SpawnBloodSpillFX(GetWorld(),  Dismemberer->BloodParticlesNiagaraCallbackHandler, Dismemberer->GetRandomBloodSpillFX(), PoseableMesh, LimbData, From, To, Dismemberer->BloodSpillFXRotationOffset, Dismemberer->BloodSpillFXDepth);
}

void ADismemberedLimb::SpawnLimbExplosionFX() const
{
	if (!Check() || !IsValid(Dismemberer->DismembererBloodsData))
		return;
	
	UDismembererFactory::SpawnDismemberExplosionFX(GetWorld(),  Dismemberer->BloodParticlesNiagaraCallbackHandler, LimbData.LimbExplosionFX, GetActorLocation());
}

bool ADismemberedLimb::Check() const
{
	return IsValid(Dismemberer) && IsValid(GetWorld());
}
