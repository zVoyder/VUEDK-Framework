// Copyright VUEDK, Inc. All Rights Reserved.

#include "Effects/BloodDecal.h"
#include "DismembererSystem.h"
#include "Effects/BloodDecalsPool.h"
#include "Components/DecalComponent.h"
#include "Factories/DismembererFactory.h"

ABloodDecal::ABloodDecal()
{
	PrimaryActorTick.bCanEverTick = true;
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	SetRootComponent(DecalComponent);
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
	OnInit();
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

void ABloodDecal::OnDecalLifeTimeProcessNative(float Timer, float InDuration) const
{
	OnDecalLifeTimeProcess(Timer, InDuration);
}

void ABloodDecal::OnDecalFadeInNative(const float Timer, const float InFadeInTime, const float InDuration) const
{
	if (!IsValid(DynamicDecalMaterial) || InFadeInTime <= 0.f)
		return;
	
	const float Alpha = FMath::Clamp(Timer / InFadeInTime, 0.f, 1.f);
	DynamicDecalMaterial->SetScalarParameterValue(DecalFadeParameterName, Alpha);
	OnDecalFadeIn(Timer, InFadeInTime, InDuration);
}

void ABloodDecal::OnDecalFadeOutNative(const float Timer, const float InFadeOutTime, const float InDuration) const
{
	if (!IsValid(DynamicDecalMaterial) || InFadeOutTime <= 0.f)
		return;
	
	const float ElapsedFadeOut = Timer - (InDuration - InFadeOutTime);
	const float Alpha = FMath::Clamp(1.f - (ElapsedFadeOut / InFadeOutTime), 0.f, 1.f);
	
	DynamicDecalMaterial->SetScalarParameterValue(DecalFadeParameterName, Alpha);
	OnDecalFadeOut(Timer, InFadeOutTime, InDuration);
}

void ABloodDecal::OnInit_Implementation()
{
}

void ABloodDecal::OnDecalFadeIn_Implementation(const float Timer, const float InFadeInTime, const float InDuration) const
{
}

void ABloodDecal::OnDecalLifeTimeProcess_Implementation(float Timer, float InDuration) const
{
}

void ABloodDecal::OnDecalFadeOut_Implementation(const float Timer, const float InFadeOutTime, const float InDuration) const
{
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
	OnDecalLifeTimeProcessNative(CurrentTimer, Duration);
	
	if (CurrentTimer < FadeInTime)
	{
		OnDecalFadeInNative(CurrentTimer, FadeInTime, Duration);
		return;
	}

	if (CurrentTimer >= Duration - FadeOutTime)
		OnDecalFadeOutNative(CurrentTimer, FadeOutTime, Duration);
	
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
