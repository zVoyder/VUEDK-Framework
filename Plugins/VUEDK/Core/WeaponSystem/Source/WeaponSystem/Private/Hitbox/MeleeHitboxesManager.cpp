// Copyright VUEDK, Inc. All Rights Reserved.

#include "Hitbox/MeleeHitboxesManager.h"
#include "WeaponSystem.h"
#include "Hitbox/MeleeHitbox.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/WeaponMelee.h"

UMeleeHitboxesManager::UMeleeHitboxesManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMeleeHitboxesManager::Init(AWeaponMelee* InWeaponMelee)
{
	if (!IsValid(InWeaponMelee))
	{
		UE_LOG(LogWeaponSystem, Error, TEXT("UMeleeHitboxesManager::Init: InWeaponMelee is not valid."));
		SetActive(false);
		return;
	}
	WeaponMelee = InWeaponMelee;

	WeaponMelee->GetComponents<UMeleeHitbox>(MeleeHitboxes);
	if (MeleeHitboxes.IsEmpty())
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("UMeleeHitboxesManager::Init: No MeleeHitboxes found on weapon: %s"), *WeaponMelee->GetName());
		SetActive(false);
		return;
	}

	for (UMeleeHitbox* MeleeHitbox : MeleeHitboxes)
	{
		if (IsValid(MeleeHitbox))
			MeleeHitbox->Init(WeaponMelee);
	}
}

void UMeleeHitboxesManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceDamageHitbox();
}

void UMeleeHitboxesManager::SetTracingHitboxes(const bool bEnableTrace)
{
	bIsTracingHitboxes = bEnableTrace;

	if (bEnableTrace)
	{
		const UWeaponMeleeAttackData* CurrentAttack = WeaponMelee->GetCurrentAttack();
		if (!IsValid(CurrentAttack))
			return;

		if (CurrentAttack->bAllowsMultipleHits && CurrentAttack->AttackTickRate > 0.f)
			StartTraceTimer(CurrentAttack->AttackTickRate);
	}
	else
	{
		StopTraceTimer();
		ClearHitActors();
	}
}

void UMeleeHitboxesManager::TraceDamageHitbox()
{
	if (!bIsTracingHitboxes)
		return;

	if (!Check())
		return;

	TMap<UMeleeHitbox*, TArray<FHitResult>> HitMap = TMap<UMeleeHitbox*, TArray<FHitResult>>();
	for (UMeleeHitbox* MeleeHitbox : MeleeHitboxes)
	{
		if (!IsValid(MeleeHitbox))
			continue;

		TArray<FHitResult> HitboxHitResults = MeleeHitbox->TraceHitbox();

		if (HitboxHitResults.Num() > 0)
			HitMap.Add(MeleeHitbox, HitboxHitResults);
	}

	if (CheckInterruptCollisions(HitMap))
		return;

	HandleHitActors(HitMap);
}

void UMeleeHitboxesManager::HandleHitActors(TMap<UMeleeHitbox*, TArray<FHitResult>> HitboxesHitResults)
{
    const UWeaponMeleeAttackData* CurrentAttack = WeaponMelee->GetCurrentAttack();
    if (!IsValid(CurrentAttack))
        return;

    const bool bAllowsMultipleHits = CurrentAttack->bAllowsMultipleHits;
    const int32 MaxHits = CurrentAttack->MaxHits;
    TSet<AActor*> ActorsInThisTick;
    const FWeaponMeleeData& WeaponMeleeData = WeaponMelee->WeaponMeleeData;

    for (auto& HitboxResults : HitboxesHitResults)
    {
        UMeleeHitbox* MeleeHitbox = HitboxResults.Key;
        TArray<FHitResult>& HitResults = HitboxResults.Value;

        for (const FHitResult& HitResult : HitResults)
        {
            AActor* HitActor = HitResult.GetActor();
            if (!IsValid(HitActor))
                continue;
        	
            ActorsInThisTick.Add(HitActor);
            const bool bWasInsideLastTick = ActorsCurrentlyInside.Contains(HitActor);

            if (!bAllowsMultipleHits)
            {
                const int32* ExistingCount = ActorsHitCounts.Find(HitActor);
                if (ExistingCount && *ExistingCount > 0)
                    continue;
            }

            const int32 CurrentCount = ActorsHitCounts.Contains(HitActor) ? ActorsHitCounts[HitActor] : 0;
            bool bShouldApplyDamage = false;
            if (CurrentCount == 0 && !bWasInsideLastTick)
            {
                bShouldApplyDamage = true;
            }
            else if (bAllowsMultipleHits)
            {
                if (!bWasInsideLastTick)
                {
                    if (MaxHits == 0 || CurrentCount < MaxHits)
                        bShouldApplyDamage = true;
                }
            }

            if (bShouldApplyDamage)
            {
            	WeaponMelee->CallPreHitEvent(MeleeHitbox, HitResult, MeleeHitbox->GetProcessedDamage());
            	
                UGameplayStatics::ApplyPointDamage(
                    HitActor,
                    MeleeHitbox->GetProcessedDamage(),
                    HitResult.ImpactNormal,
                    HitResult,
                    WeaponMelee->GetInstigatorController(),
                    WeaponMelee->GetOwner(),
                    MeleeHitbox->GetDamageType()
                );

                int32& RefCount = ActorsHitCounts.FindOrAdd(HitActor);
                ++RefCount;
            	WeaponMelee->CallHitEvent(MeleeHitbox, HitResult, MeleeHitbox->GetProcessedDamage());
            }
        }
    }
	
    ActorsCurrentlyInside = MoveTemp(ActorsInThisTick);
}

bool UMeleeHitboxesManager::CheckInterruptCollisions(TMap<UMeleeHitbox*, TArray<FHitResult>> HitboxesHitResults) const
{
	for (auto& HitboxResults : HitboxesHitResults)
	{
		TArray<FHitResult>& HitResults = HitboxResults.Value;

		for (const FHitResult& HitResult : HitResults)
		{
			FWeaponMeleeData& WeaponMeleeData = WeaponMelee->WeaponMeleeData;
			if (WeaponMeleeData.AttackInterruptChannels.Contains(HitResult.Component->GetCollisionObjectType()))
			{
				WeaponMelee->CallInterruptEvent(HitboxResults.Key, HitResult);
				return true;
			}
		}
	}

	return false;
}

void UMeleeHitboxesManager::ClearHitActors()
{
	ActorsCurrentlyInside.Empty();
	ActorsHitCounts.Empty();
}

void UMeleeHitboxesManager::StartTraceTimer(const float IntervalSeconds)
{
	if (!GetWorld())
		return;

	GetWorld()->GetTimerManager().SetTimer(
		TraceTimer,
		this,
		&UMeleeHitboxesManager::OnTraceTimerTick,
		IntervalSeconds,
		true
	);
}

void UMeleeHitboxesManager::StopTraceTimer()
{
	if (!GetWorld())
		return;

	GetWorld()->GetTimerManager().ClearTimer(TraceTimer);
}

void UMeleeHitboxesManager::OnTraceTimerTick()
{
	ClearHitActors();
}

bool UMeleeHitboxesManager::Check() const
{
	return IsValid(WeaponMelee);
}
