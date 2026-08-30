// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Objects/RespawnSelectors/Base/SmartRespawnSelectorBase.h"
#include "Objects/SmartRespawnBehaviours/RespawnStreamingSource.h"
#include "Objects/SmartRespawnBehaviours/Base/SmartRespawnBehaviour.h"
#include "SmartRespawnFactory.generated.h"

UCLASS()
class SMARTRESPAWNSYSTEM_API USmartRespawnFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static USmartRespawnRegisterSaverBase* CreateRegisterSaver(USmartRespawnSubsystem* SmartRespawnSubsystem);

	static USmartRespawnBehaviour* CreateRespawnBehaviour(USmartRespawnSubsystem* SmartRespawnSubsystem, const FSmartRespawnSnapshot& RespawnSnapshot);

	static USmartRespawnSelectorBase* CreateRespawnSelector(USmartRespawnSubsystem* SmartRespawnSubsystem, TSubclassOf<USmartRespawnSelectorBase> SelectorClass);

	static ARespawnStreamingSource* SpawnRespawnStreamingSource(const USmartRespawnSubsystem* SmartRespawnSubsystem, APlayerController* PlayerController, const FGameplayTag& RespawnTag, const FSmartRespawnSnapshot& RespawnSnapshot);
};
