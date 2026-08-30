// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EQualityType : uint8
{
	ShadingQuality UMETA(DisplayName = "Shading Quality"),
	TextureQuality UMETA(DisplayName = "Texture Quality"),
	FoliageQuality UMETA(DisplayName = "Foliage Quality"),
    VisualEffectsQuality UMETA(DisplayName = "Visual Effects Quality"),
    AntiAliasingQuality UMETA(DisplayName = "Anti-Aliasing Quality"),
    ViewDistanceQuality UMETA(DisplayName = "Distance Quality"),
    GlobalIlluminationQuality UMETA(DisplayName = "Global Illumination Quality"),
    ShadowQuality UMETA(DisplayName = "Shadow Quality"),
    AudioQuality UMETA(DisplayName = "Audio Quality"),
    PostProcessQuality UMETA(DisplayName = "Post-Process Quality"),
    ReflectionQuality UMETA(DisplayName = "Reflection Quality")
};
