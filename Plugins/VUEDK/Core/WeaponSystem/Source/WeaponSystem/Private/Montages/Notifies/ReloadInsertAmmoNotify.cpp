// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/Notifies//ReloadInsertAmmoNotify.h"
#include "WeaponSystem.h"
#include "Montages/Data/WeaponAnimMetaData.h"
#include "Weapons/WeaponFirearm.h"
#include "Weapons/Managers/ReloadManager.h"

void UReloadInsertAmmoNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UReloadInsertAmmoNotify::Notify: MeshComp is null."));
		return;
	}

	AActor* InOwner = MeshComp->GetOwner();
	if (!IsValid(InOwner))
		return;
	
	Weapon = Cast<AWeaponFirearm>(InOwner);

	if (!IsValid(Weapon))
	{
		const UWeaponAnimMetaData* MetaData = Cast<UWeaponAnimMetaData>(Animation->FindMetaDataByClass(UWeaponAnimMetaData::StaticClass()));
		Weapon = MetaData ? Cast<AWeaponFirearm>(MetaData->Weapon) : nullptr;
		
		if (!IsValid(Weapon))
		{
			UE_LOG(LogWeaponSystem, Warning, TEXT("UReloadInsertAmmoNotify::Notify: Weapon is not valid."));
			return;
		}
	}

	UReloadManager* ReloadManager = Weapon->GetReloadManager();
	if (!IsValid(ReloadManager))
		return;

	ReloadManager->InsertAmmoInBehaviourOfIndex(ReloadBehaviourIndex);
}
