// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Magazine.h"
#include "UObject/Object.h"
#include "MagazinesManager.generated.h"

class UShooter;

UCLASS(BlueprintType, EditInlineNew)
class WEAPONSYSTEM_API UMagazinesManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<UMagazine*> Magazines;

private:
	UPROPERTY()
	UShooter* Shooter;
	
public:
	void Init(UShooter* InShooter);

	virtual UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintPure)
	UMagazine* GetMagazine(const int32 Index) const;

	UFUNCTION(BlueprintPure)
	bool HasMagazineOfIndex(const int32 Index) const;
};
