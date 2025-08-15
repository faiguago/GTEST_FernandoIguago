// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Widgets/SCompoundWidget.h"

class FMinesweeperModule;

/**
*	Minesweeper Button Widget
*/

class SMinesweeperButton final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperButton)
		{
		}

	SLATE_ARGUMENT(bool, bIsMine)
	SLATE_ARGUMENT(FVector2D, Coordinates)
	SLATE_ARGUMENT(FMinesweeperModule*, MinesweeperModule)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void RevealMine() const;
	void RevealAdjacency() const;
	void SetClicked();
	void SetNeighborMineCount(int const Count) const;

	FVector2D GetCoordinates() const { return Coordinates; }

	bool IsAdjacent(FVector2D const InCoordinates);
	void SetMine() { bIsMine = true; }
	bool HasMine() const { return bIsMine; }
	bool IsClicked() const { return bIsClicked; }

private:
	FReply HandleClick();

	TSharedPtr<SButton> Button;
	TSharedPtr<STextBlock> MineCountText;

	FMinesweeperModule* MinesweeperModule { nullptr };

	uint8 bIsMine : 1;
	uint8 bIsClicked : 1;

	FVector2D Coordinates{};
};
