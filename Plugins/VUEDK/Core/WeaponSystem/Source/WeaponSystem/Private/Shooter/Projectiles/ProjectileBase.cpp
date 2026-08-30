// Copyright VUEDK, Inc. All Rights Reserved.

#include "Shooter/Projectiles/ProjectileBase.h"
#include "WeaponSystem.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ProjectileCollision"));
	SetRootComponent(ProjectileCollision);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->BounceVelocityStopSimulatingThreshold = 200.f;
}

void AProjectileBase::Tick(float DeltaSeconds)
{
	if (!bIsProjectileAlive)
		return;

	Super::Tick(DeltaSeconds);
	ProcessProjectileLifeSpan(DeltaSeconds);
	UpdateProjectileSpeed();
}

void AProjectileBase::Init(AActor* InInstigator, const TSubclassOf<UDamageType>& InDamageTypeClass, float InDamage, float InRange, float InInitialSpeed, const FVector& InDirection)
{
	ProjectileInstigator = InInstigator;
	DamageTypeClass = InDamageTypeClass;
	Damage = InDamage;

	UE_LOG(LogShooter, Log, TEXT("ProjectileBase::Init: Instigator=%s, DamageType=%s, Damage=%.2f, Range=%.2f, InitialSpeed=%.2f, Direction=%s"),
		*GetNameSafe(InInstigator), *GetNameSafe(DamageTypeClass), InDamage, InRange, InInitialSpeed, *InDirection.ToString());
	
	Range = InRange;
	Direction = InDirection.GetSafeNormal();
	InitialSpeed = InInitialSpeed;
	UpdateLifeSpan();
	ProjectileMovementComponent->bShouldBounce = true;
	UpdateProjectileMovementComponent();
	SetSpeed(InitialSpeed);
	StartProjectileLifeSpan();
	OnInit();
}

void AProjectileBase::OnPooledActorEndPlay_Implementation()
{
	Super::OnPooledActorEndPlay_Implementation();
	bIsProjectileAlive = false;
}

void AProjectileBase::SetDamage(float InDamage)
{
	if (InDamage < 0.f)
		InDamage = 0.f;

	Damage = InDamage;
}

void AProjectileBase::SetDamageType(const TSubclassOf<UDamageType> InDamageTypeClass)
{
	DamageTypeClass = InDamageTypeClass;
}

void AProjectileBase::SetRange(const float InRange)
{
	Range = FMath::Max(0.f, InRange);
	UpdateLifeSpan();
}

void AProjectileBase::SetDirection(const FVector InDirection)
{
	Direction = InDirection.GetSafeNormal();
	SetVelocity(CurrentSpeed * Direction);
}

void AProjectileBase::SetSpeed(const float NewSpeed)
{
	CurrentSpeed = NewSpeed;
	SetVelocity(CurrentSpeed * Direction);
}

void AProjectileBase::SetVelocity(const FVector NewVelocity) const
{
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

	CalculateCurveArea();
	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AProjectileBase::NotifyProjectileHit);
}

void AProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!IsValid(ProjectileMovementComponent))
	{
		UE_LOG(LogShooter, Warning, TEXT("ProjectileBase::BeginPlay: ProjectileMovementComponent is not valid."));
		return;
	}

	ProjectileMovementComponent->OnProjectileBounce.RemoveDynamic(this, &AProjectileBase::NotifyProjectileHit);
}

void AProjectileBase::UpdateLifeSpan()
{
	LifeSpan = CalculateLifeSpan(Range, InitialSpeed);
	CurrentLifeSpan = FMath::Min(CurrentLifeSpan, LifeSpan);
}

void AProjectileBase::StartProjectileLifeSpan()
{
	CurrentLifeSpan = 0.f;
	bIsProjectileAlive = true;
}

void AProjectileBase::OnProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
}

float AProjectileBase::CalculateLifeSpan(const float InRange, const float InSpeed) const
{
	if (InRange <= 0.f || InSpeed <= 0.f)
		return 0.f;

	return InRange / (InSpeed * CurveArea);
}

void AProjectileBase::CalculateCurveArea()
{
	if (!bUseSpeedCurve || !IsValid(SpeedMultiplierCurve))
	{
		CurveArea = 1.f;
		return;
	}
	
	constexpr float SampleStep = 0.01f;
	float Area = 0.f;
	for (float t = 0.f; t < 1.f; t += SampleStep)
		Area += SpeedMultiplierCurve->GetFloatValue(t) * SampleStep;

	if (Area <= KINDA_SMALL_NUMBER)
		Area = 1.f;

	CurveArea = Area;
}

void AProjectileBase::UpdateProjectileMovementComponent() const
{
	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
}

void AProjectileBase::UpdateProjectileSpeed() const
{
	if (!bUseSpeedCurve || !IsValid(SpeedMultiplierCurve))
		return;

	const float Time = CurrentLifeSpan / LifeSpan;
	const float SpeedMultiplier = SpeedMultiplierCurve->GetFloatValue(Time);
	const FVector NewDirection = GetActorForwardVector();
	SetVelocity(InitialSpeed * SpeedMultiplier * NewDirection);
}

void AProjectileBase::ProcessProjectileLifeSpan(const float DeltaSeconds)
{
	CurrentLifeSpan += DeltaSeconds;

	if (CurrentLifeSpan >= LifeSpan)
		EndProjectileLifeSpan();
}

void AProjectileBase::EndProjectileLifeSpan()
{
	UE_LOG(LogShooter, Log, TEXT("ProjectileBase::EndProjectileLifeSpan: Projectile lifespan ended. Disposing projectile."));
	OnProjectileLifeSpanEnd();
	DisposeProjectile();
}

void AProjectileBase::NotifyProjectileHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	OnProjectileHit(ImpactResult, ImpactVelocity);
	ReceiveOnProjectileHit(ImpactResult, ImpactVelocity);
	OnProjectileHitEvent.Broadcast(ImpactResult);
}
