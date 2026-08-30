// Copyright VUEDK, Inc. All Rights Reserved.

#include "LoadingScreenSubsystem.h"
#include "GameMapsSettings.h"
#include "LoadingScreenSystem.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "Utility/LoadingScreenUtility.h"

void ULoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Settings = GetDefault<ULoadingScreenSystemSettings>();
	if (!IsValid(Settings))
	{
		UE_LOG(LogLoadingScreenSystem, Error, TEXT("ULoadingScreenSubsystem::Initialize: LoadingScreenSystemSettings is not valid."));
		return;
	}
	
	ULoadingScreenUtility::Init(this);
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ULoadingScreenSubsystem::OnPreLoadMapWithContext);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ULoadingScreenSubsystem::OnPostLoadMapWithWorld);
}

void ULoadingScreenSubsystem::Deinitialize()
{
	Super::Deinitialize();

	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

void ULoadingScreenSubsystem::SetupLoadingScreen(const ULoadingScreenData* LoadingScreenData, const bool bAutoResetToDefault)
{
	if (!Check())
		return;
	
	if (!IsValid(LoadingScreenData))
	{
		UE_LOG(LogLoadingScreenSystem, Warning, TEXT("ULoadingScreenSubsystem::SetupLoadingScreen: LoadingScreenData is not valid."));
		return;
	}
	
	const TSoftClassPtr<UUserWidget> LoadingScreenSoftClass = LoadingScreenData->LoadingScreenWidgetClass;
	const TSubclassOf<UUserWidget> LoadingScreenWidgetClass = LoadingScreenSoftClass.LoadSynchronous();
	UUserWidget* LoadingScreenWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenWidgetClass);

	if (IsValid(LoadingScreenWidget))
		LoadingScreenAttributes.WidgetLoadingScreen = LoadingScreenWidget->TakeWidget();
	else
		LoadingScreenAttributes.WidgetLoadingScreen = nullptr;

	bIsPendingResetToDefault = bAutoResetToDefault;
	LoadingScreenAttributes.MoviePaths = LoadingScreenData->MoviePaths;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = LoadingScreenData->MinimumLoadingScreenDisplayTime;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = LoadingScreenData->bAutoCompleteWhenLoadingCompletes;
	LoadingScreenAttributes.bAllowInEarlyStartup = LoadingScreenData->bAllowInEarlyStartup;
	LoadingScreenAttributes.PlaybackType = LoadingScreenData->PlaybackType;
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void ULoadingScreenSubsystem::ResetToDefaultLoadingScreen()
{
	const ULoadingScreenData* DefaultLoadingScreenData = Settings->DefaultLoadingScreen.LoadSynchronous();
	SetupLoadingScreen(DefaultLoadingScreenData, false);
}

void ULoadingScreenSubsystem::ClearLoadingScreen(const bool bAutoResetToDefault)
{
	GetMoviePlayer()->SetupLoadingScreen(FLoadingScreenAttributes());
	bIsPendingResetToDefault = bAutoResetToDefault;
}

void ULoadingScreenSubsystem::OnPreLoadMapWithContext(const FWorldContext& WorldContext, const FString& MapName)
{
	const FString DefaultMapName = UGameMapsSettings::GetGameDefaultMap();
	if (Settings->bExcludeStartingMap && MapName == DefaultMapName)
		return;

	if (Settings->bExcludeStartingMapOnStartup && !bHasEverLoaded && MapName == DefaultMapName)
		return;
	
	bHasEverLoaded = true;
	ResetToDefaultLoadingScreen(); // Auto reset to default loading screen before loading a new map
}

void ULoadingScreenSubsystem::OnPostLoadMapWithWorld(UWorld* World)
{
	if (!bIsPendingResetToDefault)
		return;

	ResetToDefaultLoadingScreen();
	bIsPendingResetToDefault = false;
}

bool ULoadingScreenSubsystem::Check() const
{
	return IsValid(Settings);
}
