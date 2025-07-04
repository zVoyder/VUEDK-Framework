// Copyright VUEDK, Inc. All Rights Reserved.

#include "Effects/CameraShakeFeedback.h"
#include "Kismet/GameplayStatics.h"

void UCameraShakeFeedback::OnPlay_Implementation()
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	CameraManager->StartCameraShake(CameraShakeClass, Scale, PlaySpace, UserPlaySpaceRot);
	Finish();
}
