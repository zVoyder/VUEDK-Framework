// Copyright VUEDK, Inc. All Rights Reserved.

#include "Data/DismemberableAvatarData.h"
#include "PhysicsEngine/SkeletalBodySetup.h"

#if WITH_EDITOR
void UDismemberableAvatarData::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PhysicsAsset == nullptr)
		return;

	const FName ChangedPropertyName = PropertyChangedEvent.Property
		? PropertyChangedEvent.Property->GetFName()
		: NAME_None;

	if (ChangedPropertyName != GET_MEMBER_NAME_CHECKED(UDismemberableAvatarData, PhysicsAsset))
		return;

	DismemberableLimbs.Empty();
	for (const USkeletalBodySetup* BodySetup : PhysicsAsset->SkeletalBodySetups)
	{
		if (BodySetup)
		{
			const FName BoneName = BodySetup->BoneName;
			DismemberableLimbs.Add(FDismemberableLimbData(BoneName));
		}
	}
}
#endif
