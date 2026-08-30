// Copyright VUEDK, Inc. All Rights Reserved.

#include "WeaponCrosshair/Widgets/FirearmCrosshairWidget.h"
#include "WeaponSystem.h"
#include "Animation/WidgetAnimation.h"
#include "Weapons/Managers/ReloadManager.h"

void UFirearmCrosshairWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (!IsValid(Firearm))
		return;

	Firearm->OnAimEnabled.RemoveDynamic(this, &UFirearmCrosshairWidget::OnAimEnabled);
	Firearm->OnAimDisabled.RemoveDynamic(this, &UFirearmCrosshairWidget::OnAimDisabled);

	UReloadManager* ReloadManager = Firearm->GetReloadManager();
	if (IsValid(ReloadManager))
		return;

	ReloadManager->OnReloadStarted.RemoveDynamic(this, &UFirearmCrosshairWidget::OnReloadStart);
	ReloadManager->OnReloadEnded.RemoveDynamic(this, &UFirearmCrosshairWidget::OnReloadEnd);
	ReloadManager->OnReloadInsertedAmmo.RemoveDynamic(this, &UFirearmCrosshairWidget::OnReloadInsertAmmo);

	const UShooterBehaviourBase* Behaviour = Firearm->Shooter->GetShooterBehaviour(FirearmBehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->GetSpreadHandler()->OnProcessingSpread.RemoveDynamic(this, &UFirearmCrosshairWidget::OnSpreadChanged);
}

void UFirearmCrosshairWidget::OnInit_Implementation()
{
	Super::OnInit_Implementation();

	Firearm = Cast<AWeaponFirearm>(Weapon);

	if (!IsValid(Firearm))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UFirearmCrosshairWidget::OnInit_Implementation: Firearm is null."));
		return;
	}

	Firearm->OnAimEnabled.AddDynamic(this, &UFirearmCrosshairWidget::OnAimEnabled);
	Firearm->OnAimDisabled.AddDynamic(this, &UFirearmCrosshairWidget::OnAimDisabled);

	UReloadManager* ReloadManager = Firearm->GetReloadManager();
	if (IsValid(ReloadManager))
		return;

	ReloadManager->OnReloadStarted.AddDynamic(this, &UFirearmCrosshairWidget::OnReloadStart);
	ReloadManager->OnReloadEnded.AddDynamic(this, &UFirearmCrosshairWidget::OnReloadEnd);
	ReloadManager->OnReloadInsertedAmmo.AddDynamic(this, &UFirearmCrosshairWidget::OnReloadInsertAmmo);
	
	const UShooterBehaviourBase* Behaviour = Firearm->Shooter->GetShooterBehaviour(FirearmBehaviourIndex);
	if (!IsValid(Behaviour))
		return;

	Behaviour->GetSpreadHandler()->OnProcessingSpread.AddDynamic(this, &UFirearmCrosshairWidget::OnSpreadChanged);
}

void UFirearmCrosshairWidget::OnAimDisabled_Implementation()
{
}

void UFirearmCrosshairWidget::OnAimEnabled_Implementation()
{
}

void UFirearmCrosshairWidget::OnSpreadChanged_Implementation(float Spread)
{
}

void UFirearmCrosshairWidget::OnReloadStart_Implementation(const FReloadRequest& Request)
{
}

void UFirearmCrosshairWidget::OnReloadEnd_Implementation(const FReloadRequest& Request, bool bInterrupted)
{
}

void UFirearmCrosshairWidget::OnReloadInsertAmmo_Implementation(UShooterBehaviourBase* Behaviour, int32 InsertedAmmo, int32 RemainingAmmo)
{
}

void UFirearmCrosshairWidget::AnimateCrosshair(UWidgetAnimation* CrosshairAnimation)
{
	if (!IsValid(CrosshairAnimation))
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UFirearmCrosshairWidget::AnimateCrosshair: CrosshairAnimation is null."));
		return;
	}

	if (!Check())
		return;

	const float MaxSpread = Firearm->GetWeaponMaxSpread();
	const float Spread = Firearm->GetWeaponSpread();

	const float Ratio = FMath::Clamp(Spread / MaxSpread, 0.0f, 1.0f);
	const float Start = Ratio - 0.0001f;
	const float End = Ratio;

	PlayAnimationTimeRange(CrosshairAnimation, Start, End, 1);
}

bool UFirearmCrosshairWidget::Check() const
{
	return IsValid(Firearm);
}
