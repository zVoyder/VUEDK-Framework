// Copyright VUEDK, Inc. All Rights Reserved.

#include "Thrower/Throwables/ThrowableGrenade.h"
#include "Kismet/GameplayStatics.h"

AThrowableGrenade::AThrowableGrenade()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AThrowableGrenade::OnThrowableLifeSpanEnd_Implementation()
{
	Super::OnThrowableLifeSpanEnd_Implementation();
	Explode();
}

void AThrowableGrenade::Explode()
{
	InvalidateLifeSpanTimer();
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	AController* InstigatorController = IsValid(ThrowableInstigator) ? ThrowableInstigator->GetInstigatorController() : GetInstigatorController();
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		Damage,
		GetActorLocation(),
		ExplosionRadius,
		DamageTypeClass,
		IgnoredActors,
		this,
		InstigatorController,
		bDoFullDamage,
		ExplosionChannel
	);
	
	OnExplosion();
	NativeReleasePooledActor();
}

void AThrowableGrenade::OnExplosion_Implementation()
{
}
