// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Projectiles/ProjectileBase.h"
#include "Shooter/Shooter.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(MeshComponent);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(MeshComponent);
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->BounceVelocityStopSimulatingThreshold = 200.f;
}

void AProjectileBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ProcessProjectileLifeSpan(DeltaSeconds);
}

void AProjectileBase::Init(AActor* InInstigator, const TSubclassOf<UDamageType>& InDamageTypeClass, float InDamage, float InRange, float InSpeed, const FVector& InDirection)
{
	ProjectileInstigator = InInstigator;
	DamageTypeClass = InDamageTypeClass;
	Damage = InDamage;
	Range = InRange;
	ProjectileMovementComponent->bShouldBounce = true;
	InitVelocityAndLifeSpan(Range, InSpeed, InDirection);
	OnInit();
}

void AProjectileBase::OnPooledActorEndPlay_Implementation()
{
	Super::OnPooledActorEndPlay_Implementation();
	bIsProjectileAlive = false;
}

void AProjectileBase::InitVelocityAndLifeSpan_Implementation(float InRange, float InSpeed, const FVector& InDirection)
{
	SetVelocity(InDirection * InSpeed);
	StartProjectileLifeSpan(CalculateLifeSpan(InRange, InSpeed));
}

float AProjectileBase::CalculateLifeSpan_Implementation(const float InRange, const float InSpeed) const
{
	return InRange / InSpeed;
}

void AProjectileBase::SetDamage(float InDamage)
{
	if (InDamage < 0.f)
		InDamage = 0.f;

	Damage = InDamage;
}

void AProjectileBase::SetRange(float InRange)
{
	if (InRange < 0.f)
		InRange = 0.f;

	Range = InRange;
	if (bIsProjectileAlive)
		StartProjectileLifeSpan(CalculateLifeSpan(InRange, ProjectileMovementComponent->Velocity.Size()));
	else
		RemainingLifeSpan = CalculateLifeSpan(InRange, ProjectileMovementComponent->Velocity.Size());
}

void AProjectileBase::SetVelocity(const FVector NewVelocity) const
{
	ProjectileMovementComponent->SetUpdatedComponent(MeshComponent);
	ProjectileMovementComponent->Velocity = NewVelocity;
}

void AProjectileBase::SetProjectileInstigator(AActor* InInstigator)
{
	ProjectileInstigator = InInstigator;
}

float AProjectileBase::GetDamage() const
{
	return Damage;
}

float AProjectileBase::GetRange() const
{
	return Range;
}

FVector AProjectileBase::GetProjectileVelocity() const
{
	return ProjectileMovementComponent->Velocity;
}

AActor* AProjectileBase::GetProjectileInstigator() const
{
	return ProjectileInstigator;
}

void AProjectileBase::DisposeProjectile()
{
	if (!bIsProjectileAlive)
		return;

	NativeReleasePooledActor();
}

void AProjectileBase::ClearPooledActor_Implementation()
{
	ProjectileMovementComponent->StopMovementImmediately();
}

void AProjectileBase::OnInit_Implementation()
{
}

void AProjectileBase::OnProjectileHit_Implementation(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	DisposeProjectile();
}

void AProjectileBase::OnProjectileLifeSpanEnd_Implementation()
{
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(ProjectileMovementComponent))
	{
		UE_LOG(LogShooter, Warning, TEXT("ProjectileBase::BeginPlay: ProjectileMovementComponent is not valid."));
		return;
	}

	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AProjectileBase::OnProjectileHit);
}

void AProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!IsValid(ProjectileMovementComponent))
	{
		UE_LOG(LogShooter, Warning, TEXT("ProjectileBase::BeginPlay: ProjectileMovementComponent is not valid."));
		return;
	}

	ProjectileMovementComponent->OnProjectileBounce.RemoveDynamic(this, &AProjectileBase::OnProjectileHit);
}

void AProjectileBase::StartProjectileLifeSpan(const float InLifeSpan)
{
	RemainingLifeSpan = InLifeSpan;
	bIsProjectileAlive = true;
}

void AProjectileBase::ProcessProjectileLifeSpan(const float DeltaSeconds)
{
	if (!bIsProjectileAlive)
		return;

	RemainingLifeSpan -= DeltaSeconds;

	if (RemainingLifeSpan <= 0.f)
		EndProjectileLifeSpan();
}

void AProjectileBase::EndProjectileLifeSpan()
{
	OnProjectileLifeSpanEnd();
	DisposeProjectile();
}

void AProjectileBase::ProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	OnProjectileHit(ImpactResult, ImpactVelocity);
	OnProjectileHitEvent.Broadcast(ImpactResult);
}
