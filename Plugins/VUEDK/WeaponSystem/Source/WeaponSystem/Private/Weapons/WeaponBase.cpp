// Copyright VUEDK, Inc. All Rights Reserved.

#include "Weapons/WeaponBase.h"
#include "WeaponSystem.h"
#include "Factories/WeaponAnimFactory.h"
#include "GameFramework/Character.h"
#include "Montages/Data/WeaponAnimMetaData.h"
#include "UniversalObjectLocators/AnimInstanceLocatorFragment.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponMeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	SetRootComponent(WeaponMeshRoot);
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(WeaponMeshRoot);

#if WITH_EDITORONLY_DATA
	ForwardArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ForwardArrowComponent->SetArrowColor(FColor::Red);
	ForwardArrowComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	ForwardArrowComponent->SetArrowSize(.2f);
	RightArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrowComponent"));
	RightArrowComponent->SetArrowColor(FColor::Green);
	RightArrowComponent->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
	RightArrowComponent->SetArrowSize(.2f);
	UpArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("UpArrowComponent"));
	UpArrowComponent->SetArrowColor(FColor::Blue);
	UpArrowComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	UpArrowComponent->SetArrowSize(.2f);
#endif
}

void AWeaponBase::Init(APawn* InOwner, UObject* InPayload)
{
	SetPayload(InPayload);
	SetOwner(InOwner);
	SetInstigator(InOwner);
	SetOwnerAnimInstance();
}

void AWeaponBase::Equip()
{
	if (bIsEquipped)
		return;

	bIsEquipped = true;
	OnWeaponEquipped();
	OnWeaponEquippedEvent.Broadcast();
}

void AWeaponBase::Unequip()
{
	if (!bIsEquipped)
		return;

	bIsEquipped = false;
	OnWeaponUnequipped();
	OnWeaponUnequippedEvent.Broadcast();
}

bool AWeaponBase::WeaponAttack()
{
	if (NativeDeployWeaponAttack())
	{
		bIsWeaponAttacking = true;
		OnWeaponAttackSuccess();
		OnWeaponAttackSuccessEvent.Broadcast();
		return true;
	}

	bIsWeaponAttacking = false;
	OnWeaponAttackFail();
	OnWeaponAttackFailEvent.Broadcast();
	return false;
}

void AWeaponBase::EndWeaponAttack()
{
	bIsWeaponAttacking = false;
	OnEndWeaponAttack();
	OnEndWeaponAttackEvent.Broadcast();
}

void AWeaponBase::SetPayload(UObject* InPayload)
{
	Payload = InPayload;
}

void AWeaponBase::SetWeaponDamage(const float NewDamage)
{
	WeaponData.Damage = NewDamage;
}

float AWeaponBase::GetWeaponDamage() const
{
	return WeaponData.Damage;
}

UObject* AWeaponBase::GetPayload() const
{
	return Payload;
}

bool AWeaponBase::IsEquipped() const
{
	return bIsEquipped;
}

bool AWeaponBase::IsWeaponAttacking() const
{
	return bIsWeaponAttacking;
}

FWeaponData AWeaponBase::GetWeaponData() const
{
	return WeaponData;
}

UAnimInstance* AWeaponBase::GetOwnerAnimInstance() const
{
	return OwnerAnimInstance;
}

UAnimInstance* AWeaponBase::GetWeaponAnimInstance() const
{
	return WeaponMesh->GetAnimInstance();
}

bool AWeaponBase::IsPlayingWeaponMontage(const FWeaponMontageData& WeaponMontageData) const
{
	if (PlayingMontages.IsEmpty())
		return false;

	if (!IsValid(WeaponMontageData.GetWeaponMontage()) && !IsValid(WeaponMontageData.GetCharacterMontage()))
		return false;

	return PlayingMontages.Contains(WeaponMontageData.GetWeaponMontage()) || PlayingMontages.Contains(WeaponMontageData.GetCharacterMontage());
}

void AWeaponBase::ResumeWeaponMontage(const FWeaponMontageData& WeaponMontageData) const
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

void AWeaponBase::PauseWeaponMontage(const FWeaponMontageData& WeaponMontageData) const
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

void AWeaponBase::StartWeaponMontage(FWeaponMontageData WeaponMontageData, const float WeaponPlayRate, const float CharacterPlayRate)
{
	const bool bHasCharacterPriority = IsValid(WeaponMontageData.GetCharacterMontage()) && WeaponMontageData.MontageEndPriority == EMontageEndPriority::CharacterPriority;
	PlayMontageInternal(GetOwnerAnimInstance(), WeaponMontageData, WeaponMontageData.GetCharacterMontage(), CharacterPlayRate, WeaponMontageData.bCharacterMontageStopAllMontages, bHasCharacterPriority);
	PlayMontageInternal(GetWeaponAnimInstance(), WeaponMontageData, WeaponMontageData.GetWeaponMontage(), WeaponPlayRate, WeaponMontageData.bWeaponMontageStopAllMontages, !bHasCharacterPriority);
}

void AWeaponBase::StartWeaponMontageWithBlends(FWeaponMontageData WeaponMontageData, const float WeaponPlayRate, const float CharacterPlayRate, const FAlphaBlendArgs& WeaponBlendIn, const FAlphaBlendArgs& CharacterBlendIn)
{
	const bool bHasCharacterPriority = IsValid(WeaponMontageData.GetCharacterMontage()) && WeaponMontageData.MontageEndPriority == EMontageEndPriority::CharacterPriority;
	PlayMontageWithBlendInternal(GetOwnerAnimInstance(), WeaponMontageData, WeaponMontageData.GetCharacterMontage(), CharacterPlayRate, WeaponMontageData.bCharacterMontageStopAllMontages, CharacterBlendIn, bHasCharacterPriority);
	PlayMontageWithBlendInternal(GetWeaponAnimInstance(), WeaponMontageData, WeaponMontageData.GetWeaponMontage(), WeaponPlayRate, WeaponMontageData.bWeaponMontageStopAllMontages, WeaponBlendIn, !bHasCharacterPriority);
}

void AWeaponBase::StopWeaponMontage(const FWeaponMontageData WeaponMontageData)
{
	if (!PlayingMontages.Contains(WeaponMontageData.GetWeaponMontage()) && !PlayingMontages.Contains(WeaponMontageData.GetCharacterMontage()))
		return;

	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (IsValid(AnimInstance))
		AnimInstance->Montage_Stop(0.f, WeaponMontageData.GetCharacterMontage());

	UAnimInstance* WeaponAnimInstance = GetWeaponAnimInstance();
	if (IsValid(WeaponAnimInstance))
		WeaponAnimInstance->Montage_Stop(0.f, WeaponMontageData.GetWeaponMontage());

	RemovePlayingMontage(WeaponMontageData.GetWeaponMontage());
	RemovePlayingMontage(WeaponMontageData.GetCharacterMontage());
}

void AWeaponBase::StopWeaponMontageWithBlends(const FWeaponMontageData& WeaponMontageData, const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut)
{
	if (!PlayingMontages.Contains(WeaponMontageData.GetWeaponMontage()) && !PlayingMontages.Contains(WeaponMontageData.GetCharacterMontage()))
		return;

	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (IsValid(AnimInstance))
		AnimInstance->Montage_StopWithBlendOut(CharacterBlendOut, WeaponMontageData.GetCharacterMontage());

	UAnimInstance* WeaponAnimInstance = GetWeaponAnimInstance();
	if (IsValid(WeaponAnimInstance))
		WeaponAnimInstance->Montage_StopWithBlendOut(WeaponBlendOut, WeaponMontageData.GetWeaponMontage());

	RemovePlayingMontage(WeaponMontageData.GetWeaponMontage());
	RemovePlayingMontage(WeaponMontageData.GetCharacterMontage());
}

bool AWeaponBase::NativeDeployWeaponAttack()
{
	return DeployWeaponAttack();
}

void AWeaponBase::PlayMontageInternal(UAnimInstance* AnimInstance, FWeaponMontageData& WeaponMontageData, UAnimMontage* Montage, float PlayRate, bool bStopAll, bool bRegisterPlayingMontage)
{
	if (IsValid(AnimInstance) && IsValid(Montage))
	{
		SetWeaponMetaData(Montage);

		AnimInstance->Montage_Play(Montage, PlayRate, EMontagePlayReturnType::MontageLength, 0.f, bStopAll);

		if (bRegisterPlayingMontage)
		{
			WeaponMontageData.OnMontageBegin.Broadcast();
			WeaponMontageData.OnMontageEndedDelegate.Unbind();
			WeaponMontageData.OnMontageEndedDelegate.BindUObject(this, &AWeaponBase::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(WeaponMontageData.OnMontageEndedDelegate, Montage);
			AddPlayingMontage(Montage, WeaponMontageData);
		}
	}
}

void AWeaponBase::PlayMontageWithBlendInternal(UAnimInstance* AnimInstance, FWeaponMontageData& WeaponMontageData, UAnimMontage* Montage, float PlayRate, bool bStopAll, const FAlphaBlendArgs& BlendIn, bool bRegisterPlayingMontage)
{
	if (IsValid(AnimInstance) && IsValid(Montage))
	{
		SetWeaponMetaData(Montage);

		AnimInstance->Montage_PlayWithBlendIn(Montage, BlendIn, PlayRate, EMontagePlayReturnType::MontageLength, 0.f, bStopAll);
		
		if (bRegisterPlayingMontage)
		{
			WeaponMontageData.OnMontageBegin.Broadcast();
			WeaponMontageData.OnMontageEndedDelegate.Unbind();
			WeaponMontageData.OnMontageEndedDelegate.BindUObject(this, &AWeaponBase::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(WeaponMontageData.OnMontageEndedDelegate, Montage);
			AddPlayingMontage(Montage, WeaponMontageData);
		}
	}
}

void AWeaponBase::OnEndWeaponAttack_Implementation()
{
}

bool AWeaponBase::DeployWeaponAttack_Implementation()
{
	return true;
}

void AWeaponBase::OnWeaponAttackSuccess_Implementation()
{
}

void AWeaponBase::OnWeaponAttackFail_Implementation()
{
}

void AWeaponBase::OnWeaponEquipped_Implementation()
{
}

void AWeaponBase::OnWeaponUnequipped_Implementation()
{
}

void AWeaponBase::SetOwnerAnimInstance()
{
	UAnimInstance* AnimInstance;

	if (Owner->IsA<ACharacter>() && !bUseTag) // If it's a character, get the anim instance from the character
	{
		const ACharacter* Character = Cast<ACharacter>(Owner);
		if (IsValid(Character))
		{
			AnimInstance = Character->GetMesh()->GetAnimInstance();
			OwnerAnimInstance = AnimInstance;
			return;
		}
	}

	const USkeletalMeshComponent* SkeletalMeshComponent = Owner->FindComponentByTag<USkeletalMeshComponent>(AnimInstanceMeshTag);

	if (!IsValid(SkeletalMeshComponent))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("AWeaponBase::SetOwnerAnimInstance: No valid SkeletalMeshComponent found with tag '%s'. Animations will not be played."), *AnimInstanceMeshTag.ToString());
		return;
	}

	AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	OwnerAnimInstance = AnimInstance;
}

void AWeaponBase::AddPlayingMontage(UAnimMontage* Montage, const FWeaponMontageData& WeaponMontageData)
{
	PlayingMontages.Add(Montage, WeaponMontageData);
}

void AWeaponBase::RemovePlayingMontage(const UAnimMontage* Montage)
{
	PlayingMontages.Remove(Montage);
}

void AWeaponBase::SetWeaponMetaData(UAnimMontage* Montage)
{
	if (!IsValid(Montage))
		return;

	UWeaponAnimMetaData* MetaData = Cast<UWeaponAnimMetaData>(Montage->FindMetaDataByClass(UWeaponAnimMetaData::StaticClass()));
	if (IsValid(MetaData) && MetaData->Weapon == this)
		return;

	Montage->RemoveMetaData(MetaData);
	UWeaponAnimMetaData* NewMetaData = UWeaponAnimFactory::CreateWeaponAnimMetaData(this);
	if (!IsValid(NewMetaData))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("AWeaponBase::SetWeaponMetaData: Failed to create weapon anim meta data for montage '%s'."), *Montage->GetName());
		return;
	}

	Montage->AddMetaData(NewMetaData);
}

void AWeaponBase::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (!PlayingMontages.Contains(AnimMontage))
		return;
	
	const FWeaponMontageData& WeaponMontage = PlayingMontages[AnimMontage];
	WeaponMontage.OnMontageFinished.Broadcast(bInterrupted);
	RemovePlayingMontage(AnimMontage);
}
