// Copyright VUEDK, Inc. All Rights Reserved.

#include "UI/RPGWidgetsUtility.h"
#include "Fonts/FontMeasure.h"

void URPGWidgetsUtility::DrawCustomPolygon(FPaintContext& Context, TArray<FVector2D> Points, USlateBrushAsset* Brush, const int32 ZOrder, const FLinearColor Tint)
{
	if (Points.Num() < 3 || !Brush)
		return;

	TArray<FSlateVertex> Vertices;
	TArray<SlateIndex> Indices;

	FVector2D Center(0.f, 0.f);
	for (const FVector2D& Point : Points)
		Center += Point;
	Center /= Points.Num();

	const FSlateRenderTransform& RenderTransform = Context.AllottedGeometry.GetAccumulatedRenderTransform();
	const FColor FillColor = Tint.ToFColor(true);
	const int32 CenterIndex = Vertices.Num();

	const FVector2f UVCenter(0.5f, 0.5f);
	Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(
		RenderTransform,
		FVector2f(Center),
		UVCenter,
		FillColor
	));

	for (int32 i = 0; i < Points.Num(); ++i)
	{
		const FVector2D& A = Points[i];
		const FVector2D& B = Points[(i + 1) % Points.Num()];

		FVector2D DirA = (A - Center).GetSafeNormal();
		FVector2D DirB = (B - Center).GetSafeNormal();

		const FVector2f UVA = UVCenter + FVector2f(DirA) * 0.5f;
		const FVector2f UVB = UVCenter + FVector2f(DirB) * 0.5f;

		const int32 IndexA = Vertices.Num();
		const int32 IndexB = Vertices.Num() + 1;

		Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(
			RenderTransform, FVector2f(A), UVA, FillColor
		));
		Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(
			RenderTransform, FVector2f(B), UVB, FillColor
		));

		Indices.Add(CenterIndex);
		Indices.Add(IndexA);
		Indices.Add(IndexB);
	}

	const FSlateBrush& SlateBrush = Brush->Brush;
	const FSlateResourceHandle& ResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(SlateBrush);

	FSlateDrawElement::MakeCustomVerts(
		Context.OutDrawElements,
		Context.MaxLayer + ZOrder,
		ResourceHandle,
		Vertices,
		Indices,
		nullptr,
		0,
		0
	);
}

void URPGWidgetsUtility::DrawPolygon(FPaintContext& Context, int32 NumberOfSides, FVector2D Radius, USlateBrushAsset* Brush, FVector2D& OutCenter, TArray<FVector2D>& OutPoints, const int32 ZOrder, const FLinearColor Tint, USlateBrushAsset* BorderBrush, const FLinearColor BorderTint, float BorderThickness)
{
	if (NumberOfSides < 3 || !Brush)
		return;

	TArray<FVector2D> Points;
	const FVector2D WidgetSize = Context.AllottedGeometry.GetLocalSize();
	const FVector2D Center = WidgetSize * 0.5f;

	const float AngleStep = 2 * PI / NumberOfSides;

	for (int32 i = 0; i < NumberOfSides; ++i)
	{
		const float Angle = i * AngleStep - PI / 2.0f; // offset per partire dall'alto
		const float X = Center.X + Radius.X * FMath::Cos(Angle);
		const float Y = Center.Y + Radius.Y * FMath::Sin(Angle);
		Points.Add(FVector2D(X, Y));
	}

	TArray<FVector2D> EnlargedPoints;
	for (const FVector2D& P : Points)
	{
		FVector2D Dir = (P - Center).GetSafeNormal();
		EnlargedPoints.Add(P + Dir * BorderThickness);
	}

	OutCenter = Center;
	OutPoints = Points;
	DrawCustomPolygon(Context, EnlargedPoints, BorderBrush, ZOrder, BorderTint);
	DrawCustomPolygon(Context, Points, Brush, ZOrder + 1, Tint);
}

void URPGWidgetsUtility::DrawPolygonWithProgress(FPaintContext& Context, int32 NumberOfSides, FVector2D Radius, TArray<float> Progresses, USlateBrushAsset* Brush, FVector2D& OutCenter, TArray<FVector2D>& OutPoints, const int32 ZOrder, const FLinearColor Tint, USlateBrushAsset* BorderBrush, const FLinearColor BorderTint, float BorderThickness, float MinShrink)
{
	if (NumberOfSides < 3 || !Brush)
		return;

	if (Progresses.Num() != NumberOfSides)
		Progresses.Init(0.0f, NumberOfSides);

	TArray<FVector2D> Points;
	const FVector2D WidgetSize = Context.AllottedGeometry.GetLocalSize();
	const FVector2D Center = WidgetSize * 0.5f;

	const float AngleStep = 2 * PI / NumberOfSides;

	for (int32 i = 0; i < NumberOfSides; ++i)
	{
		const float Angle = i * AngleStep - PI / 2.0f;
		FVector2D OrigPos = FVector2D(Center.X + Radius.X * FMath::Cos(Angle),
		                              Center.Y + Radius.Y * FMath::Sin(Angle));

		float VertexProgress = FMath::Clamp(Progresses[i], 0.0f, 1.0f);
		VertexProgress = FMath::Clamp(VertexProgress, 0.0f, 1.0f);

		FVector2D Dir = (OrigPos - Center).GetSafeNormal();
		float DistOrig = FVector2D::Distance(OrigPos, Center);
		float MinDist = DistOrig * MinShrink;
		const float DistFinal = FMath::Lerp(MinDist, DistOrig, VertexProgress);

		FVector2D MovedPos = Center + Dir * DistFinal;
		Points.Add(MovedPos);
	}

	TArray<FVector2D> EnlargedPoints;
	for (const FVector2D& P : Points)
	{
		FVector2D Dir = (P - Center).GetSafeNormal();
		EnlargedPoints.Add(P + Dir * BorderThickness);
	}

	OutCenter = Center;
	OutPoints = Points;
	DrawCustomPolygon(Context, EnlargedPoints, BorderBrush, ZOrder, BorderTint);
	DrawCustomPolygon(Context, Points, Brush, ZOrder + 1, Tint);
}

void URPGWidgetsUtility::DrawLabelsAroundPolygon(FPaintContext& Context, const TArray<FVector2D>& Points, const FVector2D& Center, const TArray<FText>& Labels, const int32 ZOrder, const FSlateFontInfo FontInfo, const FLinearColor TextColor, float TextOffset)
{
	const TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();

	for (int32 i = 0; i < Points.Num(); ++i)
	{
		if (i >= Labels.Num() || Labels[i].IsEmpty())
			continue;

		const FVector2D& A = Points[i];
		FVector2D Dir = (A - Center).GetSafeNormal();
		FVector2D TextSize = FontMeasureService->Measure(Labels[i], FontInfo);

		FVector2D BaseTextPos = A + Dir * TextOffset;
		FVector2D Anchor = FVector2D::ZeroVector;

		if (FMath::Abs(Dir.X) > FMath::Abs(Dir.Y))
		{
			Anchor.X = (Dir.X > 0) ? 0.0f : 1.0f;
			Anchor.Y = 0.5f;
		}
		else
		{
			Anchor.X = 0.5f;
			Anchor.Y = (Dir.Y > 0) ? 0.0f : 1.0f;
		}

		FVector2D FinalTextPos = BaseTextPos - TextSize * Anchor;

		FSlateDrawElement::MakeText(
			Context.OutDrawElements,
			Context.MaxLayer + ZOrder,
			Context.AllottedGeometry.ToOffsetPaintGeometry(FinalTextPos),
			Labels[i],
			FontInfo,
			ESlateDrawEffect::None,
			TextColor
		);
	}
}
