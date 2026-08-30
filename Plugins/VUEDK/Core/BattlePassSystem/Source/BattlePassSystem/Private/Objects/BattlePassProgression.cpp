// Copyright VUEDK, Inc. All Rights Reserved.

#include "Objects/BattlePassProgression.h"
#include "Data/BattlePassesSaveData.h"
#include "Objects/BattlePass.h"

void UBattlePassProgression::Init(UBattlePass* InRelatedBattlePass)
{
	RelatedBattlePass = InRelatedBattlePass;
	CurrentLevel = RelatedBattlePass->GetBattlePassData()->StartingLevel;
}

void UBattlePassProgression::LoadProgression(const FBattlePassSaveData& BattlePassSaveData)
{
	CurrentLevel = BattlePassSaveData.SavedLevel;
	SetLevelExperience(BattlePassSaveData.SavedLevelExperience, false);
}

void UBattlePassProgression::AddLevelExperience(const float Experience)
{
	SetLevelExperience(CurrentLevelExperience + Experience);
	OnExperienceAdded.Broadcast(Experience);
}

void UBattlePassProgression::RemoveLevelExperience(const float Experience)
{
	SetLevelExperience(CurrentLevelExperience - Experience);
	OnExperienceRemoved.Broadcast(Experience);
}

void UBattlePassProgression::SetLevelExperience(const float Experience, const bool bNotify)
{
	CurrentLevelExperience = Experience;

	const int32 OldLevel = CurrentLevel;
	CalculateLevel();
	if (bNotify)
	{
		OnExperienceSet.Broadcast(CurrentLevelExperience, CurrentLevel, OldLevel);
		if (OldLevel != CurrentLevel)
			OnLevelChanged.Broadcast(CurrentLevel, OldLevel);
	}
}

int32 UBattlePassProgression::GetLevel() const
{
	return CurrentLevel;
}

float UBattlePassProgression::GetLevelExperience() const
{
	return CurrentLevelExperience;
}

float UBattlePassProgression::GetExperienceForNextLevel() const
{
	return GetExperienceForLevel(CurrentLevel + 1);
}

float UBattlePassProgression::GetExperienceForLevel(const int32 Level) const
{
	const UCurveFloat* Curve = GetProgressionCurve();
	if (!IsValid(Curve))
		return 0.0f;

	return Curve->GetFloatValue(Level);
}

float UBattlePassProgression::GetTotalExperience() const
{
	float TotalExperience = 0.f;
	for (int32 i = 0; i <= CurrentLevel; i++)
		TotalExperience += GetExperienceForLevel(i);

	return TotalExperience + CurrentLevelExperience;
}

void UBattlePassProgression::CalculateLevel()
{
	float ExperienceForNextLevel = GetExperienceForNextLevel();

	while (CurrentLevelExperience >= ExperienceForNextLevel && ExperienceForNextLevel > 0.0f)
	{
		CurrentLevel++;
		CurrentLevelExperience -= ExperienceForNextLevel;
		ExperienceForNextLevel = GetExperienceForNextLevel();
	}
	
	CurrentLevelExperience = FMath::Max(CurrentLevelExperience, 0.0f); // Ensure experience doesn't go negative
}

UCurveFloat* UBattlePassProgression::GetProgressionCurve() const
{
	return Check() ? RelatedBattlePass->GetBattlePassData()->ProgressionCurve : nullptr;
}

bool UBattlePassProgression::Check() const
{
	return IsValid(RelatedBattlePass);
}
