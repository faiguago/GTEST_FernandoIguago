// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleManager.h"

class SMinesweeperButton;
class FToolBarBuilder;
class FMenuBuilder;
class SUniformGridPanel;

class FMinesweeperModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	void RevealAdjacentButtons(FVector2D const InVector2D);
	int CountAdjacentMines(FVector2D const InVector2D);
	void MineClicked();
	void BlockClicked();
	bool GameWon() const { return RemainingBlocks <= 0; }	

private:
	void RegisterMenus();

	FReply GenerateMinesweeperGrid();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	TSharedPtr<class FUICommandList> PluginCommands;

	/** Slate Pointers */
	TSharedPtr<SEditableTextBox> WidthEntryTextBox;
	TSharedPtr<SEditableTextBox> HeightEntryTextBox;
	TSharedPtr<SEditableTextBox> TotalMineCountTextBox;
	TSharedPtr<SUniformGridPanel> MinesweeperGridPanel;
	TSharedPtr<STextBlock> GameOverText;
	TSharedPtr<SScrollBar> VerticalScrollBar;
	TSharedPtr<SScrollBar> HorizontalScrollbar;

	int GridHeight{};
	int GridWidth{};
	int TotalMines{};
	int RemainingBlocks{};

	TArray<TSharedPtr<SMinesweeperButton>> MineButtons;
};
