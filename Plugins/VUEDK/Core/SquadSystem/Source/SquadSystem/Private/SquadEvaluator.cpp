// Copyright VUEDK, Inc. All Rights Reserved.

#include "SquadEvaluator.h"

#include "SquadSystem.h"
#include "Member/MemberEvaluator.h"

USquadMember* USquadEvaluator::FindBestLeader(const TArray<USquadMember*>& Members) const
{
	float BestScore = -FLT_MAX;
	USquadMember* BestLeader = nullptr;

	for (USquadMember* Member : Members)
	{
		if (!IsValid(Member))
			continue;

		if (Member->CanBeLeader())
		{
			const float LeaderScore = Member->LeaderController->EvaluateLeadership(Members);
			BestScore = PostLeaderEvaluation(BestScore, Member, Members);

			if (LeaderScore > BestScore)
			{
				BestScore = LeaderScore;
				BestLeader = Member;
			}
		}
	}

	return BestLeader;
}

FHypotheticalFormation USquadEvaluator::FindBestFormation(const TArray<USquadMember*>& Members, const int32 MaxMembersPerSquad)
{
	const TArray<FHypotheticalSquad> HypotheticalSquads = GenerateHypotheticalSquads(Members, MaxMembersPerSquad);
	const TArray<FHypotheticalFormation> BestFormations = FindBestFormations(HypotheticalSquads, Members.Num());

	if (BestFormations.Num() == 0)
		return FHypotheticalFormation();

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	// Log all formations with members and squads scores
	for (const FHypotheticalFormation& Formation : BestFormations)
	{
		float TotalFormationScore = 0.f;
		for (const FHypotheticalSquad& Squad : Formation.Squads)
		{
			TotalFormationScore += Squad.Score;
		}

		UE_LOG(LogSquadSystem, VeryVerbose, TEXT("USquadEvaluator::FindBestFormation: Formation Score: %f with %d squads."), TotalFormationScore, Formation.Squads.Num());
		for (const FHypotheticalSquad& Squad : Formation.Squads)
		{
			FString MemberNames;
			for (const USquadMember* Member : Squad.Members)
			{
				if (IsValid(Member) && IsValid(Member->GetOwner()))
				{
					MemberNames += Member->GetOwner()->GetName() + TEXT(" ");
				}
			}

			UE_LOG(LogSquadSystem, VeryVerbose, TEXT("  Squad Score: %f | Members: %s"), Squad.Score, *MemberNames);
		}
	}
#endif
	
	switch (FormationPreference)
	{
	case EFormationPreference::PreferFewerSquads:
		return FindFewestSquadsFormation(BestFormations);
	case EFormationPreference::PreferMoreSquads:
		return FindMostSquadsFormation(BestFormations);
	case EFormationPreference::Balanced:
		return FindBalancedFormation(BestFormations);
	case EFormationPreference::Random:
		return GetRandomFormation(BestFormations);
	}

	return GetRandomFormation(BestFormations);
}

float USquadEvaluator::PostLeaderEvaluation_Implementation(const float InitialScore, USquadMember* Leader, const TArray<USquadMember*>& Members) const
{
	return InitialScore;
}

float USquadEvaluator::PostSquadEvaluation_Implementation(const float InitialScore, const TArray<USquadMember*>& Members) const
{
	return InitialScore;
}

FHypotheticalFormation USquadEvaluator::FindFewestSquadsFormation(const TArray<FHypotheticalFormation>& Formations)
{
	float MinSquadCount = FLT_MAX;
	TArray<FHypotheticalFormation> BestFormations;

	for (const FHypotheticalFormation& Formation : Formations)
	{
		if (Formation.Squads.Num() == MinSquadCount)
		{
			BestFormations.Add(Formation);
			continue;
		}

		if (Formation.Squads.Num() < MinSquadCount)
		{
			MinSquadCount = Formation.Squads.Num();
			BestFormations.Empty();
			BestFormations.Add(Formation);
		}
	}

	return BestFormations[FMath::RandRange(0, BestFormations.Num() - 1)];
}

FHypotheticalFormation USquadEvaluator::FindMostSquadsFormation(const TArray<FHypotheticalFormation>& Formations)
{
	float MaxSquadCount = 0;
	TArray<FHypotheticalFormation> BestFormations;

	for (const FHypotheticalFormation& Formation : Formations)
	{
		if (Formation.Squads.Num() == MaxSquadCount)
		{
			BestFormations.Add(Formation);
			continue;
		}

		if (Formation.Squads.Num() > MaxSquadCount)
		{
			MaxSquadCount = Formation.Squads.Num();
			BestFormations.Empty();
			BestFormations.Add(Formation);
		}
	}

	return BestFormations[FMath::RandRange(0, BestFormations.Num() - 1)];
}

FHypotheticalFormation USquadEvaluator::FindBalancedFormation(const TArray<FHypotheticalFormation>& Formations)
{
	float MinVariance = FLT_MAX;
	TArray<FHypotheticalFormation> BestFormations;

	for (const FHypotheticalFormation& Formation : Formations)
	{
		const int32 SquadCount = Formation.Squads.Num();
		if (SquadCount == 0)
			continue;

		float TotalMembers = 0.f;
		for (const FHypotheticalSquad& Squad : Formation.Squads)
			TotalMembers += Squad.Members.Num();

		const float AvgSize = TotalMembers / SquadCount;

		float Variance = 0.f;
		for (const FHypotheticalSquad& Squad : Formation.Squads)
		{
			const float Diff = Squad.Members.Num() - AvgSize;
			Variance += Diff * Diff;
		}
		Variance /= SquadCount;

		if (FMath::IsNearlyEqual(Variance, MinVariance))
		{
			BestFormations.Add(Formation);
		}
		else if (Variance < MinVariance)
		{
			MinVariance = Variance;
			BestFormations.Empty();
			BestFormations.Add(Formation);
		}
	}

	return BestFormations[FMath::RandRange(0, BestFormations.Num() - 1)];
}

FHypotheticalFormation USquadEvaluator::GetRandomFormation(const TArray<FHypotheticalFormation>& Formations)
{
	return Formations[FMath::RandRange(0, Formations.Num() - 1)];
}

TArray<FHypotheticalFormation> USquadEvaluator::FindBestFormations(const TArray<FHypotheticalSquad>& Squads, const int32 MemberCount)
{
	const int32 MaxMask = 1 << MemberCount;

	TArray<float> DP;
	DP.Init(-FLT_MAX, MaxMask);
	DP[0] = 0.f;

	TArray<TArray<TArray<int32>>> Formations;
	Formations.SetNum(MaxMask);
	Formations[0].Add({});

	for (int32 Mask = 0; Mask < MaxMask; ++Mask)
	{
		if (DP[Mask] < -FLT_MAX / 2)
			continue;

		for (int32 i = 0; i < Squads.Num(); ++i)
		{
			const FHypotheticalSquad& Squad = Squads[i];

			if (Mask & Squad.Mask)
				continue;

			const int32 NewMask = Mask | Squad.Mask;
			const float NewScore = DP[Mask] + Squad.Score;

			if (NewScore > DP[NewMask] + KINDA_SMALL_NUMBER)
			{
				DP[NewMask] = NewScore;
				Formations[NewMask].Empty();

				for (const auto& Formation : Formations[Mask])
				{
					TArray<int32> NewFormation = Formation;
					NewFormation.Add(i);
					Formations[NewMask].Add(NewFormation);
				}
			}
			else if (FMath::IsNearlyEqual(NewScore, DP[NewMask]))
			{
				for (const auto& Formation : Formations[Mask])
				{
					TArray<int32> NewFormation = Formation;
					NewFormation.Add(i);
					Formations[NewMask].Add(NewFormation);
				}
			}
		}
	}

	float MaxScore = -FLT_MAX;
	for (const float Score : DP)
		if (Score > MaxScore)
			MaxScore = Score;

	TSet<TArray<int32>> UniqueFormations;
	TArray<FHypotheticalFormation> Result;

	for (int32 Mask = 0; Mask < MaxMask; ++Mask)
	{
		if (!FMath::IsNearlyEqual(DP[Mask], MaxScore))
			continue;

		for (auto& FormationIndices : Formations[Mask])
		{
			TArray<int32> SortedIndices = FormationIndices;
			SortedIndices.Sort();

			bool bIsAlreadyInSet;
			UniqueFormations.Add(SortedIndices, &bIsAlreadyInSet);

			if (!bIsAlreadyInSet)
			{
				FHypotheticalFormation Formation;
				for (const int32 Index : FormationIndices)
					Formation.Squads.Add(Squads[Index]);

				Result.Add(Formation);
			}
		}
	}

	return Result;
}

TArray<FHypotheticalSquad> USquadEvaluator::GenerateHypotheticalSquads(const TArray<USquadMember*>& Members, const int32 MaxSquadSize) const
{
	TArray<FHypotheticalSquad> HypotheticalSquads;

	const int32 Size = Members.Num();
	const int32 MaxCombinations = (1 << Size) - 1;

	for (int32 Mask = 1; Mask <= MaxCombinations; ++Mask) // Power set
	{
		const int32 BitCount = FMath::CountBits(Mask);
		if (BitCount > MaxSquadSize)
			continue;

		TArray<USquadMember*> Combination;
		for (int32 i = 0; i < Size; ++i)
		{
			if (Mask & (1 << i))
				Combination.Add(Members[i]);
		}

		FHypotheticalSquad NewSquad(Mask, Combination);
		EvaluateSquad(NewSquad);
		HypotheticalSquads.Add(NewSquad);
	}

	return HypotheticalSquads;
}

float USquadEvaluator::EvaluateSquad(FHypotheticalSquad& Squad) const
{
	float Score = 0.0f;

	for (const USquadMember* Member : Squad.Members)
	{
		if (!IsValid(Member))
			continue;

		const UMemberEvaluator* MemberEvaluator = Member->MemberEvaluator;
		if (!IsValid(MemberEvaluator))
			continue;

		Score += MemberEvaluator->Evaluate(Squad.Members);
	}

	Score = PostSquadEvaluation(Score, Squad.Members);
	Squad.Score = Score;
	return Score;
}
