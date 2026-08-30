// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatAllocator.h"
#include "Base/CombatAllocatorObjectBase.h"
#include "CombatSlotsEvaluators/Base/CombatSlotEvaluatorBase.h"
#include "CombatSlotsValidators/Base/CombatSlotValidatorBase.h"
#include "Data/CombatSlot.h"
#include "UObject/Object.h"
#include "CombatSlotsProviderBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class COMBATALLOCATIONSYSTEM_API UCombatSlotsProviderBase : public UCombatAllocatorObjectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0, UIMin = 0), Category = "Combat Slots Provider|Generation")
	int32 NumSlots = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Slots Provider|Generation", Instanced)
	TArray<TObjectPtr<UCombatSlotValidatorBase>> SlotValidators;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Slots Provider|Generation")
	TArray<TObjectPtr<UCombatSlotEvaluatorBase>> SlotEvaluators;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Slots Provider|Navigation")
	bool bProjectToNavMesh = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bProjectToNavMesh", EditConditionHides), Category = "Combat Slots Provider|Navigation")
	bool bProjectValidate = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat Slots Provider|Navigation",
		meta = (EditCondition = "bProjectToNavMesh", EditConditionHides, ClampMin = 0.f, UIMin = 0.f))
	FVector NavMeshProjectionExtent = FVector(100.f, 100.f, 100.f);

private:
	TArray<FCombatSlot> CombatSlots;
	UPROPERTY()
	TMap<TWeakObjectPtr<const AActor>, int32> OccupantsLookupMap;

public:
	void Tick(float DeltaTime);

	bool TryGetSlot(const AActor* Requester, FCombatSlot& OutSlot) const;

	bool TryGetSlotAt(const int32 SlotIndex, FCombatSlot& OutSlot) const;

	bool ClaimSlot(AActor* Requester, int32& OutSlotIndex);

	bool ClaimSlotAt(AActor* Requester, const int32 SlotIndex);

	bool ReleaseSlot(const AActor* Requester);

	bool ReleaseSlotAt(const int32 SlotIndex);

	void ReleaseAllSlots();

	bool HasSlot(const AActor* Requester, int32& OutSlotIndex) const;

	bool HasAvailableSlots() const;

protected:
	virtual void OnInit() override;

	UFUNCTION(BlueprintNativeEvent)
	void ComputeSlotLocation(const int32& SlotIndex, const FVector& ProviderLocation, FVector& OutLocation);

private:
	bool ChooseBestSlot(const AActor* Requester, int32& OutBestSlotIndex) const;

	float EvaluateSlot(const FCombatSlot& Slot, const FVector& ProviderLocation, const AActor* Requester) const;
	
	void CreateSlots();
	
	void ComputeSlotValidation(FCombatSlot& Slot, const FVector& ProviderLocation);
	
	bool OccupySlot(AActor* Requester, const int32 SlotIndex);
	
	void UpdateSlots();

	void ComputeSlot(const int32 SlotIndex, const FVector& ProviderLocation);

	bool ProjectSlotOnNavMesh(FVector& Location) const;

	bool ClearSlotAt(int32 SlotIndex);

	void ClearSlot(FCombatSlot& Slot);

	void ClearSlotIfNeeded(FCombatSlot& Slot);

	FVector GetProviderLocation() const;
	
	void RemoveInvalidOccupantsKeys();
	
	bool Check() const;

#if !UE_BUILD_SHIPPING
	void DebugDrawSlots();
#endif
};
