// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Data/GameEffectsCueData.h"
#include "GameEffectsCueNotify.generated.h"

UCLASS()
class GAMEEFFECTSCUESYSTEM_API UGameEffectsCueNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameEffects|Cue")
	UGameEffectsCueData* CueData;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
