// Copyright VUEDK, Inc. All Rights Reserved.

#include "CombatSlotsProviders/Base/CombatSlotsProviderBase.h"
#include "CombatAllocationSystem.h"
#include "NavigationSystem.h"

void UCombatSlotsProviderBase::Tick(float DeltaTime)
{
	UpdateSlots();
}

bool UCombatSlotsProviderBase::TryGetSlot(const AActor* Requester, FCombatSlot& OutSlot) const
{
	int32 SlotIndex = -1;
	if (HasSlot(Requester, SlotIndex) && CombatSlots.IsValidIndex(SlotIndex))
	{
		OutSlot = CombatSlots[SlotIndex];
		return true;
	}

	return false;
}

bool UCombatSlotsProviderBase::TryGetSlotAt(const int32 SlotIndex, FCombatSlot& OutSlot) const
{
	if (CombatSlots.IsValidIndex(SlotIndex))
	{
		OutSlot = CombatSlots[SlotIndex];
		return true;
	}

	return false;
}

bool UCombatSlotsProviderBase::ClaimSlot(AActor* Requester, int32& OutSlotIndex)
{
	int32 ExistingSlotIndex;
	if (HasSlot(Requester, ExistingSlotIndex))
		return false;

	if (!ChooseBestSlot(Requester, OutSlotIndex))
		return false;

	return OccupySlot(Requester, OutSlotIndex);
}

bool UCombatSlotsProviderBase::ClaimSlotAt(AActor* Requester, const int32 SlotIndex)
{
	int32 ExistingSlotIndex;
	if (HasSlot(Requester, ExistingSlotIndex))
		return false;

	return OccupySlot(Requester, SlotIndex);
}

bool UCombatSlotsProviderBase::ReleaseSlot(const AActor* Requester)
{
	if (const int32* FoundIndex = OccupantsLookupMap.Find(Requester))
		return ClearSlotAt(*FoundIndex);

	return false;
}

bool UCombatSlotsProviderBase::ReleaseSlotAt(const int32 SlotIndex)
{
	return ClearSlotAt(SlotIndex);
}

void UCombatSlotsProviderBase::ReleaseAllSlots()
{
	for (FCombatSlot& Slot : CombatSlots)
		Slot.Occupant.Reset();
}

bool UCombatSlotsProviderBase::HasSlot(const AActor* Requester, int32& OutSlotIndex) const
{
	if (const int32* FoundIndex = OccupantsLookupMap.Find(Requester))
	{
		OutSlotIndex = *FoundIndex;
		return true;
	}

	OutSlotIndex = -1;
	return false;
}

bool UCombatSlotsProviderBase::HasAvailableSlots() const
{
	for (const FCombatSlot& Slot : CombatSlots)
	{
		if (Slot.IsAvailable())
			return true;
	}

	return false;
}

void UCombatSlotsProviderBase::OnInit()
{
	Super::OnInit();
	CreateSlots();

	for (TObjectPtr<UCombatSlotValidatorBase>& Validator : SlotValidators)
	{
		if (IsValid(Validator))
			Validator->Init(CombatAllocator, ProviderTag);
	}

	for (TObjectPtr<UCombatSlotEvaluatorBase>& Evaluator : SlotEvaluators)
	{
		if (IsValid(Evaluator))
			Evaluator->Init(CombatAllocator, ProviderTag);
	}

#if !UE_BUILD_SHIPPING
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	World->GetTimerManager().SetTimerForNextTick(this, &UCombatSlotsProviderBase::DebugDrawSlots);
#endif
}

void UCombatSlotsProviderBase::ComputeSlotLocation_Implementation(const int32& SlotIndex, const FVector& ProviderLocation, FVector& OutLocation)
{
	OutLocation = ProviderLocation;
}

bool UCombatSlotsProviderBase::ChooseBestSlot(const AActor* Requester, int32& OutBestSlotIndex) const
{
	if (!Check())
		return false;

	const FVector ProviderLocation = GetProviderLocation();
	float BestScore = TNumericLimits<float>::Lowest();
	OutBestSlotIndex = -1;
	for (int32 i = 0; i < CombatSlots.Num(); ++i)
	{
		const FCombatSlot& Slot = CombatSlots[i];

		if (!Slot.IsAvailable())
			continue;

		const float Score = EvaluateSlot(Slot, ProviderLocation, Requester);
		if (Score > BestScore)
		{
			BestScore = Score;
			OutBestSlotIndex = i;
		}
	}

	return OutBestSlotIndex != -1;
}

float UCombatSlotsProviderBase::EvaluateSlot(const FCombatSlot& Slot, const FVector& ProviderLocation, const AActor* Requester) const
{
	float TotalScore = 0.0f;
	for (const TObjectPtr<UCombatSlotEvaluatorBase>& Evaluator : SlotEvaluators)
	{
		if (!IsValid(Evaluator))
			continue;

		TotalScore += Evaluator->Evaluate(Slot, ProviderLocation, Requester);
	}

	return TotalScore;
}

void UCombatSlotsProviderBase::CreateSlots()
{
	for (int32 i = 0; i < NumSlots; ++i)
	{
		FCombatSlot NewSlot;
		CombatSlots.Add(NewSlot);
	}
}

void UCombatSlotsProviderBase::ComputeSlotValidation(FCombatSlot& Slot, const FVector& ProviderLocation)
{
	bool bSlotValid = true; // Assume the slot is valid until a validator proves otherwise

	if (SlotValidators.IsEmpty())
	{
		// If no validators are set, consider the slot valid by default
		Slot.bIsValid = bSlotValid;
		return;
	}

	for (const TObjectPtr<UCombatSlotValidatorBase>& Validator : SlotValidators)
	{
		if (!IsValid(Validator))
			continue;

		if (!Validator->ValidateSlot(Slot, ProviderLocation))
		{
			bSlotValid = false;
			break;
		}
	}

	Slot.bIsValid = bSlotValid;
}

bool UCombatSlotsProviderBase::OccupySlot(AActor* Requester, const int32 SlotIndex)
{
	RemoveInvalidOccupantsKeys(); // Clean up any invalid keys before attempting to occupy a slot

	if (!CombatSlots.IsValidIndex(SlotIndex))
		return false;

	FCombatSlot& Slot = CombatSlots[SlotIndex];
	if (!Slot.IsAvailable())
		return false;

	Slot.Occupant = Requester;
	OccupantsLookupMap.Add(Requester, SlotIndex);
	return true;
}

void UCombatSlotsProviderBase::UpdateSlots()
{
	if (!Check())
		return;

	const FVector ProviderLocation = GetProviderLocation();
	for (int i = 0; i < CombatSlots.Num(); i++)
		ComputeSlot(i, ProviderLocation);
}

void UCombatSlotsProviderBase::ComputeSlot(const int32 SlotIndex, const FVector& ProviderLocation)
{
	FCombatSlot& Slot = CombatSlots[SlotIndex];
	ComputeSlotLocation(SlotIndex, ProviderLocation, Slot.Location);

	if (bProjectToNavMesh)
	{
		const bool bProjectionSucceeded = ProjectSlotOnNavMesh(Slot.Location);
		if (bProjectValidate && !bProjectionSucceeded)
		{
			Slot.bIsValid = false;
			ClearSlotIfNeeded(Slot);
			return;
		}
	}

	ComputeSlotValidation(Slot, ProviderLocation);
	ClearSlotIfNeeded(Slot);
}

bool UCombatSlotsProviderBase::ProjectSlotOnNavMesh(FVector& Location) const
{
	FNavLocation NavLocation;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (IsValid(NavSys) && NavSys->ProjectPointToNavigation(Location, NavLocation, NavMeshProjectionExtent))
	{
		Location = NavLocation.Location;
		return true;
	}

	return false;
}

bool UCombatSlotsProviderBase::ClearSlotAt(const int32 SlotIndex)
{
	if (!CombatSlots.IsValidIndex(SlotIndex))
		return false;

	FCombatSlot& Slot = CombatSlots[SlotIndex];
	ClearSlot(Slot);
	return true;
}

void UCombatSlotsProviderBase::ClearSlot(FCombatSlot& Slot)
{
	if (AActor* Occupant = Slot.Occupant.Get())
		OccupantsLookupMap.Remove(Occupant);

	Slot.Occupant.Reset();
}

void UCombatSlotsProviderBase::ClearSlotIfNeeded(FCombatSlot& Slot)
{
	if (!Slot.bIsValid && Slot.Occupant.IsValid())
		ClearSlot(Slot);
}

FVector UCombatSlotsProviderBase::GetProviderLocation() const
{
	return CombatAllocator->GetOwner()->GetActorLocation();
}

void UCombatSlotsProviderBase::RemoveInvalidOccupantsKeys()
{
	for (auto MapIt = OccupantsLookupMap.CreateIterator(); MapIt; ++MapIt)
	{
		TWeakObjectPtr<const AActor> Key = MapIt.Key();
		if (!Key.IsValid())
			MapIt.RemoveCurrent();
	}
}

bool UCombatSlotsProviderBase::Check() const
{
	return IsValid(CombatAllocator);
}

#if !UE_BUILD_SHIPPING
void UCombatSlotsProviderBase::DebugDrawSlots()
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;

	World->GetTimerManager().SetTimerForNextTick(this, &UCombatSlotsProviderBase::DebugDrawSlots);
	if (!FCombatAllocationSystemModule::CVarDrawDebugCombatSlots.GetValueOnGameThread())
		return;

	for (int i = 0; i < CombatSlots.Num(); i++)
	{
		const FCombatSlot& Slot = CombatSlots[i];

		FColor DebugColor = Slot.bIsValid ? FColor::Green : FColor::Red;
		if (Slot.Occupant.IsValid())
			DebugColor = FColor::Blue;

		DrawDebugSphere(World, Slot.Location, 25.0f, 12, DebugColor, false, -1.0f);
		FString SlotName = FString::Printf(TEXT("%d"), i);
		FString SlotStatus = Slot.bIsValid ? Slot.Occupant.IsValid() ? FString::Printf(TEXT("Occupied by %s"), *Slot.Occupant.Get()->GetName()) : TEXT("Free") : TEXT("Invalid");
		FString Label = FString::Printf(TEXT("Slot: %s\nStatus: %s"), *SlotName, *SlotStatus);
		DrawDebugString(World, Slot.Location + FVector(0, 0, 30.0f), Label, nullptr, DebugColor, 0.f, true);
	}
}
#endif
