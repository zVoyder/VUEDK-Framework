// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MoviePlayer.h"
#include "Engine/DataAsset.h"
#include "LoadingScreenData.generated.h"

UCLASS(BlueprintType)
class LOADINGSCREENSYSTEM_API ULoadingScreenData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UUserWidget> LoadingScreenWidgetClass;
	/** The movie paths local to the game's Content/Movies/ directory we will play. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FString> MoviePaths;
	/** The minimum time that a loading screen should be opened for. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MinimumLoadingScreenDisplayTime;
	/** If true, the loading screen will disappear as soon as all movies are played and loading is done. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bAutoCompleteWhenLoadingCompletes;
	/** If true loading screens here cannot have any uobjects of any kind or use any engine features at all. This will start the movies very early as a result on platforms that support it */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bAllowInEarlyStartup;
	/** Should we just play back, loop, etc.  NOTE: if the playback type is MT_LoopLast, then bAutoCompleteWhenLoadingCompletes will be togged on when the last movie is hit*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EMoviePlaybackType> PlaybackType;

	ULoadingScreenData() : MinimumLoadingScreenDisplayTime(-1.0f),
	                       bAutoCompleteWhenLoadingCompletes(true),
	                       bAllowInEarlyStartup(false),
	                       PlaybackType(EMoviePlaybackType::MT_Looped)
	{
	}
};
