// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

// Save System
#define TIME_PLAYED_TIMER_RATE 0.1f

// Save Files
#define SAVE_EXTENSION TEXT(".sav")
#define SAVES_DIRECTORY TEXT("saves/")
#define SAVES_DIRECTORY_FULLPATH FPaths::Combine(FPaths::ProjectSavedDir(), "SaveGames/", SAVES_DIRECTORY)
#define SAVE_SLOT_NAME TEXT("save_")
#define AUTO_SAVE_SLOT_NAME TEXT("auto_")
#define SHARED_SAVE_FILE_NAME TEXT("sharedsave")

// Meta Files
#define META_DIRECTORY FPaths::Combine(SAVES_DIRECTORY, TEXT(".meta/"))
#define META_DIRECTORY_FULLPATH FPaths::Combine(FPaths::ProjectSavedDir(), META_DIRECTORY)
#define META_INFOS_PATH FPaths::Combine(META_DIRECTORY, TEXT("infos"))
