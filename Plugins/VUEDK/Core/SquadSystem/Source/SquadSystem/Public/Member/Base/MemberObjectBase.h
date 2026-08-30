// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MemberObjectBase.generated.h"

class USquad;
class USquadMember;

UCLASS(Abstract, NotBlueprintable, NotBlueprintType, EditInlineNew)
class SQUADSYSTEM_API UMemberObjectBase : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	USquadMember* OwnerMember;
	
public:
	void Init(USquadMember* InOwnerMember);

	virtual void Tick(float DeltaTime);
	
	virtual void BeginPlay();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	
	UFUNCTION(BlueprintPure)
	USquadMember* GetOwnerMember() const;

	UFUNCTION(BlueprintPure)
	AActor* GetOwnerActor() const;

	UFUNCTION(BlueprintPure)
	USquad* GetOwnerSquad() const;

	UFUNCTION(BlueprintPure)
	USquadMember* GetOwnerSquadLeader() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnJoinSquad(USquad* NewSquad);

	UFUNCTION(BlueprintNativeEvent)
	void OnLeaveSquad(USquad* OldSquad);

protected:
#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif
	
	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintNativeEvent)
	void OnInit(USquadMember* InOwnerMember);
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Tick")
	void ReceiveTick(float DeltaTime);
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Begin Play")
	void ReceiveBeginPlay();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "End Play")
	void ReceiveEndPlay(const EEndPlayReason::Type EndPlayReason);
};
