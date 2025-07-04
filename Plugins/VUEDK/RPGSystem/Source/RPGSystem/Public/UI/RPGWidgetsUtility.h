// Copyright VUEDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Slate/SlateBrushAsset.h"
#include "RPGWidgetsUtility.generated.h"

UCLASS()
class RPGSYSTEM_API URPGWidgetsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|WidgetsUtility")
	static void DrawCustomPolygon(UPARAM(ref) FPaintContext& Context, TArray<FVector2D> Points, USlateBrushAsset* Brush, const int32 ZOrder = 0, const FLinearColor Tint = FLinearColor::White);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|WidgetsUtility")
	static void DrawPolygon(UPARAM(ref) FPaintContext& Context, int32 NumberOfSides, FVector2D Radius, USlateBrushAsset* Brush, FVector2D& OutCenter, TArray<FVector2D>& OutPoints, const int32 ZOrder = 0, const FLinearColor Tint = FLinearColor::White, USlateBrushAsset* BorderBrush = nullptr, const FLinearColor BorderTint = FLinearColor::White, float BorderThickness = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|WidgetsUtility")
	static void DrawPolygonWithProgress(UPARAM(ref) FPaintContext& Context, int32 NumberOfSides, FVector2D Radius, TArray<float> Progresses, USlateBrushAsset* Brush, FVector2D& OutCenter, TArray<FVector2D>& OutPoints, const int32 ZOrder = 0, const FLinearColor Tint = FLinearColor::White, USlateBrushAsset* BorderBrush = nullptr, const FLinearColor BorderTint = FLinearColor::White, float BorderThickness = 1.0f, float MinShrink = .2f);

	UFUNCTION(BlueprintCallable, Category = "VUEDK|RPGSystem|WidgetsUtility")
	static void DrawLabelsAroundPolygon(UPARAM(ref) FPaintContext& Context, const TArray<FVector2D>& Points, const FVector2D& Center, const TArray<FText>& Labels, const int32 ZOrder = 0, const FSlateFontInfo FontInfo = FSlateFontInfo(), const FLinearColor TextColor = FLinearColor::White, float TextOffset = 10.0f);
};
