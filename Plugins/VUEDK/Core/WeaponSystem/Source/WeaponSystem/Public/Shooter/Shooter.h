// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShootBarrel.h"
#include "Behaviours/ShooterBehaviourBase.h"
#include "Components/ActorComponent.h"
#include "Managers/Magazine/MagazinesManager.h"
#include "Shooter.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UShooter : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourEnabled OnBehaviourEnabled;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourDisabled OnBehaviourDisabled;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourShootSuccess OnBehaviourShootSuccess;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourShootFail OnBehaviourShootFail;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnEndShootSequence OnBehaviourShootSequenceEnded;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourAmmoChanged OnBehaviourAmmoChanged;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourRefilled OnBehaviourRefilled;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourFull OnBehaviourFull;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnBehaviourEmpty OnBehaviourEmpty;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMagazineRefilled OnMagazineRefilled;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMagazineAmmoChanged OnMagazineAmmoChanged;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMagazineFull OnMagazineFull;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMagazineEmpty OnMagazineEmpty;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "If true, the shooter auto-initializes on BeginPlay: it finds all child ShootBarrels and assigns them to behaviours using the child index."))
	bool bAutoInitialize = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "If true, allows multiple shooter behaviours to shoot at the same time."))
	bool bCanShootInParallel = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TArray<UShooterBehaviourBase*> ShooterBehaviours;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	UMagazinesManager* MagazinesManager;

public:
	UShooter();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void Init(APawn* InOwner);

	UFUNCTION(BlueprintCallable)
	void SetupShootBarrel(UShootBarrel* InShootBarrel, const int32 BehaviourIndex = 0) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool Shoot(const int32 BehaviourIndex = 0) const;

	UFUNCTION(BlueprintCallable)
	void ShootAll() const;

	UFUNCTION(BlueprintCallable)
	void EndShootSequence(const int32 BehaviourIndex = 0) const;
	
	UFUNCTION(BlueprintCallable)
	void EnableAllBehaviours() const;

	UFUNCTION(BlueprintCallable)
	void DisableAllBehaviours() const;

	UFUNCTION(BlueprintPure)
	UShooterBehaviourBase* GetShooterBehaviour(const int32 BehaviourIndex = 0) const;
	
	UFUNCTION(BlueprintPure)
	UMagazine* GetMagazine(const int32 MagazineIndex = 0) const;

	UFUNCTION(BlueprintPure)
	bool IsAnyBehaviourShooting(const int32 IgnoreBehaviourIndex = -1) const;

protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void AutoInit();
	
	void BindEvents() const;

	void UnbindEvents() const;
	
	void TickBehaviours(const float DeltaTime) const;

	UFUNCTION()
	void CallBehaviourEnabledEvent(UShooterBehaviourBase* Behaviour);
	
	UFUNCTION()
	void CallBehaviourDisabledEvent(UShooterBehaviourBase* Behaviour);
	
	UFUNCTION()
	void CallBehaviourShootSuccessEvent(UShooterBehaviourBase* Behaviour, UShootBarrel* ShootBarrel, int32 ShotIndex);
	
	UFUNCTION()
	void CallBehaviourShootFailEvent(UShooterBehaviourBase* Behaviour, EShootFailReason FailReason);
	
	UFUNCTION()
	void CallBehaviourShootSequenceEndedEvent(UShooterBehaviourBase* Behaviour);
	
	UFUNCTION()
	void CallBehaviourAmmoChangedEvent(UShooterBehaviourBase* Behaviour, UMagazine* Magazine, int32 CurrentAmmo, int32 MagSize);
	
	UFUNCTION()
	void CallBehaviourRefilledEvent(UShooterBehaviourBase* Behaviour, UMagazine* Magazine, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo);
	
	UFUNCTION()
	void CallBehaviourFullEvent(UShooterBehaviourBase* Behaviour, UMagazine* Magazine);
	
	UFUNCTION()
	void CallBehaviourEmptyEvent(UShooterBehaviourBase* Behaviour, UMagazine* Magazine);

	UFUNCTION()
	void CallMagazineRefilledEvent(const UObject* Instigator, const UMagazine* Magazine, int32 CurrentAmmo, int32 RefilledAmmo, int32 RemainingAmmo);
	
	UFUNCTION()
	void CallMagazineAmmoChangedEvent(const UObject* Instigator, const UMagazine* Magazine, int32 CurrentAmmo, int32 MagSize);
	
	UFUNCTION()
	void CallMagazineFullEvent(const UObject* Instigator, const UMagazine* Magazine);
	
	UFUNCTION()
	void CallMagazineEmptyEvent(const UObject* Instigator, const UMagazine* Magazine);
};
