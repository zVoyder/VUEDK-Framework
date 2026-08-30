// Copyright VUEDK, Inc. All Rights Reserved.

#include "BattlePassSubsystem.h"
#include "BattlePassSystem.h"
#include "BattlePassSystemSettings.h"
#include "Data/BattlePassesSaveData.h"
#include "Factories/BattlePassFactory.h"
#include "Utility/BattlePassUtility.h"

void UBattlePassSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UBattlePassSystemSettings* Settings = GetDefault<UBattlePassSystemSettings>();
	if (!Settings)
	{
		UE_LOG(LogBattlePassSystem, Error, TEXT("UBattlePassSubsystem::Initialize: Failed to get Battle Pass System Settings."));
		return;
	}

	CreateSaveMode(Settings);
	CreateBattlePasses(Settings);
	LoadBattlePasses();
	UBattlePassUtility::Init(this);
}

UBattlePass* UBattlePassSubsystem::GetBattlePass(const FGameplayTag& BattlePassTag) const
{
	return BattlePasses.Contains(BattlePassTag) ? BattlePasses[BattlePassTag] : nullptr;
}

const TMap<FGameplayTag, UBattlePass*>& UBattlePassSubsystem::GetAllBattlePasses() const
{
	return BattlePasses;
}

void UBattlePassSubsystem::CreateBattlePasses(const UBattlePassSystemSettings* Settings)
{
	for (const auto& Pair : Settings->BattlePasses)
	{
		const FGameplayTag& BattlePassTag = Pair.Key;
		UBattlePassData* BattlePassData = Pair.Value.LoadSynchronous();

		if (!IsValid(BattlePassData))
		{
			UE_LOG(LogBattlePassSystem, Error, TEXT("UBattlePassSubsystem::CreateBattlePasses: Failed to load Battle Pass Data for tag %s."), *BattlePassTag.ToString());
			continue;
		}

		const bool bIsBattlePassActive = Settings->bAllBattlePassesActive || Settings->ActiveBattlePasses.Contains(BattlePassTag);
		UBattlePass* BattlePass = UBattlePassFactory::CreateBattlePass(BattlePassTag, BattlePassData, bIsBattlePassActive);
		if (!IsValid(BattlePass))
		{
			UE_LOG(LogBattlePassSystem, Error, TEXT("UBattlePassSubsystem::CreateBattlePasses: Failed to create Battle Pass for tag %s."), *BattlePassTag.ToString());
			continue;
		}
		
		BattlePasses.Add(BattlePassTag, BattlePass);
		BattlePass->GetProgression()->OnExperienceSet.AddUniqueDynamic(this, &UBattlePassSubsystem::OnAnyBattlePassExperienceSet); // Save whenever experience is set on any Battle Pass progression
		BattlePass->OnUpdateRewards.BindUObject(this, &UBattlePassSubsystem::SaveBattlePasses); // Save whenever rewards are updated
	}
}

void UBattlePassSubsystem::CreateSaveMode(const UBattlePassSystemSettings* Settings)
{
	BattlePassSaveMode = UBattlePassFactory::CreateBattlePassSaveMode(Settings->SaveModeClass, this);
	if (!IsValid(BattlePassSaveMode))
	{
		UE_LOG(LogBattlePassSystem, Error, TEXT("UBattlePassSubsystem::CreateSaveMode: Failed to create Battle Pass Save Mode. Progress will not be saved or loaded."));
		return;
	}

	BattlePassSaveMode->OnBattlePassLoadComplete.BindUObject(this, &UBattlePassSubsystem::OnLoadBattlePassesComplete);
	BattlePassSaveMode->OnBattlePassLoadFailed.BindUObject(this, &UBattlePassSubsystem::OnBattlePassFailedToLoad);
}

void UBattlePassSubsystem::SaveBattlePasses() const
{
	if (!IsValid(BattlePassSaveMode))
		return;

	BattlePassSaveMode->Save();
}

void UBattlePassSubsystem::LoadBattlePasses() const
{
	if (!IsValid(BattlePassSaveMode))
		return;

	BattlePassSaveMode->Load();
}

void UBattlePassSubsystem::OnLoadBattlePassesComplete(FBattlePassesSaveData SavedData)
{
	for (const auto& Pair : SavedData.SavedBattlePasses)
	{
		const FGameplayTag& BattlePassTag = Pair.Key;
		const FBattlePassSaveData& BattlePassSaveData = Pair.Value;
		UBattlePass* BattlePass = BattlePasses.FindRef(BattlePassTag);
		if (!IsValid(BattlePass))
		{
			UE_LOG(LogBattlePassSystem, Warning, TEXT("UBattlePassSubsystem::OnLoadBattlePassesComplete: Invalid Battle Pass found for tag %s while loading progression data."), *BattlePassTag.ToString());
			continue;
		}
		
		BattlePass->LoadBattlePass(BattlePassSaveData);
	}

	OnBattlePassesLoaded.Broadcast(true, false);
}

void UBattlePassSubsystem::OnBattlePassFailedToLoad(bool bIsError)
{
	OnBattlePassesLoaded.Broadcast(false, bIsError);
}

void UBattlePassSubsystem::OnAnyBattlePassExperienceSet(float NewExperience, int32 NewLevel, int32 OldLevel)
{
	SaveBattlePasses();
}
