// Copyright Epic Games, Inc. All Rights Reserved.

#include "Minesweeper.h"
#include "MinesweeperStyle.h"
#include "MinesweeperCommands.h"
#include "LevelEditor.h"
#include "MinesweeperButton.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Components/Button.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"

static const FName MinesweeperTabName("Minesweeper");
static const int MaxGridWidth = 100;
static const int MaxGridHeight = 100;

#define LOCTEXT_NAMESPACE "FMinesweeperModule"

void FMinesweeperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FMinesweeperStyle::Initialize();
	FMinesweeperStyle::ReloadTextures();

	FMinesweeperCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweeperCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMinesweeperModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperTabName,
		FOnSpawnTab::CreateRaw(
			this, &FMinesweeperModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMinesweeperTabTitle", "Minesweeper"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMinesweeperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMinesweeperStyle::Shutdown();

	FMinesweeperCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperTabName);
}

TSharedRef<SDockTab> FMinesweeperModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FMinesweeperModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("Minesweeper.cpp"))
	);

	FText WidthText = FText::FromString(TEXT("Width:"));
	FText HeightText = FText::FromString(TEXT("Height:"));
	FText NumberOfMinesText = FText::FromString(TEXT("Number Of Mines:"));

	FSlateFontInfo const BoldTitleFont = FSlateFontInfo(
		FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"),
		24);
	FSlateFontInfo const RegularTitleFont = FSlateFontInfo(
		FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"),
		24);
	FSlateFontInfo const StandardFont = FSlateFontInfo(
		FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"),
		18);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			/** Minesweeper Tab Design, w/ Horizontal & Vertical Scrollboxes */
			SNew(SBorder)
				.Clipping(EWidgetClipping::ClipToBounds)
				[
					/** Outer Canvas */
					SNew(SOverlay)
						+ SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							/** Inner Canvas */
							SNew(SOverlay)
								+ SOverlay::Slot()
								.HAlign(HAlign_Center)
								.VAlign(VAlign_Center)
								[
									/** Content Window */
									SNew(SScrollBox)
										.Orientation(Orient_Horizontal)
										.ExternalScrollbar(HorizontalScrollbar)
										.ScrollBarAlwaysVisible(true)

										+ SScrollBox::Slot()
										.HAlign(HAlign_Left)
										.VAlign(VAlign_Top)
										[
											SNew(SScrollBox)
												.Orientation(Orient_Vertical)
												.ExternalScrollbar(VerticalScrollBar)
												.ScrollBarAlwaysVisible(true)

												+ SScrollBox::Slot()
												.HAlign(HAlign_Left)
												.VAlign(VAlign_Top)
												[

													/** Minesweeper Window Content */
													SNew(SVerticalBox)

														+ SVerticalBox::Slot()
														.HAlign(HAlign_Left)
														.VAlign(VAlign_Top)
														[
															SNew(SHorizontalBox)

																+ SHorizontalBox::Slot()
																.HAlign(HAlign_Fill)
																.VAlign(VAlign_Center)
																[
																	SNew(STextBlock)
																		.Font(BoldTitleFont)
																		.Text(WidthText)
																]

																+ SHorizontalBox::Slot()
																.HAlign(HAlign_Fill)
																.VAlign(VAlign_Center)
																.AutoWidth()
																[
																	SNew(SBox)
																		.WidthOverride(80.f)
																		.VAlign(VAlign_Fill)
																		[
																			/** Width Entry */
																			SAssignNew(WidthEntryTextBox, SEditableTextBox)
																				.Justification(ETextJustify::Left)
																				.Font(StandardFont)
																				.Text(FText::FromString(TEXT("15")))
																		]
																]

																+ SHorizontalBox::Slot()
																.Padding(25, 0, 10, 0)
																.HAlign(HAlign_Fill)
																.VAlign(VAlign_Center)
																[
																	SNew(STextBlock)
																		.Font(BoldTitleFont)
																		.Text(HeightText)
																]

																+ SHorizontalBox::Slot()
																.HAlign(HAlign_Fill)
																.VAlign(VAlign_Center)
																.AutoWidth()
																[
																	SNew(SBox)
																		.WidthOverride(80.f)
																		.VAlign(VAlign_Fill)
																		[
																			/** Height Entry */
																			SAssignNew(HeightEntryTextBox, SEditableTextBox)
																				.Justification(ETextJustify::Left)
																				.Font(StandardFont)
																				.Text(FText::FromString(TEXT("15")))
																		]

																]
														]

														+ SVerticalBox::Slot()
														.HAlign(HAlign_Left)
														.VAlign(VAlign_Top)
														[
															SNew(SHorizontalBox)

																+ SHorizontalBox::Slot()
																.AutoWidth()
																.HAlign(HAlign_Fill)
																.VAlign(VAlign_Center)
																[
																	SNew(STextBlock)
																		.Font(RegularTitleFont)
																		.Text(NumberOfMinesText)
																]

																+ SHorizontalBox::Slot()
																.AutoWidth()
																.HAlign(HAlign_Fill)
																.VAlign(VAlign_Center)
																[

																	SNew(SBox)
																		.WidthOverride(80.f)
																		[
																			/** Number of Mines Entry */
																			SAssignNew(TotalMineCountTextBox, SEditableTextBox)
																				.Justification(ETextJustify::Left)
																				.Font(StandardFont)
																				.Text(FText::FromString(TEXT("50")))
																		]
																]
														]

														+ SVerticalBox::Slot()
														.HAlign(HAlign_Left)
														.VAlign(VAlign_Top)
														.Padding(25, 45, 25, 45)
														[
															SNew(SButton)
																.Text(FText::FromString(TEXT("Generate New Grid")))
																.HAlign(HAlign_Fill)
																.VAlign(VAlign_Fill)
																.OnClicked_Raw(this, &FMinesweeperModule::GenerateMinesweeperGrid)
														]

														+ SVerticalBox::Slot()
														.HAlign(HAlign_Center)
														.VAlign(VAlign_Center)
														.Padding(5, 5, 5, 5)
														[
															SAssignNew(GameOverText, STextBlock)
																.Text(FText::FromString(TEXT("GAME OVER")))
																.Font(BoldTitleFont)
																.Visibility(EVisibility::Hidden)
														]

														+ SVerticalBox::Slot()
														.HAlign(HAlign_Fill)
														.VAlign(VAlign_Fill)
														.AutoHeight()
														.Padding(50, 50, 50, 50)
														[
															SAssignNew(MinesweeperGridPanel, SUniformGridPanel)
														]
												]
										]
								]

								/** Vertical Scroll Bar placement */
								+ SOverlay::Slot()
								.HAlign(HAlign_Right)
								.VAlign(VAlign_Fill)
								[
									SAssignNew(VerticalScrollBar, SScrollBar)
										.Orientation(Orient_Vertical)
										.AlwaysShowScrollbarTrack(true)
								]
						]

						/** Horizontal Scroll Bar placement */
						+ SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Bottom)
						[
							SAssignNew(HorizontalScrollbar, SScrollBar)
								.Orientation(Orient_Horizontal)
								.AlwaysShowScrollbarTrack(true)
						]
				]

		];
}

FReply FMinesweeperModule::GenerateMinesweeperGrid()
{
	UE_LOG(LogTemp, Warning, TEXT("GENERATING MINESWEEPER GRID"))

	/** Setup New Game */
	MinesweeperGridPanel->ClearChildren();
	MineButtons.Empty();
	GridHeight = 0;
	GridWidth = 0;
	TotalMines = 0;
	GameOverText->SetVisibility(EVisibility::Hidden);

	if (HeightEntryTextBox && WidthEntryTextBox && TotalMineCountTextBox)
	{
		// Get the text from the entry boxes
		auto const HeightText = HeightEntryTextBox->GetText();
		auto const WidthText = WidthEntryTextBox->GetText();
		auto const TotalMinesText = TotalMineCountTextBox->GetText();

		if (HeightText.IsNumeric() && WidthText.IsNumeric() && TotalMinesText.IsNumeric())
		{
			// Parse the text to integers
			GridHeight = FCString::Atoi(*HeightText.ToString());
			GridWidth = FCString::Atoi(*WidthText.ToString());
			TotalMines = FCString::Atoi(*TotalMinesText.ToString());

			RemainingBlocks = GridHeight * GridWidth - TotalMines;

			// Log the values for debugging
			UE_LOG(LogTemp, Warning, TEXT("Height: %d"), GridHeight);
			UE_LOG(LogTemp, Warning, TEXT("Width: %d"), GridWidth);
			UE_LOG(LogTemp, Warning, TEXT("Total Mines: %d"), TotalMines);

			// Ensure the grid dimensions and total mines are within reasonable limits
			if (GridHeight > MaxGridHeight)
			{
				GridHeight = MaxGridHeight;
			}
			if (GridWidth > MaxGridWidth)
			{
				GridWidth = MaxGridWidth;
			}
			if (TotalMines >= GridHeight * GridWidth)
			{
				// At least one block must be empty
				TotalMines = GridHeight * GridWidth - 1;
			}

			// Initialize the remaining mines count
			int RemainingMines = TotalMines;

			// Spawn button and Initial spawn for mines
			for (int x{}, y{}; y < GridHeight; ++x)
			{
				bool bSpawnMine{ false };
				if (x == GridWidth)
				{
					++y;
					x = 0;

					if (y == GridHeight)
					{
						break;
					}
				}

				// 30% chance to spawn a mine on the first pass
				if (RemainingMines > 0)
				{
					if (FMath::RandRange(0, 99) < 33)
					{
						bSpawnMine = true;
						RemainingMines--;
					}
				}

				TSharedPtr<SMinesweeperButton> MinesweeperButton;

				// Create a new Minesweeper Button and add it to the Grid Panel
				SUniformGridPanel::FSlot* Slot;
				MinesweeperGridPanel->AddSlot(x, y).Expose(Slot);
				Slot->SetHorizontalAlignment(HAlign_Fill);
				Slot->SetVerticalAlignment(VAlign_Fill);
				Slot->AttachWidget(
					SAssignNew(MinesweeperButton, SMinesweeperButton)
					.bIsMine(bSpawnMine)
					.Coordinates(FVector2D(x, y))
					.MinesweeperModule(this));

				MineButtons.Add(MinesweeperButton);
			}

			// Spawn all the mines
			while (RemainingMines > 0)
			{
				for (auto Button : MineButtons)
				{
					if (Button->HasMine())
					{
						continue;
					}

					// 30% chance to spawn a mine
					bool bSpawnMine{ false };
					if (FMath::RandRange(0, 99) < 33)
					{
						bSpawnMine = true;
						RemainingMines--;
					}

					// Set the mine on the button
					if (bSpawnMine)
					{
						Button->SetMine();
					}

					// If there are no remaining mines, break out of the loop
					if (RemainingMines <= 0)
					{
						break;
					}
				}
			}

			//UE_LOG(LogTemp, Warning, TEXT("Remaining Mines: %d"), RemainingMines)
		}
	}

	return FReply::Handled();
}

void FMinesweeperModule::RevealAdjacentButtons(FVector2D const InVector2D)
{
	for (auto Button : MineButtons)
	{
		if (!Button->IsClicked() && Button->IsAdjacent(InVector2D) && !Button->HasMine())
		{
			auto const Count = CountAdjacentMines(Button->GetCoordinates());

			if (Count == 0)
			{
				Button->SetClicked();
				Button->RevealAdjacency();
			}
			else
			{
				Button->SetClicked();
				Button->SetNeighborMineCount(Count);
			}
		}
	}
}

int FMinesweeperModule::CountAdjacentMines(FVector2D const InVector2D)
{
	int Count{};

	for (auto Button : MineButtons)
	{
		if (!Button->IsClicked() && Button->IsAdjacent(InVector2D) && Button->HasMine())
		{
			Count++;
		}
	}

	return Count;
}

void FMinesweeperModule::MineClicked()
{
	for (auto const Button : MineButtons)
	{
		Button->RevealMine();
		Button->SetVisibility(EVisibility::HitTestInvisible);
	}

	// Display Game Over Text
	GameOverText->SetText(FText::FromString(TEXT("GAME OVER!")));
	GameOverText->SetVisibility(EVisibility::Visible);
}

void FMinesweeperModule::BlockClicked()
{
	RemainingBlocks--;
	if (RemainingBlocks <= 0)
	{
		for (auto const Button : MineButtons)
		{
			Button->SetVisibility(EVisibility::HitTestInvisible);
		}

		// Display YOU WIN! Text
		GameOverText->SetText(FText::FromString(TEXT("YOU WIN!")));
		GameOverText->SetVisibility(EVisibility::Visible);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Blocks: + %d"), RemainingBlocks);
}

void FMinesweeperModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperTabName);
}

void FMinesweeperModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweeperCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(
					FToolMenuEntry::InitToolBarButton(FMinesweeperCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMinesweeperModule, Minesweeper)