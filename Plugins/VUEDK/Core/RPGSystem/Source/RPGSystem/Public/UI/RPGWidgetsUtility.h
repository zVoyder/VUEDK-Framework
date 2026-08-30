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
	/**
	 * Draws a custom polygon using the specified points and brush.
	 * @param Context The paint context to draw on.
	 * @param Points The vertices of the polygon.
	 * @param Brush The brush asset to use for filling the polygon.
	 * @param ZOrder The Z-order for drawing.
	 * @param Tint The color tint to apply to the polygon.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|WidgetsUtility")
	static void DrawCustomPolygon(UPARAM(ref) FPaintContext& Context, TArray<FVector2D> Points, USlateBrushAsset* Brush, const int32 ZOrder = 0, const FLinearColor Tint = FLinearColor::White);

	/**
	 * Draws a regular polygon with the specified number of sides and radius.
	 * @param Context The paint context to draw on.
	 * @param NumberOfSides The number of sides of the polygon.
	 * @param Radius The radius of the polygon.
	 * @param Brush The brush asset to use for filling the polygon.
	 * @param OutCenter Output parameter for the center of the polygon.
	 * @param OutPoints Output parameter for the vertices of the polygon.
	 * @param ZOrder The Z-order for drawing.
	 * @param Tint The color tint to apply to the polygon.
	 * @param BorderBrush Optional brush for the border.
	 * @param BorderTint The color tint for the border.
	 * @param BorderThickness The thickness of the border.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|WidgetsUtility")
	static void DrawPolygon(UPARAM(ref) FPaintContext& Context, int32 NumberOfSides, FVector2D Radius, USlateBrushAsset* Brush, FVector2D& OutCenter, TArray<FVector2D>& OutPoints, const int32 ZOrder = 0, const FLinearColor Tint = FLinearColor::White, USlateBrushAsset* BorderBrush = nullptr, const FLinearColor BorderTint = FLinearColor::White, float BorderThickness = 1.0f);

	/**
	 * Draws a polygon with progress values for each side, allowing for partial filling.
	 * @param Context The paint context to draw on.
	 * @param NumberOfSides The number of sides of the polygon.
	 * @param Radius The radius of the polygon.
	 * @param Progresses The progress values for each side (0.0 to 1.0).
	 * @param Brush The brush asset to use for filling the polygon.
	 * @param OutCenter Output parameter for the center of the polygon.
	 * @param OutPoints Output parameter for the vertices of the polygon.
	 * @param ZOrder The Z-order for drawing.
	 * @param Tint The color tint to apply to the polygon.
	 * @param BorderBrush Optional brush for the border.
	 * @param BorderTint The color tint for the border.
	 * @param BorderThickness The thickness of the border.
	 * @param MinShrink The minimum shrink factor for the polygon sides.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|WidgetsUtility")
	static void DrawPolygonWithProgress(UPARAM(ref) FPaintContext& Context, int32 NumberOfSides, FVector2D Radius, TArray<float> Progresses, USlateBrushAsset* Brush, FVector2D& OutCenter, TArray<FVector2D>& OutPoints, const int32 ZOrder = 0, const FLinearColor Tint = FLinearColor::White, USlateBrushAsset* BorderBrush = nullptr, const FLinearColor BorderTint = FLinearColor::White, float BorderThickness = 1.0f, float MinShrink = .2f);

	/**
	 * Draws labels around the specified polygon points.
	 * @param Context The paint context to draw on.
	 * @param Points The vertices of the polygon.
	 * @param Center The center of the polygon.
	 * @param Labels The labels to draw around the polygon.
	 * @param ZOrder The Z-order for drawing.
	 * @param FontInfo The font information for the labels.
	 * @param TextColor The color of the text.
	 * @param TextOffset The offset distance from the polygon for the labels.
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGSystem|WidgetsUtility")
	static void DrawLabelsAroundPolygon(UPARAM(ref) FPaintContext& Context, const TArray<FVector2D>& Points, const FVector2D& Center, const TArray<FText>& Labels, const int32 ZOrder = 0, const FSlateFontInfo FontInfo = FSlateFontInfo(), const FLinearColor TextColor = FLinearColor::White, float TextOffset = 10.0f);
};
