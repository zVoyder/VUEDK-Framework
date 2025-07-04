// Copyright VUEDK, Inc. All Rights Reserved.

#include "Data/HitBoxBonesData.h"
#include "PhysicsEngine/SkeletalBodySetup.h"

#if WITH_EDITOR
void UHitBoxBonesData::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PhysicsAsset == nullptr)
		return;

	const FName ChangedPropertyName = PropertyChangedEvent.Property
		? PropertyChangedEvent.Property->GetFName()
		: NAME_None;

	if (ChangedPropertyName != GET_MEMBER_NAME_CHECKED(UHitBoxBonesData, PhysicsAsset))
		return;

	BonesDamageMultipliers.Empty();
	for (const USkeletalBodySetup* BodySetup : PhysicsAsset->SkeletalBodySetups)
	{
		if (BodySetup)
		{
			const FName BoneName = BodySetup->BoneName;
			BonesDamageMultipliers.Add(BoneName, 1.0f);
		}
	}
}
#endif
