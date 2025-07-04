// Copyright VUEDK, Inc. All Rights Reserved.

#include "GameEffectsCueNotify.h"
#include "GameEffectsCueSystem.h"
#include "GameEffectsCueManager.h"

void UGameEffectsCueNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp))
	{
		UE_LOG(LogGameEffectsCueSystem, Warning, TEXT("UGameEffectsCueNotify::Notify: MeshComp is not valid."));
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner))
	{
		UE_LOG(LogGameEffectsCueSystem, Warning, TEXT("UGameEffectsCueNotify::Notify: Owner is not valid."));
		return;
	}

	UGameEffectsCueManager::StartGameEffectsCue(Owner, CueData);
}
