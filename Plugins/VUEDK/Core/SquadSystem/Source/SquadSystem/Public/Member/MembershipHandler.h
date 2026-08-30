// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/MemberObjectBase.h"
#include "MembershipHandler.generated.h"

UENUM()
enum ESquadResolution : uint8
{
	DefaultPreferred,
	DirectPreferred,
	Direct,
	Rebuild
};

UCLASS(Blueprintable, BlueprintType)
class SQUADSYSTEM_API UMembershipHandler : public UMemberObjectBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	UFUNCTION(BlueprintCallable)
	void SendJoinRequestTo(USquadMember* Target) const;
	
	UFUNCTION(BlueprintNativeEvent)
	bool EvaluateJoinRequestFrom(USquadMember* Target, TEnumAsByte<ESquadResolution>& OutResolution);
		
	UFUNCTION(BlueprintNativeEvent)
	bool EvaluateMembershipValidity(TEnumAsByte<ESquadResolution>& OutResolution);
	
private:
	bool NegotiateJoinWith(USquadMember* Target, USquadMember*& OutCounterpart, TEnumAsByte<ESquadResolution>& OutResolution) const;
};
