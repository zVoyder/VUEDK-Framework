// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ShooterHandlerBase.h"
#include "Shooter/Behaviours/ShootModes/Base/ShootModeBase.h"
#include "ShootModesHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnShootModeHandlerRequestHandled,
	int32, RequestIndex,
	UShootModeBase*, ShootMode,
	bool, bDeployShoot,
	bool, bSuccess
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnShootSequenceEnded
);

UCLASS()
class WEAPONSYSTEM_API UShootModesHandler : public UShooterHandlerBase
{
	GENERATED_BODY()

public:
	FOnShootModeHandlerRequestHandled OnShootRequestHandled;
	FOnShootSequenceEnded OnShootSequenceEnded;
	
private:
	int32 ShootModeIndex = 0;
	int32 HandledRequests = 0;
	UPROPERTY()
	TArray<UShootModeBase*> ShootModes;

public:
	void SetModeIndex(const int32 NewIndex);
	
	bool RequestShoot(const EShootType ShootType);

	void EndSequence();
	
	bool IsProcessingRequest() const;

	int32 GetModeIndex() const;
	
	int32 GetHandledRequests() const;

	UShootModeBase* GetShootMode() const;
	
protected:
	virtual void OnInit() override;
	
	UFUNCTION()
	void OnHandleShootRequest(UShootModeBase* ShootMode, bool bDeployShoot, bool bSuccess);
};
