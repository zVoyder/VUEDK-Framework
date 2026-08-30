// Copyright VUEDK, Inc. All Rights Reserved.

#include "CustomSettingsSubsystem.h"
#include "Utility/CustomSettingsUtility.h"

void UCustomSettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UCustomGameUserSettings* UserSettings = UCustomSettingsUtility::GetCustomUserSettings();
	if (!IsValid(UserSettings))
		return;
	
	UserSettings->OnCustomSettingsApplied.AddUniqueDynamic(this, &UCustomSettingsSubsystem::OnCustomSettingsApplied);
	UserSettings->OnCustomOptionChanged.AddUniqueDynamic(this, &UCustomSettingsSubsystem::OnCustomOptionChanged);
}

void UCustomSettingsSubsystem::Deinitialize()
{
	Super::Deinitialize();
	UCustomGameUserSettings* UserSettings = UCustomSettingsUtility::GetCustomUserSettings();
	if (!IsValid(UserSettings))
		return;
	
	UserSettings->OnCustomSettingsApplied.RemoveDynamic(this, &UCustomSettingsSubsystem::OnCustomSettingsApplied);
	UserSettings->OnCustomOptionChanged.RemoveDynamic(this, &UCustomSettingsSubsystem::OnCustomOptionChanged);
}

void UCustomSettingsSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CreateCustomSettingObjects();
	OnCustomSettingsApplied();
}

void UCustomSettingsSubsystem::CreateCustomSettingObjects()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
		return;
	
	const UCSSSettings* Settings = GetDefault<UCSSSettings>();
	if (!IsValid(Settings))
		return;

	CustomSettingObjectsMap.Empty(); // Safeguard
	for (auto Option : Settings->CustomSettingsMap)
	{
		FGameplayTag OptionTag = Option.Key;
		FCustomOptionData OptionData = Option.Value;
		TSubclassOf<UCustomOptionObject> OptionClass = OptionData.CustomOptionClass.LoadSynchronous();
		if (!IsValid(OptionClass))
			continue;

		UCustomOptionObject* OptionObject = NewObject<UCustomOptionObject>(this, OptionClass);
		if (IsValid(OptionObject))
		{
			OptionObject->Init(World);
			CustomSettingObjectsMap.Add(Option.Key, OptionObject);
		}
	}
}

void UCustomSettingsSubsystem::OnCustomSettingsApplied()
{
	const UCustomGameUserSettings* UserSettings = UCustomSettingsUtility::GetCustomUserSettings();
	if (!IsValid(UserSettings))
		return;
	
	for (const auto& SettingPair : CustomSettingObjectsMap)
	{
		const FGameplayTag& OptionTag = SettingPair.Key;
		UCustomOptionObject* OptionObject = SettingPair.Value;
		if (!IsValid(OptionObject))
			continue;

		float OptionValue = 0.f;
		if (UserSettings->TryGetCustomOption(OptionTag, OptionValue))
			OptionObject->OnCustomSettingsApplied(OptionValue);
	}
}

void UCustomSettingsSubsystem::OnCustomOptionChanged(const FGameplayTag& Tag, float Value)
{
	if (!CustomSettingObjectsMap.Contains(Tag))
		return;
	
	UCustomOptionObject* OptionObject = CustomSettingObjectsMap[Tag];
	if (!IsValid(OptionObject))
		return;

	OptionObject->OnCustomOptionChanged(Value);
}
