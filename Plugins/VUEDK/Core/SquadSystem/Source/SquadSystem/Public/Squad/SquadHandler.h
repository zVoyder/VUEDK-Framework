// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SquadHandler.generated.h"

class USquad;

UCLASS(Abstract, Blueprintable, BlueprintType)
class SQUADSYSTEM_API USquadHandler : public UObject
{
	GENERATED_BODY()

	friend class USquad;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Squad Handler")
	bool bCanEverTick = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Squad Handler")
	bool bAutoActivate = true;
	
private:
	bool bIsActive;
	UPROPERTY()
	USquad* OwnerSquad;
	
public:
	void Init(USquad* InOwnerSquad);

	void Deinit();

	UFUNCTION(BlueprintCallable)
	void SetActive(const bool bNewActive);

	UFUNCTION(BlueprintPure)
	bool IsActive() const;

	UFUNCTION(BlueprintPure)
	USquad* GetSquad() const;
	
	virtual void Tick(const float DeltaTime);

	virtual void BeginPlay();

	virtual void EndPlay();

protected:
#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

	virtual UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnInit(USquad* InOwnerSquad);

	UFUNCTION(BlueprintNativeEvent)
	void OnAddedToSquad(USquad* InOwnerSquad);

	UFUNCTION(BlueprintNativeEvent)
	void OnRemovedFromSquad(USquad* InOwnerSquad);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Tick"))
	void ReceiveTick(float DeltaTime);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BeginPlay"))
	void ReceiveBeginPlay();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "EndPlay"))
	void ReceiveEndPlay();

private:
	bool Check() const;
};
