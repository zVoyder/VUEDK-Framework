// Copyright VUEDK, Inc. All Rights Reserved.

#include "DismemberedLimb.h"
#include "DismembererSystem.h"
#include "Effects/BloodParticlesNiagaraCallbackHandler.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Factories/DismembererFactory.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "PhysicsEngine/SkeletalBodySetup.h"
#include "PhysicsEngine/SphereElem.h"

class USkeletalBodySetup;

ADismemberedLimb::ADismemberedLimb()
{
	PrimaryActorTick.bCanEverTick = true;
	PhysicRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PhysicRootComponent"));
	PhysicRootComponent->SetSphereRadius(0.f);
	PhysicRootComponent->bApplyImpulseOnDamage = false;
	RootComponent = PhysicRootComponent;
	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	PoseableMesh->SetupAttachment(PhysicRootComponent);
}

void ADismemberedLimb::Init(const FDismemberableLimbData& InLimbData, UDismemberer* InDismemberer, const FVector& Impulse, float LifeSpan)
{
	Dismemberer = InDismemberer;

	if (!IsValid(Dismemberer))
	{
		UE_LOG(LogDismembermentSystem, Warning, TEXT("DismemberedLimb::Init: Invalid Dismemberer."));
		ReceiveDispose();
		return;
	}

	LimbData = InLimbData;
	TargetBoneName = LimbData.BoneName;
	TargetSkeletalMeshComponent = Dismemberer->GetSkeletalMeshComponent();

	if (!Check())
	{
		UE_LOG(LogDismembermentSystem, Warning, TEXT("DismemberedLimb::Init: Invalid Check."));
		ReceiveDispose();
		return;
	}

	PoseableMesh->SetReceivesDecals(Dismemberer->bLimbReceiveDecals);
	bApplyImpulseOnDamage = Dismemberer->bLimbApplyImpulseOnDamage;
	CopyMesh();
	PhysicRootComponent->SetAllMassScale(LimbData.LimbMass);
	IsolateLimb();
	SpawnBloodSpillFX();
	CloneLimbCollision();
	ApplyLimbImpulse(Impulse, TargetBoneName);
	LifeSpanDuration = LifeSpan;

	if (bAutoStartLifeSpan)
		StartLifeSpanTimer();

	OnInit(LimbData, Dismemberer, Impulse, LifeSpan);
}

void ADismemberedLimb::SetLifeSpanDuration(const float Duration)
{
	LifeSpanDuration = Duration;
}

float ADismemberedLimb::GetLifeSpanDuration() const
{
	return LifeSpanDuration;
}

TArray<FName> ADismemberedLimb::GetLimbBoneNames() const
{
	return LimbBoneNames;
}

UDismemberer* ADismemberedLimb::GetDismemberer() const
{
	return Dismemberer;
}

FDismemberableLimbData ADismemberedLimb::GetLimbData() const
{
	return LimbData;
}

void ADismemberedLimb::Dispose()
{
	if (!IsValid(Dismemberer))
		return;

	ResetLimbState();
	ReceiveDispose();
}

void ADismemberedLimb::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &ADismemberedLimb::OnAnyDamageNative);
	OnTakePointDamage.AddDynamic(this, &ADismemberedLimb::OnPointDamageNative);
	OnTakeRadialDamage.AddDynamic(this, &ADismemberedLimb::OnRadialDamageNative);
}

void ADismemberedLimb::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OnTakeAnyDamage.RemoveDynamic(this, &ADismemberedLimb::OnAnyDamageNative);
	OnTakePointDamage.RemoveDynamic(this, &ADismemberedLimb::OnPointDamageNative);
	OnTakeRadialDamage.RemoveDynamic(this, &ADismemberedLimb::OnRadialDamageNative);
}

void ADismemberedLimb::OnAnyDamageNative(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (LimbData.IsGoingToExplode(Damage))
		ExplodeLimb();

	OnAnyDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
}

void ADismemberedLimb::OnPointDamageNative(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	SpawnBloodExplosionFX(HitLocation);
	ApplyLimbImpulse(-ShotFromDirection * Damage, BoneName);

	OnPointDamage(DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);
}

void ADismemberedLimb::OnRadialDamageNative(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	SpawnBloodExplosionFX(HitInfo.ImpactPoint);
	ApplyLimbImpulse((Origin - GetActorLocation()).GetSafeNormal() * Damage, NAME_None);

	OnRadialDamage(DamagedActor, Damage, DamageType, Origin, HitInfo, InstigatedBy, DamageCauser);
}

void ADismemberedLimb::OnInit_Implementation(const FDismemberableLimbData& InLimbData, UDismemberer* InDismemberer, const FVector& Impulse, float LifeSpan)
{
}

void ADismemberedLimb::OnPointDamage_Implementation(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
}

void ADismemberedLimb::OnAnyDamage_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
}

void ADismemberedLimb::OnRadialDamage_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
}

void ADismemberedLimb::ReceiveDispose_Implementation()
{
	Destroy();
}

void ADismemberedLimb::StartLifeSpanTimer()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogDismembermentSystem, Warning, TEXT("DismemberedLimb::StartLifeSpanTimer: Invalid world. Disposing limb immediately."));
		Dispose();
		return;
	}

	if (LifeSpanDuration <= 0.f)
	{
		Dispose();
		return;
	}

	World->GetTimerManager().SetTimer(LifeSpanTimerHandle, this, &ADismemberedLimb::Dispose, LifeSpanDuration, false);
}

void ADismemberedLimb::StopLifeSpanTimer()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogDismembermentSystem, Warning, TEXT("DismemberedLimb::StopLifeSpanTimer: Invalid world. Cannot stop lifespan timer. Disposing limb immediately."));
		Dispose();
		return;
	}

	World->GetTimerManager().ClearTimer(LifeSpanTimerHandle);
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

void ADismemberedLimb::CopyMesh() const
{
	const TArray<UMaterialInterface*> Materials = TargetSkeletalMeshComponent->GetMaterials();
	UMaterialInterface* OverlayMaterial = TargetSkeletalMeshComponent->GetOverlayMaterial();
	PoseableMesh->SetSkinnedAssetAndUpdate(TargetSkeletalMeshComponent->GetSkeletalMeshAsset());
	ApplyMaterials(Materials, OverlayMaterial);
}

void ADismemberedLimb::ApplyMaterials(TArray<UMaterialInterface*> Materials, UMaterialInterface* OverlayMaterial) const
{
	for (int32 i = 0; i < PoseableMesh->GetNumMaterials(); i++)
	{
		if (UMaterialInterface* Material = Materials.IsValidIndex(i) ? Materials[i] : nullptr)
			PoseableMesh->SetMaterial(i, Material);
	}

	PoseableMesh->SetOverlayMaterial(OverlayMaterial);
}

void ADismemberedLimb::ExplodeLimb()
{
	SpawnLimbExplosionFX();
	ReceiveDispose();
}

void ADismemberedLimb::AlignLimbWithTargetBone()
{
	const FVector WorldLoc = TargetSkeletalMeshComponent->GetBoneLocation(TargetBoneName, EBoneSpaces::WorldSpace);
	const FRotator WorldRot = TargetSkeletalMeshComponent->GetComponentRotation();
	SetActorLocationAndRotation(WorldLoc, WorldRot);
}

void ADismemberedLimb::CopyPoseFromSkeletalMesh()
{
	LimbBoneNames.Empty();
	LimbBoneNames.Add(TargetBoneName);

	const FVector BoneLoc = PoseableMesh->GetBoneLocationByName(TargetBoneName, EBoneSpaces::ComponentSpace);
	const int32 BoneCount = FMath::Min(PoseableMesh->GetNumBones(), TargetSkeletalMeshComponent->GetNumBones());
	for (int32 i = 1; i < BoneCount; i++)
	{
		const FName CurrentBoneName = TargetSkeletalMeshComponent->GetBoneName(i);
		if (CurrentBoneName.IsNone())
			continue;

		const int32 PoseableBoneIndex = PoseableMesh->GetBoneIndex(CurrentBoneName);
		if (PoseableBoneIndex == INDEX_NONE)
			continue;

		const FRotator CurrentBoneRot = TargetSkeletalMeshComponent->GetBoneQuaternion(CurrentBoneName, EBoneSpaces::ComponentSpace).Rotator();
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

	UPhysicsAsset* PhysicsAsset = TargetSkeletalMeshComponent->GetPhysicsAsset();

	if (!IsValid(PhysicsAsset))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("DismemberedLimb::CloneLimbCollision: PhysicsAsset is nullptr."));
		return;
	}

	for (const USkeletalBodySetup* BodySetup : PhysicsAsset->SkeletalBodySetups)
	{
		if (!GetLimbBoneNames().Contains(BodySetup->BoneName) || Dismemberer->GetDismemberedBonesNames().Contains(BodySetup->BoneName))
			continue;

		const FTransform BoneTransform = TargetSkeletalMeshComponent->GetSocketTransform(BodySetup->BoneName, RTS_World);
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

void ADismemberedLimb::RegisterCollision(UShapeComponent* Collision)
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

	const FName ParentBone = PoseableMesh->GetParentBone(LimbData.BoneName);
	const FVector From = PoseableMesh->GetBoneLocation(ParentBone);
	const FVector To = PoseableMesh->GetSocketLocation(LimbData.BoneName);
	UDismembererFactory::SpawnBloodSpillFX(GetWorld(), Dismemberer->BloodParticlesNiagaraCallbackHandler, Dismemberer->GetRandomBloodSpillFX(), PoseableMesh, LimbData, From, To, Dismemberer->BloodSpillRotationOffset, Dismemberer->BloodSpillDepth);
}

void ADismemberedLimb::SpawnLimbExplosionFX() const
{
	if (!Check() || !IsValid(Dismemberer->DismembererBloodsData))
		return;

	UDismembererFactory::SpawnDismemberExplosionFX(GetWorld(), Dismemberer->BloodParticlesNiagaraCallbackHandler, LimbData.LimbExplosionFX, GetActorLocation());
}

void ADismemberedLimb::ResetLimbState()
{
	Dismemberer = nullptr;
	TargetSkeletalMeshComponent = nullptr;
	TargetBoneName = NAME_None;
	LimbData = FDismemberableLimbData();
	LimbBoneNames.Empty();
	bApplyImpulseOnDamage = false;

	if (PoseableMesh)
	{
		PoseableMesh->ResetRelativeTransform();
		PoseableMesh->SetSkinnedAssetAndUpdate(nullptr);

		for (int32 BoneIndex = 0; BoneIndex < PoseableMesh->GetNumBones(); ++BoneIndex)
		{
			const FName BoneName = PoseableMesh->GetBoneName(BoneIndex);
			PoseableMesh->SetBoneLocationByName(BoneName, FVector::ZeroVector, EBoneSpaces::ComponentSpace);
			PoseableMesh->SetBoneRotationByName(BoneName, FRotator::ZeroRotator, EBoneSpaces::ComponentSpace);
			PoseableMesh->SetBoneScaleByName(BoneName, FVector::One(), EBoneSpaces::ComponentSpace);
			PoseableMesh->UnHideBoneByName(BoneName);
		}

		PoseableMesh->SetOverlayMaterial(nullptr);
		PoseableMesh->EmptyOverrideMaterials();
	}

	TArray<UActorComponent*> CollisionChildren;
	GetComponents(UShapeComponent::StaticClass(), CollisionChildren);

	for (UActorComponent* Component : CollisionChildren)
	{
		// PhysicRootComponent must NOT be destroyed
		if (Component != PhysicRootComponent)
			Component->DestroyComponent();
	}

	if (PhysicRootComponent)
	{
		PhysicRootComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
		PhysicRootComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		PhysicRootComponent->SetSimulatePhysics(false);
		PhysicRootComponent->SetCollisionProfileName(TEXT("NoCollision"));
		PhysicRootComponent->SetLinearDamping(0.f);
		PhysicRootComponent->SetAngularDamping(0.f);
		PhysicRootComponent->SetAllMassScale(1.f);
		PhysicRootComponent->SetRelativeLocation(FVector::ZeroVector);
		PhysicRootComponent->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

bool ADismemberedLimb::Check() const
{
	return
		IsValid(GetWorld()) &&
		IsValid(Dismemberer) &&
		IsValid(PoseableMesh) &&
		IsValid(TargetSkeletalMeshComponent) &&
		IsValid(TargetSkeletalMeshComponent->GetSkinnedAsset());
}
