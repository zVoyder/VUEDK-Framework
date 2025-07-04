// Copyright VUEDK, Inc. All Rights Reserved.

#include "Dismemberer.h"
#include "NiagaraFunctionLibrary.h"
#include "DismemberedLimb.h"
#include "NiagaraComponent.h"
#include "Utility/DismembererFactory.h"

UDismemberer::UDismemberer(): DismemberableAvatarData(nullptr),
                              DismembererBloodsData(nullptr),
                              SkeletalMeshComponent(nullptr),
                              BloodParticlesNiagaraCallbackHandler(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDismemberer::Init(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	SkeletalMeshComponent = InSkeletalMeshComponent;
}

void UDismemberer::ReconstructLimb(const FName LimbRootBoneName, const bool bStopBloodSpill, const bool bDestroySpawnedLimb)
{
	if (!Check())
		return;
	
	if (!IsLimbDismembered(LimbRootBoneName, true)) // true because to check only root bone
		return;
	
	SkeletalMeshComponent->UnHideBoneByName(LimbRootBoneName);
	SkeletalMeshComponent->ResetAllBodiesSimulatePhysics();
	SkeletalMeshComponent->RefreshBoneTransforms();
	SkeletalMeshComponent->UpdateComponentToWorld();
	SkeletalMeshComponent->RecreatePhysicsState();
	
	if (bDestroySpawnedLimb)
	{
		ADismemberedLimb* SpawnedLimb = SpawnedDismemberedLimbs.FindRef(LimbRootBoneName);
		if (IsValid(SpawnedLimb)) // If the limb is not destroyed yet, since it has a life span
			SpawnedLimb->Destroy();
	}

	SpawnedDismemberedLimbs.Remove(LimbRootBoneName);
	DismemberedBonesNames.Remove(LimbRootBoneName);
	for (const FName& ChildBoneName : GetChildBoneNames(LimbRootBoneName))
		DismemberedBonesNames.Remove(ChildBoneName);

	if (bStopBloodSpill)
		StopBloodSpill(LimbRootBoneName);
}

void UDismemberer::ReconstructLimbs(TArray<FName> BoneNames, const bool bStopBloodSpill, const bool bDestroySpawnedLimb)
{
	if (!Check())
		return;

	BoneNames = SortBonesNames(BoneNames);
	for (const FName BoneName : BoneNames)
		ReconstructLimb(BoneName, bStopBloodSpill, bDestroySpawnedLimb);
}

void UDismemberer::ReconstructAllLimbs(const bool bStopBloodSpill, const bool bDestroySpawnedLimb)
{
	if (!Check())
		return;
	
	for (const auto BoneName : GetDismemberedRootBonesNames())
		ReconstructLimb(BoneName, bStopBloodSpill, bDestroySpawnedLimb);
}

void UDismemberer::DismemberLimb(const FName BoneName, const FVector Impulse, const float Damage)
{
	if (!Check())
		return;

	if (!IsLimbDismemberable(BoneName) || IsLimbDismembered(BoneName))
		return;

	if (FDismemberableLimbData LimbData; FindLimb(BoneName, LimbData) && LimbData.IsGoingToExplode(Damage))
		ExplodeLimb(LimbData);
	else
		DetachLimb(LimbData, Impulse);
}

void UDismemberer::DismemberLimbs(const TArray<FName> BoneNames, const FVector Impulse, const float Damage)
{
	if (!Check())
		return;

	for (const FName BoneName : BoneNames)
		DismemberLimb(BoneName, Impulse, Damage);
}

void UDismemberer::DismemberRandomLimbs(int32 LimbsCount, const FVector Impulse, const float Damage)
{
	if (!Check())
		return;

	TArray<FName> AvailableBones = DismemberablaLimbsNames;
	AvailableBones.RemoveAll([this](const FName BoneName)
	{
		return DismemberedBonesNames.Contains(BoneName);
	});

	LimbsCount = FMath::Clamp(LimbsCount, 0, AvailableBones.Num());
	for (int32 i = 0; i < LimbsCount; i++)
	{
		const int32 RandomIndex = FMath::RandRange(0, AvailableBones.Num() - 1);
		const FName RandomBoneName = AvailableBones.IsValidIndex(RandomIndex) ? AvailableBones[RandomIndex] : NAME_None;
		DismemberLimb(RandomBoneName, Impulse, Damage);
		AvailableBones.Remove(RandomBoneName);
	}
}

void UDismemberer::DismemberAllLimbs(const FVector Impulse, const float Damage)
{
	if (!Check())
		return;

	TArray<FName> SortedBones = DismemberablaLimbsNames;
	SortedBones.Sort([this](const FName& A, const FName& B)
	{
		return SkeletalMeshComponent->BoneIsChildOf(A, B);
	});

	for (const FName BoneName : SortedBones)
		DismemberLimb(BoneName, Impulse, Damage);
}

void UDismemberer::StopBloodSpill(const FName SpillBoneName)
{
	if (!SpawnedBloodSpills.Contains(SpillBoneName))
		return;

	UNiagaraComponent* BloodSpill = SpawnedBloodSpills.FindRef(SpillBoneName);
	if (IsValid(BloodSpill) && BloodSpill->IsActive())
		BloodSpill->Deactivate();

	SpawnedBloodSpills.Remove(SpillBoneName);
}

void UDismemberer::StopBloodSpills(const TArray<FName> SpillBoneNames)
{
	for (const FName BoneName : SpillBoneNames)
		StopBloodSpill(BoneName);
}

void UDismemberer::StopAllBloodSpills()
{
	for (const auto BloodSpill : SpawnedBloodSpills)
		StopBloodSpill(BloodSpill.Key);
}

bool UDismemberer::IsLimbDismembered(const FName BoneName, const bool bCheckOnlyRootBone) const
{
	if (bCheckOnlyRootBone)
		return SpawnedDismemberedLimbs.Contains(BoneName);

	return DismemberedBonesNames.Contains(BoneName);
}

bool UDismemberer::IsLimbDismemberable(const FName BoneName) const
{
	return DismemberablaLimbsNames.Contains(BoneName);
}

TArray<FName> UDismemberer::GetDismemberedBonesNames() const
{
	return DismemberedBonesNames;
}

TArray<FName> UDismemberer::GetDismemberedRootBonesNames() const
{
	TArray<FName> DismemberedRootBones;
	SpawnedDismemberedLimbs.GetKeys(DismemberedRootBones);
	return DismemberedRootBones;
}

TArray<ADismemberedLimb*> UDismemberer::GetDismemberedLimbs() const
{
	TArray<ADismemberedLimb*> DismemberedLimbs;
	SpawnedDismemberedLimbs.GenerateValueArray(DismemberedLimbs);
	return DismemberedLimbs;
}

FVector UDismemberer::GetBloodSpillDirectionSourceLocation() const
{
	return SkeletalMeshComponent->GetSocketLocation(BloodSpillDirectionSource);
}

USkeletalMeshComponent* UDismemberer::GetSkeletalMeshComponent() const
{
	return SkeletalMeshComponent;
}

void UDismemberer::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnTakePointDamage.AddDynamic(this, &UDismemberer::OnTakePointDamage);
	InitDismemberableBones();
	BloodParticlesNiagaraCallbackHandler = UDismembererFactory::CreateBloodParticlesNiagaraCallbackHandler(GetWorld(), this);
}

void UDismemberer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetOwner()->OnTakePointDamage.RemoveDynamic(this, &UDismemberer::OnTakePointDamage);
}

void UDismemberer::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	if (!Check())
		return;

	SpawnBloodExplosionFX(HitLocation);
}

TArray<FName> UDismemberer::SortBonesNames(const TArray<FName>& Bones) const
{
	TArray<FName> SortBones = Bones;

	SortBones.Sort([&](const FName& A, const FName& B)
	{
		return SkeletalMeshComponent->BoneIsChildOf(A, B);
	});
	
	return SortBones;
}

TArray<FName> UDismemberer::GetChildBoneNames(const FName& BoneName) const
{
	TArray<FName> ChildBones;

	for (const FName& ChildBoneName : SkeletalMeshComponent->GetAllSocketNames())
	{
		if (SkeletalMeshComponent->BoneIsChildOf(ChildBoneName, BoneName))
			ChildBones.Add(ChildBoneName);
	}
	
	return ChildBones;
}

void UDismemberer::InitDismemberableBones()
{
	if (!IsValid(DismemberableAvatarData))
	{
		UE_LOG(LogTemp, Error, TEXT("Dismemberer::InitDismemberableBones: DismemberableAvatarData is not valid in Actor: %s"), *GetOwner()->GetName());
		return;
	}

	for (FDismemberableLimbData& Limb : DismemberableAvatarData->DismemberableLimbs)
		DismemberablaLimbsNames.Add(Limb.BoneName);
}

void UDismemberer::HideLimb(const FName& BoneName) const
{
	SkeletalMeshComponent->HideBoneByName(BoneName, EPhysBodyOp::PBO_Term);
}

void UDismemberer::DetachLimb(const FDismemberableLimbData& LimbData, const FVector& Impulse)
{
	SpawnDisemberedLimb(LimbData, Impulse);
	SpawnBloodSpillFX(LimbData);
	SpawnBloodExplosionFX(SkeletalMeshComponent->GetSocketLocation(LimbData.BoneName));
	HideLimb(LimbData.BoneName);
}

void UDismemberer::ExplodeLimb(const FDismemberableLimbData& LimbData)
{
	SpawnBloodSpillFX(LimbData);
	SpawnLimbExplosionFX(LimbData);
	HideLimb(LimbData.BoneName);
}

bool UDismemberer::FindLimb(const FName& BoneName, FDismemberableLimbData& OutLimbData) const
{
	for (const FDismemberableLimbData& Limb : DismemberableAvatarData->DismemberableLimbs)
	{
		if (Limb.BoneName == BoneName)
		{
			OutLimbData = Limb;
			return true;
		}
	}

	return false;
}

void UDismemberer::SpawnLimbExplosionFX(const FDismemberableLimbData& LimbData) const
{
	const FVector& Location = SkeletalMeshComponent->GetSocketLocation(LimbData.BoneName);
	if (!IsValid(DismembererBloodsData))
		return;

	UDismembererFactory::SpawnDismemberExplosionFX(GetWorld(), BloodParticlesNiagaraCallbackHandler, LimbData.LimbExplosionFX, Location);
}

void UDismemberer::SpawnBloodExplosionFX(const FVector& HitLocation) const
{
	if (!IsValid(DismembererBloodsData))
		return;

	UDismembererFactory::SpawnDismemberExplosionFX(GetWorld(), BloodParticlesNiagaraCallbackHandler, GetRandomBloodExplosionFX(), HitLocation);
}

void UDismemberer::SpawnBloodSpillFX(const FDismemberableLimbData& LimbData)
{
	if (!IsValid(DismembererBloodsData) || DismembererBloodsData->BloodSpillEffects.IsEmpty())
		return;

	const FVector From = SkeletalMeshComponent->GetBoneLocation(LimbData.BoneName);
	const FVector To = GetBloodSpillDirectionSourceLocation();
	UNiagaraComponent* Spill = UDismembererFactory::SpawnBloodSpillFX(GetWorld(), BloodParticlesNiagaraCallbackHandler, GetRandomBloodSpillFX(), SkeletalMeshComponent, LimbData, From, To, BloodSpillFXRotationOffset, BloodSpillFXDepth);
	SpawnedBloodSpills.Add(LimbData.BoneName, Spill);
}

UNiagaraSystem* UDismemberer::GetRandomBloodSpillFX() const
{
	if (!IsValid(DismembererBloodsData))
		return nullptr;

	const int32 Index = FMath::RandRange(0, DismembererBloodsData->BloodSpillEffects.Num() - 1);

	if (!DismembererBloodsData->BloodSpillEffects.IsValidIndex(Index))
		return nullptr;

	return DismembererBloodsData->BloodSpillEffects[Index];
}

UNiagaraSystem* UDismemberer::GetRandomBloodExplosionFX() const
{
	if (!IsValid(DismembererBloodsData))
		return nullptr;

	const int32 Index = FMath::RandRange(0, DismembererBloodsData->HitEffects.Num() - 1);

	if (!DismembererBloodsData->HitEffects.IsValidIndex(Index))
		return nullptr;

	return DismembererBloodsData->HitEffects[Index];
}

UMaterialInterface* UDismemberer::GetRandomBloodStainDecal() const
{
	if (!IsValid(DismembererBloodsData))
		return nullptr;

	const int32 Index = FMath::RandRange(0, DismembererBloodsData->BloodStainsDecals.Num() - 1);

	if (!DismembererBloodsData->BloodStainsDecals.IsValidIndex(Index))
		return nullptr;

	return DismembererBloodsData->BloodStainsDecals[Index];
}

void UDismemberer::SpawnDisemberedLimb(const FDismemberableLimbData& LimbData, const FVector& Impulse)
{
	ADismemberedLimb* Limb = UDismembererFactory::SpawnDismemberedLimb(this, LimbData, Impulse, UDismembererFactory::GetDismembererSettings()->DismemberedLimbLifeSpan, DismemberableAvatarData->LimbsScaleFactor);
	if (!IsValid(Limb))
		return;

	DismemberedBonesNames.Append(Limb->GetLimbBoneNames());
	DismemberedBonesNames = SortBonesNames(DismemberedBonesNames);
	SpawnedDismemberedLimbs.Add(LimbData.BoneName, Limb);
}

bool UDismemberer::Check() const
{
	return IsValid(SkeletalMeshComponent) && IsValid(GetWorld()) && DismemberablaLimbsNames.Num() > 0 && IsValid(DismemberableAvatarData);
}
