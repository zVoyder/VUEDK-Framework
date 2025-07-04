// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/Notifies/Base/WeaponNotifyStateBase.h"
#include "WeaponSystem.h"

void UWeaponNotifyStateBase::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UWeaponAnimMetaData* MetaData = Cast<UWeaponAnimMetaData>(Animation->FindMetaDataByClass(UWeaponAnimMetaData::StaticClass()));

	if (!IsValid(MetaData))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("WeaponNotifyStateBase::NotifyBegin: Animation '%s' does not have valid WeaponAnimMetaData."), *Animation->GetName());
		return;
	}

	WeaponAnimMetaData = MetaData;
	bIsInitialized = true;
	OnWeaponNotifyBegin(MeshComp, Animation, TotalDuration, WeaponAnimMetaData->Weapon);
}

void UWeaponNotifyStateBase::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!bIsInitialized)
		return;

	OnWeaponNotifyTick(MeshComp, Animation, FrameDeltaTime, FAnimNotifyEventReference(), WeaponAnimMetaData->Weapon);
}

void UWeaponNotifyStateBase::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!bIsInitialized)
		return;

	OnWeaponNotifyEnd(MeshComp, Animation, EventReference, WeaponAnimMetaData->Weapon);
}

void UWeaponNotifyStateBase::OnWeaponNotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, AWeaponBase* Weapon)
{
}

void UWeaponNotifyStateBase::OnWeaponNotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon)
{
}

void UWeaponNotifyStateBase::OnWeaponNotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference, AWeaponBase* Weapon)
{
}
