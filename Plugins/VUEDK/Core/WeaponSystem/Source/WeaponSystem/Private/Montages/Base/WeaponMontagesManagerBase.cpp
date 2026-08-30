// Copyright VUEDK, Inc. All Rights Reserved.

#include "Montages/Base/WeaponMontagesManagerBase.h"
#include "WeaponSystem.h"
#include "Factories/WeaponAnimFactory.h"
#include "GameFramework/Character.h"
#include "Montages/Data/WeaponAnimMetaData.h"

UWeaponMontagesManagerBase::UWeaponMontagesManagerBase() : Weapon(nullptr),
                                                           bIsPlayingEquipMontage(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponMontagesManagerBase::Init(AWeaponBase* InWeapon)
{
	SetupWeapons(InWeapon);

	if (!Check())
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("UWeaponMontagesManagerBase::BeginPlay: Check failed for %s in %s. Disabling component."), *GetName(), *GetOwner()->GetName());
		UActorComponent::SetActive(false);
		return;
	}

	SetOwnerAnimInstance();
	BindEvents();
}

void UWeaponMontagesManagerBase::ResumeWeaponMontage(const FWeaponMontageData& WeaponMontageData) const
{
	if (!PlayingMontages.Contains(WeaponMontageData.GetWeaponMontage()) && !PlayingMontages.Contains(WeaponMontageData.GetCharacterMontage()))
		return;

	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (IsValid(AnimInstance) && IsValid(WeaponMontageData.GetCharacterMontage()))
		AnimInstance->Montage_Resume(WeaponMontageData.GetCharacterMontage());

	UAnimInstance* WeaponAnimInstance = GetWeaponAnimInstance();
	if (IsValid(WeaponAnimInstance) && IsValid(WeaponMontageData.GetWeaponMontage()))
		WeaponAnimInstance->Montage_Resume(WeaponMontageData.GetWeaponMontage());
}

void UWeaponMontagesManagerBase::PauseWeaponMontage(const FWeaponMontageData& WeaponMontageData) const
{
	if (!PlayingMontages.Contains(WeaponMontageData.GetWeaponMontage()) && !PlayingMontages.Contains(WeaponMontageData.GetCharacterMontage()))
		return;

	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (IsValid(AnimInstance) && IsValid(WeaponMontageData.GetCharacterMontage()))
		AnimInstance->Montage_Pause(WeaponMontageData.GetCharacterMontage());

	UAnimInstance* WeaponAnimInstance = GetWeaponAnimInstance();
	if (IsValid(WeaponAnimInstance) && IsValid(WeaponMontageData.GetWeaponMontage()))
		WeaponAnimInstance->Montage_Pause(WeaponMontageData.GetWeaponMontage());
}

void UWeaponMontagesManagerBase::StartWeaponMontage(FWeaponMontageData& WeaponMontageData, float WeaponPlayRate, float CharacterPlayRate)
{
	const bool bHasCharacterPriority = IsValid(WeaponMontageData.GetCharacterMontage()) && WeaponMontageData.MontageEndPriority == EMontageEndPriority::CharacterPriority;
	PlayMontageInternal(GetOwnerAnimInstance(), WeaponMontageData, WeaponMontageData.GetCharacterMontage(), CharacterPlayRate, WeaponMontageData.bCharacterMontageStopAllMontages, bHasCharacterPriority);
	PlayMontageInternal(GetWeaponAnimInstance(), WeaponMontageData, WeaponMontageData.GetWeaponMontage(), WeaponPlayRate, WeaponMontageData.bWeaponMontageStopAllMontages, !bHasCharacterPriority);
}

void UWeaponMontagesManagerBase::StartWeaponMontageWithBlends(FWeaponMontageData& WeaponMontageData, const float WeaponPlayRate, const float CharacterPlayRate, const FAlphaBlendArgs& WeaponBlendIn, const FAlphaBlendArgs& CharacterBlendIn)
{
	const bool bHasCharacterPriority = IsValid(WeaponMontageData.GetCharacterMontage()) && WeaponMontageData.MontageEndPriority == EMontageEndPriority::CharacterPriority;
	PlayMontageWithBlendInternal(GetOwnerAnimInstance(), WeaponMontageData, WeaponMontageData.GetCharacterMontage(), CharacterPlayRate, WeaponMontageData.bCharacterMontageStopAllMontages, CharacterBlendIn, bHasCharacterPriority);
	PlayMontageWithBlendInternal(GetWeaponAnimInstance(), WeaponMontageData, WeaponMontageData.GetWeaponMontage(), WeaponPlayRate, WeaponMontageData.bWeaponMontageStopAllMontages, WeaponBlendIn, !bHasCharacterPriority);
}

void UWeaponMontagesManagerBase::StopWeaponMontage(const FWeaponMontageData& WeaponMontageData) const
{
	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (IsValid(AnimInstance))
		AnimInstance->Montage_Stop(0.f, WeaponMontageData.GetCharacterMontage());

	UAnimInstance* WeaponAnimInstance = GetWeaponAnimInstance();
	if (IsValid(WeaponAnimInstance))
		WeaponAnimInstance->Montage_Stop(0.f, WeaponMontageData.GetWeaponMontage());
}

void UWeaponMontagesManagerBase::StopWeaponMontageWithBlends(const FWeaponMontageData& WeaponMontageData, const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut) const
{
	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (IsValid(AnimInstance))
		AnimInstance->Montage_StopWithBlendOut(CharacterBlendOut, WeaponMontageData.GetCharacterMontage());

	UAnimInstance* WeaponAnimInstance = GetWeaponAnimInstance();
	if (IsValid(WeaponAnimInstance))
		WeaponAnimInstance->Montage_StopWithBlendOut(WeaponBlendOut, WeaponMontageData.GetWeaponMontage());
}

bool UWeaponMontagesManagerBase::IsPlayingWeaponMontage(const FWeaponMontageData& WeaponMontageData) const
{
	if (PlayingMontages.IsEmpty())
		return false;

	if (!IsValid(WeaponMontageData.GetWeaponMontage()) && !IsValid(WeaponMontageData.GetCharacterMontage()))
		return false;

	return PlayingMontages.Contains(WeaponMontageData.GetWeaponMontage()) || PlayingMontages.Contains(WeaponMontageData.GetCharacterMontage());
}

bool UWeaponMontagesManagerBase::IsBusy_Implementation() const
{
	return IsEquipOrUnequipMontagePlaying();
}

bool UWeaponMontagesManagerBase::IsEquipOrUnequipMontagePlaying() const
{
	return bIsPlayingEquipMontage;
}

UAnimInstance* UWeaponMontagesManagerBase::GetOwnerAnimInstance() const
{
	return OwnerAnimInstance;
}

UAnimInstance* UWeaponMontagesManagerBase::GetWeaponAnimInstance() const
{
	if (!Check())
		return nullptr;

	const USkeletalMeshComponent* WeaponMesh = Weapon->GetWeaponMesh();
	if (!IsValid(WeaponMesh))
		return nullptr;

	return WeaponMesh->GetAnimInstance();
}

void UWeaponMontagesManagerBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!Check())
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("UWeaponMontagesManagerBase::EndPlay: Check failed for %s in %s."), *GetName(), *GetOwner()->GetName());
		return;
	}

	UnbindEvents();
}

void UWeaponMontagesManagerBase::SetupWeapons(AWeaponBase* InWeapon)
{
	Weapon = InWeapon;
}

void UWeaponMontagesManagerBase::BindEvents()
{
	Weapon->OnWeaponAttacked.AddDynamic(this, &UWeaponMontagesManagerBase::OnWeaponAttackSuccess);
	Weapon->OnWeaponAttackFailed.AddDynamic(this, &UWeaponMontagesManagerBase::OnWeaponAttackFail);
	Weapon->OnWeaponAttackEnded.AddDynamic(this, &UWeaponMontagesManagerBase::OnWeaponEndAttack);
	Weapon->OnWeaponEquippedEvent.AddDynamic(this, &UWeaponMontagesManagerBase::OnWeaponEquipped);
	Weapon->OnWeaponUnequippedEvent.AddDynamic(this, &UWeaponMontagesManagerBase::OnWeaponUnequipped);
}

void UWeaponMontagesManagerBase::UnbindEvents()
{
	Weapon->OnWeaponAttacked.RemoveDynamic(this, &UWeaponMontagesManagerBase::OnWeaponAttackSuccess);
	Weapon->OnWeaponAttackFailed.RemoveDynamic(this, &UWeaponMontagesManagerBase::OnWeaponAttackFail);
	Weapon->OnWeaponAttackEnded.RemoveDynamic(this, &UWeaponMontagesManagerBase::OnWeaponEndAttack);
	Weapon->OnWeaponEquippedEvent.RemoveDynamic(this, &UWeaponMontagesManagerBase::OnWeaponEquipped);
	Weapon->OnWeaponUnequippedEvent.RemoveDynamic(this, &UWeaponMontagesManagerBase::OnWeaponUnequipped);
}

void UWeaponMontagesManagerBase::OnWeaponAttackSuccess()
{
}

void UWeaponMontagesManagerBase::OnWeaponAttackFail()
{
}

void UWeaponMontagesManagerBase::OnWeaponEndAttack()
{
}

void UWeaponMontagesManagerBase::OnWeaponMontageBegin(const FWeaponMontageData& WeaponMontageData)
{
}

void UWeaponMontagesManagerBase::OnWeaponMontageFinished(const FWeaponMontageData& WeaponMontageData, bool bInterrupted)
{
}

bool UWeaponMontagesManagerBase::Check() const
{
	return IsValid(Weapon);
}

void UWeaponMontagesManagerBase::AddPlayingMontage(UAnimMontage* Montage, const FWeaponMontageData& WeaponMontageData)
{
	PlayingMontages.Add(Montage, WeaponMontageData);
}

void UWeaponMontagesManagerBase::RemovePlayingMontage(const UAnimMontage* Montage)
{
	PlayingMontages.Remove(Montage);
}

void UWeaponMontagesManagerBase::PlayMontageInternal(UAnimInstance* AnimInstance, FWeaponMontageData& WeaponMontageData, UAnimMontage* Montage, float PlayRate, bool bStopAll, bool bRegisterPlayingMontage)
{
	if (IsValid(AnimInstance) && IsValid(Montage))
	{
		SetWeaponMetaData(Montage);
		AnimInstance->Montage_Play(Montage, PlayRate, EMontagePlayReturnType::MontageLength, 0.f, bStopAll);

		if (bRegisterPlayingMontage)
			RegisterMontageBegin(AnimInstance, WeaponMontageData, Montage);
	}
}

void UWeaponMontagesManagerBase::PlayMontageWithBlendInternal(UAnimInstance* AnimInstance, FWeaponMontageData& WeaponMontageData, UAnimMontage* Montage, float PlayRate, bool bStopAll, const FAlphaBlendArgs& BlendIn, bool bRegisterPlayingMontage)
{
	if (IsValid(AnimInstance) && IsValid(Montage))
	{
		SetWeaponMetaData(Montage);
		AnimInstance->Montage_PlayWithBlendIn(Montage, BlendIn, PlayRate, EMontagePlayReturnType::MontageLength, 0.f, bStopAll);

		if (bRegisterPlayingMontage)
			RegisterMontageBegin(AnimInstance, WeaponMontageData, Montage);
	}
}

void UWeaponMontagesManagerBase::RegisterMontageBegin(UAnimInstance* AnimInstance, FWeaponMontageData& WeaponMontageData, UAnimMontage* Montage)
{
	OnWeaponMontageBegin(WeaponMontageData);
	OnAnyWeaponMontageBegin.Broadcast(WeaponMontageData);
	WeaponMontageData.OnWeaponMontageBegin.Broadcast();
	WeaponMontageData.OnMontageEndedDelegate.Unbind();
	WeaponMontageData.OnMontageEndedDelegate.BindUObject(this, &UWeaponMontagesManagerBase::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(WeaponMontageData.OnMontageEndedDelegate, Montage);
	AddPlayingMontage(Montage, WeaponMontageData);
}

void UWeaponMontagesManagerBase::SetOwnerAnimInstance()
{
	UAnimInstance* AnimInstance;
	AActor* WeaponOwner = Weapon->Owner;

	if (!IsValid(WeaponOwner))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("AWeaponBase::SetOwnerAnimInstance: Weapon has no valid owner. Animations will not be played."));
		return;
	}

	if (WeaponOwner->IsA<ACharacter>() && !bUseTag) // If it's a character, get the anim instance from the character
	{
		const ACharacter* Character = Cast<ACharacter>(WeaponOwner);
		if (IsValid(Character))
		{
			AnimInstance = Character->GetMesh()->GetAnimInstance();
			OwnerAnimInstance = AnimInstance;
			return;
		}
	}

	const USkeletalMeshComponent* SkeletalMeshComponent = WeaponOwner->FindComponentByTag<USkeletalMeshComponent>(AnimInstanceMeshTag);
	if (!IsValid(SkeletalMeshComponent))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("AWeaponBase::SetOwnerAnimInstance: No valid SkeletalMeshComponent found with tag '%s'. Animations will not be played."), *AnimInstanceMeshTag.ToString());
		return;
	}

	AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	OwnerAnimInstance = AnimInstance;
}

void UWeaponMontagesManagerBase::SetWeaponMetaData(UAnimMontage* Montage) const
{
	if (!IsValid(Montage))
		return;

	UWeaponAnimMetaData* MetaData = Cast<UWeaponAnimMetaData>(Montage->FindMetaDataByClass(UWeaponAnimMetaData::StaticClass()));
	if (IsValid(MetaData) && MetaData->Weapon == Weapon)
		return;

	Montage->RemoveMetaData(MetaData);
	UWeaponAnimMetaData* NewMetaData = UWeaponAnimFactory::CreateWeaponAnimMetaData(Weapon);
	if (!IsValid(NewMetaData))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("AWeaponBase::SetWeaponMetaData: Failed to create weapon anim meta data for montage '%s'."), *Montage->GetName());
		return;
	}

	Montage->AddMetaData(NewMetaData);
}

void UWeaponMontagesManagerBase::StartEquipMontage()
{
	if (EquipMontageData.GetCharacterMontage() == nullptr && EquipMontageData.GetWeaponMontage() == nullptr)
		return;

	bIsPlayingEquipMontage = true;
	EquipMontageData.OnWeaponMontageFinished.AddUniqueDynamic(this, &UWeaponMontagesManagerBase::OnWeaponReadyToUse);
	StartWeaponMontage(EquipMontageData, 1.0f, 1.0f);
}

void UWeaponMontagesManagerBase::StartUnequipMontage()
{
	if (UnequipMontageData.GetCharacterMontage() == nullptr && UnequipMontageData.GetWeaponMontage() == nullptr)
		return;

	bIsPlayingEquipMontage = true;
	UnequipMontageData.OnWeaponMontageFinished.AddUniqueDynamic(this, &UWeaponMontagesManagerBase::OnWeaponReadyToUse);
	StartWeaponMontage(UnequipMontageData, 1.0f, 1.0f);
}

void UWeaponMontagesManagerBase::OnWeaponEquipped()
{
	StartEquipMontage();
}

void UWeaponMontagesManagerBase::OnWeaponUnequipped()
{
	StartUnequipMontage();
}

void UWeaponMontagesManagerBase::OnWeaponReadyToUse(bool bInterrupted)
{
	bIsPlayingEquipMontage = false;
}

void UWeaponMontagesManagerBase::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (!PlayingMontages.Contains(AnimMontage))
		return;
	
	const FWeaponMontageData& WeaponMontage = PlayingMontages[AnimMontage];
	OnWeaponMontageFinished(WeaponMontage, bInterrupted);
	OnAnyWeaponMontageFinished.Broadcast(WeaponMontage, bInterrupted);
	WeaponMontage.OnWeaponMontageFinished.Broadcast(bInterrupted);
	RemovePlayingMontage(AnimMontage);
}
