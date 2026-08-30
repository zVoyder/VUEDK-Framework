// Copyright VUEDK, Inc. All Rights Reserved.

#include "MaterialCutoutController.h"

UMaterialCutoutController::UMaterialCutoutController()
{
	PrimaryComponentTick.bCanEverTick = true;
	CutoutParameterInfo = FMaterialParameterInfo("CutoutAmount");
	CutoutParameterInfo.Index = 1;
	CutoutParameterInfo.Association = EMaterialParameterAssociation::LayerParameter;
}

void UMaterialCutoutController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PerformTransition(DeltaTime);
}

void UMaterialCutoutController::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void UMaterialCutoutController::StartTransition(const float Duration, const float FromCutoutAmount, const float ToCutoutAmount)
{
	StartTransitionCutoutAmount = FromCutoutAmount;
	TargetTransitionCutoutAmount = ToCutoutAmount;
	TransitionDuration = Duration;
	TransitionTimer = Duration;
	bIsPerformingTransition = true;
	OnTransitionStarted.Broadcast();
}

void UMaterialCutoutController::StopTransition()
{
	bIsPerformingTransition = false;
	StartTransitionCutoutAmount = 0.f;
	TargetTransitionCutoutAmount = 0.f;
	TransitionDuration = 0.f;
	TransitionTimer = 0.f;
	OnTransitionStopped.Broadcast();
}

void UMaterialCutoutController::ResumeTransition()
{
	if (TransitionTimer <= 0.f)
		return;
	
	bIsPerformingTransition = true;
	OnTransitionResumed.Broadcast();
}

void UMaterialCutoutController::PauseTransition()
{
	if (!bIsPerformingTransition)
		return;
	
	bIsPerformingTransition = false;
	OnTransitionPaused.Broadcast();
}

void UMaterialCutoutController::SetMaterials(const TArray<UMaterialInstanceDynamic*>& InMaterials)
{
	DynamicMaterials = InMaterials;

	for (UMaterialInstanceDynamic* Material : DynamicMaterials)
	{
		if (!IsValid(Material))
			continue;

		Material->SetScalarParameterValueByInfo(CutoutParameterInfo, CurrentCutoutAmount);
	}
}

void UMaterialCutoutController::AddMaterial(UMaterialInstanceDynamic* InMaterial)
{
	InMaterial->SetScalarParameterValueByInfo(CutoutParameterInfo, CurrentCutoutAmount);
	DynamicMaterials.AddUnique(InMaterial);
}

void UMaterialCutoutController::RemoveMaterial(UMaterialInstanceDynamic* InMaterial)
{
	InMaterial->SetScalarParameterValueByInfo(CutoutParameterInfo, 1.0f);
	DynamicMaterials.Remove(InMaterial);
}

void UMaterialCutoutController::ClearMaterials()
{
	for (UMaterialInstanceDynamic* Material : DynamicMaterials)
	{
		if (!IsValid(Material))
			continue;

		Material->SetScalarParameterValueByInfo(CutoutParameterInfo, 1.0f);
	}
	
	DynamicMaterials.Empty();
}

void UMaterialCutoutController::SetCutoutAmount(float InCutoutAmount)
{
	InCutoutAmount = FMath::Clamp(InCutoutAmount, 0.0f, 1.0f);
	for (UMaterialInstanceDynamic* Material : DynamicMaterials)
	{
		if (!IsValid(Material))
			continue;

		Material->SetScalarParameterValueByInfo(CutoutParameterInfo, InCutoutAmount);
	}

	CurrentCutoutAmount = InCutoutAmount;
}

void UMaterialCutoutController::ModifyCutoutAmount(const float InCutoutAmount)
{
	SetCutoutAmount(CurrentCutoutAmount + InCutoutAmount);
}

float UMaterialCutoutController::GetCurrentCutoutAmount() const
{
	return CurrentCutoutAmount;
}

TArray<UMaterialInstanceDynamic*> UMaterialCutoutController::GetMaterials() const
{
	return DynamicMaterials;
}

void UMaterialCutoutController::Init()
{
	CurrentCutoutAmount = DefaultCutoutAmount;
	if (bAutoSetDynamicMaterials)
		AutoCreateAndSetDynamicMaterials();
}

void UMaterialCutoutController::AutoCreateAndSetDynamicMaterials()
{
	const AActor* Owner = GetOwner();

	if (!IsValid(Owner))
		return;

	TArray<UMeshComponent*> MeshComponents;
	Owner->GetComponents<UMeshComponent>(MeshComponents);
	for (UMeshComponent* MeshComponent : MeshComponents)
	{
		if (!IsValid(MeshComponent))
			continue;

		const int32 MaterialCount = MeshComponent->GetNumMaterials();
		for (int32 Index = 0; Index < MaterialCount; Index++)
		{
			UMaterialInstanceDynamic* DynamicMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(Index);
			
			if (!IsValid(DynamicMaterial))
				continue;

			AddMaterial(DynamicMaterial);
		}
	}
}

void UMaterialCutoutController::PerformTransition(const float DeltaTime)
{
	if (!bIsPerformingTransition)
		return;
	
	if (TransitionTimer <= 0.f)
	{
		SetCutoutAmount(TargetTransitionCutoutAmount);
		StopTransition();
		OnTransitionCompleted.Broadcast();
		return;
	}
	
	TransitionTimer -= DeltaTime;
	const float Alpha = 1.0f - (TransitionTimer / TransitionDuration);
	const float NewCutoutAmount = FMath::Lerp(StartTransitionCutoutAmount, TargetTransitionCutoutAmount, Alpha);
	SetCutoutAmount(NewCutoutAmount);
}
