// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Data/WeaponData.h"
#include "GameFramework/Actor.h"
#include "Montages/Data/WeaponMontageData.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponAttackSuccess
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponAttackFail
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnEndWeaponAttack
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponEquipped
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnWeaponUnequipped
);

UCLASS(Abstract, Blueprintable, BlueprintType)
class WEAPONSYSTEM_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponAttackSuccess OnWeaponAttackSuccessEvent;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponAttackFail OnWeaponAttackFailEvent;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnEndWeaponAttack OnEndWeaponAttackEvent;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponEquipped OnWeaponEquippedEvent;
	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnWeaponUnequipped OnWeaponUnequippedEvent;

protected:
	// -- Data --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	FWeaponData WeaponData;

	// -- Montages --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Montages")
	bool bUseTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Montages", meta = (ToolTip = "Used to find the anim instance of the owner's mesh if it is not a character"))
	FName AnimInstanceMeshTag = TEXT("AnimInstanceMesh"); // Use a tag to be more flexible

	// -- Mesh --
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* WeaponMeshRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* WeaponMesh;

private:
	UPROPERTY()
	UObject* Payload;
	UPROPERTY()
	TMap<UAnimMontage*, FWeaponMontageData> PlayingMontages;
	bool bIsEquipped = false;
	bool bIsWeaponAttacking = false;
	UPROPERTY()
	UAnimInstance* OwnerAnimInstance;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UArrowComponent* ForwardArrowComponent;
	UPROPERTY()
	UArrowComponent* RightArrowComponent;
	UPROPERTY()
	UArrowComponent* UpArrowComponent;
#endif

public:
	AWeaponBase();

	/**
	 * @brief Initialize the weapon with an owner and optional payload.
	 * @param InOwner The pawn that owns this weapon.
	 * @param InPayload Optional payload object to associate with the weapon.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Init(APawn* InOwner, UObject* InPayload = nullptr);

	/**
	 * @brief Sets the weapon in the equipped state.
	 */
	UFUNCTION(BlueprintCallable)
	void Equip();

	/**
	 * @brief Sets the weapon in the unequipped state.
	 */
	UFUNCTION(BlueprintCallable)
	void Unequip();

	/**
     * Performs a weapon attack action.
     * @return true if the attack was started successfully, false otherwise.
     */
    UFUNCTION(BlueprintCallable)
    bool WeaponAttack();

    /**
     * Ends the current weapon attack action.
     */
    UFUNCTION(BlueprintCallable)
    void EndWeaponAttack();

    /**
     * Sets the payload object associated with this weapon.
     * @param InPayload The new payload object.
     */
    UFUNCTION(BlueprintCallable)
    void SetPayload(UObject* InPayload);

    /**
     * Sets the weapon's damage value.
     * @param NewDamage The new damage value to set.
     */
    UFUNCTION(BlueprintCallable)
    virtual void SetWeaponDamage(const float NewDamage);

    /**
     * Returns the current weapon damage value.
     * @return The current damage value of the weapon.
     */
    UFUNCTION(BlueprintPure)
    float GetWeaponDamage() const;

    /**
     * Returns the payload object associated with this weapon.
     * @return The payload UObject pointer, or nullptr if not set.
     */
    UFUNCTION(BlueprintPure)
    UObject* GetPayload() const;

	/**
     * Checks if the weapon is currently equipped.
     * @return true if the weapon is equipped, false otherwise.
     */
    UFUNCTION(BlueprintPure)
    bool IsEquipped() const;

    /**
     * Checks if the weapon is currently performing an attack.
     * @return true if the weapon is attacking, false otherwise.
     */
    UFUNCTION(BlueprintPure)
    virtual bool IsWeaponAttacking() const;

    /**
     * Returns the weapon data struct containing all relevant weapon properties.
     * @return The FWeaponData struct for this weapon.
     */
    UFUNCTION(BlueprintPure)
    FWeaponData GetWeaponData() const;

    /**
     * Returns the animation instance of the weapon's owner (usually the character).
     * @return The owner's UAnimInstance pointer, or nullptr if not found.
     */
    UFUNCTION(BlueprintPure)
    UAnimInstance* GetOwnerAnimInstance() const;

    /**
     * Returns the animation instance associated with the weapon itself.
     * @return The weapon's UAnimInstance pointer, or nullptr if not found.
     */
    UFUNCTION(BlueprintPure)
    UAnimInstance* GetWeaponAnimInstance() const;

	/**
	 * Checks if the specified weapon montage is currently playing.
	 * @param WeaponMontageData - The montage data to check.
	 * @return true if the montage is playing, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsPlayingWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;

	/**
	 * Resumes the specified weapon montage if it is currently paused.
	 * @param WeaponMontageData - The montage data to resume.
	 */
	UFUNCTION(BlueprintCallable)
	void ResumeWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;
	
	/**
	 * Pauses the specified weapon montage if it is currently playing.
	 * @param WeaponMontageData - The montage data to pause.
	 */
	UFUNCTION(BlueprintCallable)
	void PauseWeaponMontage(const FWeaponMontageData& WeaponMontageData) const;
	
	/**
	 * Starts playing the specified weapon montage with given play rates.
	 * @param WeaponMontageData - The montage data to play.
	 * @param WeaponPlayRate - Play rate for the weapon montage.
	 * @param CharacterPlayRate - Play rate for the character montage.
	 */
	UFUNCTION(BlueprintCallable)
	void StartWeaponMontage(FWeaponMontageData WeaponMontageData, const float WeaponPlayRate, const float CharacterPlayRate);

	/**
	 * Starts playing the specified weapon montage with custom blend-in settings.
	 * @param WeaponMontageData - The montage data to play.
	 * @param WeaponPlayRate - Play rate for the weapon montage.
	 * @param CharacterPlayRate - Play rate for the character montage.
	 * @param WeaponBlendIn - Blend-in settings for the weapon montage.
	 * @param CharacterBlendIn - Blend-in settings for the character montage.
	 */
	UFUNCTION(BlueprintCallable)
	void StartWeaponMontageWithBlends(FWeaponMontageData WeaponMontageData, const float WeaponPlayRate, const float CharacterPlayRate, const FAlphaBlendArgs& WeaponBlendIn, const FAlphaBlendArgs& CharacterBlendIn);

	/**
	 * Stops the specified weapon montage if it is playing.
	 * @param WeaponMontageData - The montage data to stop.
	 */
	UFUNCTION(BlueprintCallable)
	void StopWeaponMontage(const FWeaponMontageData WeaponMontageData);

	/**
	 * Stops the specified weapon montage with custom blend-out settings.
	 * @param WeaponMontageData - The montage data to stop.
	 * @param WeaponBlendOut - Blend-out settings for the weapon montage.
	 * @param CharacterBlendOut - Blend-out settings for the character montage.
	 */
	UFUNCTION(BlueprintCallable)
	void StopWeaponMontageWithBlends(const FWeaponMontageData& WeaponMontageData, const FAlphaBlendArgs& WeaponBlendOut, const FAlphaBlendArgs& CharacterBlendOut);

protected:
	virtual bool NativeDeployWeaponAttack();
	
	/**
	 * Called to deploy a weapon attack. Can be overridden in Blueprint.
	 * @return true if the attack was deployed successfully, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool DeployWeaponAttack();

	/**
	 * Called when a weapon attack ends. Can be overridden in Blueprint.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnEndWeaponAttack();

	/**
	 * Called when a weapon attack is successful. Can be overridden in Blueprint.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponAttackSuccess();

	/**
	 * Called when a weapon attack fails. Can be overridden in Blueprint.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponAttackFail();

	/**
	 * Called when the weapon is equipped. Can be overridden in Blueprint.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponEquipped();

	/**
	 * Called when the weapon is unequipped. Can be overridden in Blueprint.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnWeaponUnequipped();

private:
	/**
	 * Internal function to play a montage on the given animation instance.
	 * @param AnimInstance - The animation instance to play the montage on.
	 * @param WeaponMontageData - The montage data to play.
	 * @param Montage - The animation montage asset.
	 * @param PlayRate - The rate at which to play the montage.
	 * @param bStopAll - Whether to stop all other montages before playing.
	 * @param bRegisterPlayingMontage - Whether to register the montage as playing (default: true).
	 */
	void PlayMontageInternal(UAnimInstance* AnimInstance, FWeaponMontageData& WeaponMontageData, UAnimMontage* Montage, float PlayRate, bool bStopAll, bool bRegisterPlayingMontage = true);

	/**
	 * Internal function to play a montage with custom blend-in settings.
	 * @param AnimInstance - The animation instance to play the montage on.
	 * @param WeaponMontageData - The montage data to play.
	 * @param Montage - The animation montage asset.
	 * @param PlayRate - The rate at which to play the montage.
	 * @param bStopAll - Whether to stop all other montages before playing.
	 * @param BlendIn - Blend-in settings for the montage.
	 * @param bRegisterPlayingMontage - Whether to register the montage as playing (default: true).
	 */
	void PlayMontageWithBlendInternal(UAnimInstance* AnimInstance, FWeaponMontageData& WeaponMontageData, UAnimMontage* Montage, float PlayRate, bool bStopAll, const FAlphaBlendArgs& BlendIn, bool bRegisterPlayingMontage = true);

	void SetOwnerAnimInstance();

	void AddPlayingMontage(UAnimMontage* Montage, const FWeaponMontageData& WeaponMontageData);

	void RemovePlayingMontage(const UAnimMontage* Montage);

	void SetWeaponMetaData(UAnimMontage* Montage);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);
};
