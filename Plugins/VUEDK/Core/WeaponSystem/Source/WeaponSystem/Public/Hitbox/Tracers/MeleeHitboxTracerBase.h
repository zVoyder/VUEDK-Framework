// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Hitbox/MeleeHitbox.h"
#include "UObject/Object.h"
#include "MeleeHitboxTracerBase.generated.h"

UCLASS(Abstract, BlueprintType, EditInlineNew)
class WEAPONSYSTEM_API UMeleeHitboxTracerBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Hitbox")
	FColor HitboxColor = FColor::Red;
	
protected:
	UPROPERTY()
	UMeleeHitbox* MeleeHitbox;
	
public:
	virtual UWorld* GetWorld() const override;
	
	void Init(UMeleeHitbox* InMeleeHitbox);
	
	TArray<FHitResult> TraceHitbox();

#if WITH_EDITOR
	virtual void DrawHitboxPreview(const FTransform& HitboxTransform);
#endif
	
#if !UE_BUILD_SHIPPING
	virtual void DrawDebugHitboxTrace(const AWeaponMelee* WeaponMelee, const FTransform& HitboxTransform);
#endif
	
protected:
	virtual FCollisionShape CreateCollisionShape();
	
	FCollisionQueryParams CreateCollisionParams() const;
	
	ECollisionChannel GetCollisionChannel() const;
	
	bool Check() const;
};
