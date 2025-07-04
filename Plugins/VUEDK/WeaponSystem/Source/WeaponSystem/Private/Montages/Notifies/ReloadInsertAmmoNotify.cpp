// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/Notifies//ReloadInsertAmmoNotify.h"
#include "WeaponSystem.h"
#include "Weapons/WeaponFirearm.h"

void UReloadInsertAmmoNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UReloadInsertAmmoNotify::Notify: MeshComp is null."));
		return;
	}

	Init(MeshComp->GetOwner());
	
	if (!Check())
		return;

	Weapon->ReloadInsertAmmo();
}

AWeaponFirearm* UReloadInsertAmmoNotify::GetWeaponFirearm() const
{
	return Weapon;
}

bool UReloadInsertAmmoNotify::Check() const
{
	return IsValid(Weapon);
}

void UReloadInsertAmmoNotify::Init(AActor* InOwner)
{
	if (!IsValid(InOwner))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UReloadInsertAmmoNotify::Init: Owner is null."));
		return;
	}
	
	Weapon = Cast<AWeaponFirearm>(InOwner);

	if (!IsValid(Weapon))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UReloadInsertAmmoNotify::Init: Owner is not a weapon."));
		return;
	}
}
