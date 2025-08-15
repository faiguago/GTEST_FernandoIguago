// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperButton.h"

#include "Minesweeper.h"
#include "MinesweeperStyle.h"
#include "SlateOptMacros.h"
#include "Components/Button.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMinesweeperButton::Construct(const FArguments& InArgs)
{
	SetVisibility(EVisibility::SelfHitTestInvisible);

	bIsClicked = false;

	bIsMine = InArgs._bIsMine;
	MinesweeperModule = InArgs._MinesweeperModule;
	Coordinates = InArgs._Coordinates;

	FSlateFontInfo const BoldTitleFont = FSlateFontInfo(
		FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"),
		24);

	ChildSlot
		[
			SNew(SBox)
				.WidthOverride(50.f)
				.HeightOverride(50.f)

				[
					SNew(SOverlay)

						+ SOverlay::Slot()
						[
							SAssignNew(Button, SButton)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								.OnClicked_Raw(this, &SMinesweeperButton::HandleClick)
						]

						+ SOverlay::Slot()
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							SAssignNew(MineCountText, STextBlock)
								.Text(FText::FromString(TEXT("X")))
								.Font(BoldTitleFont)
								.Visibility(EVisibility::Hidden)
						]
				]
		];

	if (bIsMine)
	{
		MineCountText.Get()->SetColorAndOpacity(FLinearColor::Red);
		MineCountText.Get()->SetText(FText::FromString(FString(TEXT("X"))));
	}
}

void SMinesweeperButton::RevealMine() const
{
	if (bIsMine)
	{
		MineCountText.Get()->SetColorAndOpacity(FLinearColor::Red);
		MineCountText.Get()->SetVisibility(EVisibility::HitTestInvisible);
	}
}

void SMinesweeperButton::RevealAdjacency() const
{
	if (MinesweeperModule)
	{
		MinesweeperModule->RevealAdjacentButtons(Coordinates);
	}
}

void SMinesweeperButton::SetClicked()
{
	Button->SetEnabled(false);
	Button.Get()->SetVisibility(EVisibility::Hidden);
	
	bIsClicked = true;

	if (MinesweeperModule && !bIsMine)
	{
		MinesweeperModule->BlockClicked();
	}
}

void SMinesweeperButton::SetNeighborMineCount(int const Count) const
{
	if (Count < 1) return;

	if (Count == 1)
	{
		MineCountText.Get()->SetText(FText::FromString(FString::FromInt(1)));
		MineCountText.Get()->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 2)
	{
		MineCountText.Get()->SetText(FText::FromString(FString::FromInt(2)));
		MineCountText.Get()->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 3)
	{
		MineCountText.Get()->SetText(FText::FromString(FString::FromInt(3)));
		MineCountText.Get()->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 4)
	{
		MineCountText.Get()->SetText(FText::FromString(FString::FromInt(4)));
		MineCountText.Get()->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 5)
	{
		MineCountText.Get()->SetText(FText::FromString(FString::FromInt(5)));
		MineCountText.Get()->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 6)
	{
		MineCountText.Get()->SetText(FText::FromString(FString::FromInt(6)));
		MineCountText.Get()->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 7)
	{
		MineCountText.Get()->SetText(FText::FromString(FString::FromInt(7)));
		MineCountText.Get()->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count == 8)
	{
		MineCountText.Get()->SetText(FText::FromString(FString::FromInt(8)));
		MineCountText.Get()->SetVisibility(EVisibility::HitTestInvisible);
	}

	else if (Count > 8)
	{
		MineCountText.Get()->SetText(FText::FromString(FString::FromInt(9)));
		MineCountText.Get()->SetVisibility(EVisibility::HitTestInvisible);
	}
}

bool SMinesweeperButton::IsAdjacent(FVector2D const InCoordinates)
{
	if (Coordinates.X == InCoordinates.X && (Coordinates.Y == InCoordinates.Y + 1 || Coordinates.Y == InCoordinates.Y - 1) ||
		Coordinates.Y == InCoordinates.Y && (Coordinates.X == InCoordinates.X + 1 || Coordinates.X == InCoordinates.X - 1) ||
		Coordinates.X == InCoordinates.X - 1 && Coordinates.Y == InCoordinates.Y - 1 ||
		Coordinates.X == InCoordinates.X - 1 && Coordinates.Y == InCoordinates.Y + 1 ||
		Coordinates.X == InCoordinates.X + 1 && Coordinates.Y == InCoordinates.Y + 1 ||
		Coordinates.X == InCoordinates.X + 1 && Coordinates.Y == InCoordinates.Y - 1)
	{
		return true;
	}

	return false;
}

FReply SMinesweeperButton::HandleClick()
{
	SetClicked();

	if (MinesweeperModule)
	{
		if (bIsMine)
		{
			RevealMine();
			MinesweeperModule->MineClicked();
			return FReply::Handled();
		}

		auto const Count = MinesweeperModule->CountAdjacentMines(Coordinates);
		if (Count == 0)
		{
			RevealAdjacency();
		}
		else
		{
			SetNeighborMineCount(Count);
		}
	}

	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION