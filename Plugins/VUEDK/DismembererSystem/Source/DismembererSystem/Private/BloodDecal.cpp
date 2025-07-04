// Copyright VUEDK, Inc. All Rights Reserved.

#include "BloodDecal.h"
#include "BloodDecalsPool.h"
#include "Components/DecalComponent.h"
#include "Utility/DismembererFactory.h"

ABloodDecal::ABloodDecal()
{
	PrimaryActorTick.bCanEverTick = true;
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	SetRootComponent(DecalComponent);
}

void ABloodDecal::BeginPlay()
{
	Super::BeginPlay();
	BloodDecalsPool = GetWorld()->GetSubsystem<UBloodDecalsPool>();
	
	const UDismembererSystemSettings* Settings = GetDefault<UDismembererSystemSettings>();
	if (!IsValid(Settings))
		return;

	DecalFadeParameterName = Settings->DecalFadeParameterName;
}

void ABloodDecal::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ProcessDecalLifeTime(DeltaSeconds);
}

void ABloodDecal::Init(UMaterialInterface* DecalMaterial, const FVector& NormalOrientation, const FVector& Size, const float DecalDuration, const int SortOrder, const float FadeIn, const float FadeOut)
{
	if (!IsValid(DecalMaterial))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("ABloodDecal::Init: Invalid DecalMaterial."));
		return;
	}

	if (!Check())
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("ABloodDecal::Init: BloodDecalsPool is not valid."));
		return;
	}

	DynamicDecalMaterial = UMaterialInstanceDynamic::Create(DecalMaterial, this);
	if (!IsValid(DynamicDecalMaterial))
	{
		UE_LOG(LogDismembermentSystem, Error, TEXT("ABloodDecal::Init: Failed to create dynamic material."));
		return;
	}

	DecalComponent->DecalSize = Size;
	DecalComponent->SetDecalMaterial(DynamicDecalMaterial);
	SetActorRotation(NormalOrientation.Rotation());
	DecalComponent->SetSortOrder(SortOrder);
	DecalComponent->bUseAttachParentBound = true;
	StartDecalLifeTime(DecalDuration, FadeIn, FadeOut);
}

void ABloodDecal::OnDecalLifeTimeProcess(float Timer, float InDuration) const
{
}

void ABloodDecal::OnDecalFadeIn(const float Timer, const float InFadeInTime, const float InDuration) const
{
	if (!IsValid(DynamicDecalMaterial) || InFadeInTime <= 0.f)
		return;
	
	const float Alpha = FMath::Clamp(Timer / InFadeInTime, 0.f, 1.f);
	DynamicDecalMaterial->SetScalarParameterValue(DecalFadeParameterName, Alpha);
}

void ABloodDecal::OnDecalFadeOut(const float Timer, const float InFadeOutTime, const float InDuration) const
{
	if (!IsValid(DynamicDecalMaterial) || InFadeOutTime <= 0.f)
		return;
	
	const float ElapsedFadeOut = Timer - (InDuration - InFadeOutTime);
	const float Alpha = FMath::Clamp(1.f - (ElapsedFadeOut / InFadeOutTime), 0.f, 1.f);
	
	DynamicDecalMaterial->SetScalarParameterValue(DecalFadeParameterName, Alpha);
}

void ABloodDecal::StartDecalLifeTime(const float DecalDuration, const float InFadeInTime, const float InFadeOutTime)
{
	CurrentTimer = 0.f;
	Duration = DecalDuration;
	FadeInTime = InFadeInTime;
	FadeOutTime = InFadeOutTime;
	bIsSpawned = true;
}

void ABloodDecal::ProcessDecalLifeTime(float DeltaSeconds)
{
	if (!bIsSpawned)
		return;

	CurrentTimer += DeltaSeconds;
	OnDecalLifeTimeProcess(CurrentTimer, Duration);
	
	if (CurrentTimer < FadeInTime)
	{
		OnDecalFadeIn(CurrentTimer, FadeInTime, Duration);
		return;
	}

	if (CurrentTimer >= Duration - FadeOutTime)
		OnDecalFadeOut(CurrentTimer, FadeOutTime, Duration);
	
	if (CurrentTimer >= Duration)
		EndDecalLifeTime();
}

void ABloodDecal::EndDecalLifeTime()
{
	Duration = 0.f;
	FadeInTime = 0.f;
	FadeOutTime = 0.f;
	CurrentTimer = 0.f;
	bIsSpawned = false;
	BloodDecalsPool->ReleaseDecal(this);
}

bool ABloodDecal::Check() const
{
	return IsValid(BloodDecalsPool);
}
