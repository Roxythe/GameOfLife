#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "SettingsDialog.h"
#include <wx/numdlg.h>
#include <cstdlib>
#include <ctime>

enum
{
	ID_Play = 10001,
	ID_Pause,
	ID_Next,
	ID_Clear,
	ID_Settings,
	ID_NeighborCount,
	ID_Randomize,
	ID_RandomizeWithSeed,
	ID_New,
	ID_Open,
	ID_Save,
	ID_SaveAs,
	ID_Exit,
	ID_Finite,
	ID_Toroidal,
	ID_ResetSettings,
	ID_Import,
	ID_ShowGrid,
	ID_ShowGrid10x10,
	ID_ShowHUD,
	ID_Timer
};

// Event Table for tool bar
wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(ID_Play, MainWindow::OnPlay)
EVT_MENU(ID_Pause, MainWindow::OnPause)
EVT_MENU(ID_Next, MainWindow::OnNext)
EVT_MENU(ID_Clear, MainWindow::OnClear)
EVT_MENU(ID_Settings, MainWindow::OnOpenSettings)
EVT_MENU(ID_NeighborCount, MainWindow::OnToggleShowNeighborCount)
EVT_MENU(ID_Randomize, MainWindow::OnRandomize)
EVT_MENU(ID_RandomizeWithSeed, MainWindow::OnRandomizeWithSeed)
EVT_MENU(ID_New, MainWindow::OnNew)
EVT_MENU(ID_Open, MainWindow::OnOpen)
EVT_MENU(ID_Save, MainWindow::OnSave)
EVT_MENU(ID_SaveAs, MainWindow::OnSaveAs)
EVT_MENU(ID_Exit, MainWindow::OnExit)
EVT_MENU(ID_Finite, MainWindow::OnSelectFinite)
EVT_MENU(ID_Toroidal, MainWindow::OnSelectToroidal)
EVT_MENU(ID_ResetSettings, MainWindow::OnResetSettings)
EVT_MENU(ID_Import, MainWindow::OnImport)
EVT_MENU(ID_ShowGrid, MainWindow::OnToggleShowGrid)
EVT_MENU(ID_ShowGrid10x10, MainWindow::OnToggleShowGrid10x10)
EVT_MENU(ID_ShowHUD, MainWindow::OnToggleShowHUD)
EVT_TIMER(ID_Timer, MainWindow::OnTimer)
wxEND_EVENT_TABLE()

void MainWindow::OnRandomize(wxCommandEvent& evt)
{
	RandomizeGrid(static_cast<int>(time(NULL)));
}

void MainWindow::OnRandomizeWithSeed(wxCommandEvent& evt)
{
	long seed = wxGetNumberFromUser("Enter seed for randomization", "Seed:", "Randomize with seed", 0, LONG_MIN, LONG_MAX, this);
	if (seed != -1)
	{
		RandomizeGrid(static_cast<int>(seed));
	}
	UpdateStatusBar();
}

void MainWindow::RandomizeGrid(int seed)
{
	srand(seed);

	for (int row = 0; row < settings.gridSize; ++row)
	{
		for (int column = 0; column < settings.gridSize; ++column)
		{
			gameBoard[row][column] = (rand() % 2 == 0); // 50% chance cell will be alive
		}
	}
	drawingPanel->Refresh();
	UpdateStatusBar();
}

std::string currentFileName;

void MainWindow::OnNew(wxCommandEvent& event)
{
	gameBoard.clear();
	InitializeGrid();
	currentFileName.clear();
	drawingPanel->Refresh();
}

void MainWindow::OnOpen(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Open Cells File"), "", "", "Cells files (*.cells)|*.cells",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	// Open File
	std::ifstream file(openFileDialog.GetPath().ToStdString());
	if (!file.is_open())
	{
		wxMessageBox("Could not open file", "Error", wxICON_ERROR);
		return;
	}

	std::vector<std::string> fileContents;
	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == '!')
			continue; // Skip comments
		fileContents.push_back(line);
	}

	// Resize grid to match file
	settings.gridSize = fileContents.size();
	InitializeGrid();

	// Load file into grid
	for (int row = 0; row < settings.gridSize && row < fileContents.size(); ++row)
	{
		for (int col = 0; col < settings.gridSize && col < fileContents[row].size(); ++col)
		{
			gameBoard[row][col] = (fileContents[row][col] == '*');
		}
	}

	file.close();
	currentFileName = openFileDialog.GetPath().ToStdString();
	drawingPanel->Refresh();
}

void MainWindow::OnSaveAs(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, _("Save Cells file"), "", "", "Cells file (*.cells)|*.cells",
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	currentFileName = saveFileDialog.GetPath().ToStdString();
	SaveFile(currentFileName);
}

void MainWindow::OnSave(wxCommandEvent& event)
{
	if (currentFileName.empty())
	{
		OnSaveAs(event); // If no file name is saved, use save as
	}
	else
	{
		SaveFile(currentFileName);
	}
}

void MainWindow::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void MainWindow::SaveFile(const std::string& fileName)
{
	std::ofstream file(fileName);
	if (!file.is_open())
	{
		wxMessageBox("Could not save file", "Error", wxICON_ERROR);
		return;
	}

	for (int row = 0; row < settings.gridSize; ++row)
	{
		for (int col = 0; col < settings.gridSize; ++col)
		{
			file << (gameBoard[row][col] ? '*' : '.');
		}
		file << '\n';
	}

	file.close();
}

void MainWindow::OnSelectFinite(wxCommandEvent& event)
{
	settings.isToroidal = false;
	finiteItem->Check(true);
	toroidalItem->Check(false);

	drawingPanel->Refresh();
}

void MainWindow::OnSelectToroidal(wxCommandEvent& event)
{
	settings.isToroidal = true;
	finiteItem->Check(false);
	toroidalItem->Check(true);

	drawingPanel->Refresh();
}

void MainWindow::OnResetSettings(wxCommandEvent& event)
{
	settings.ResetToDefaults();
	settings.Save();

	InitializeGrid();
	UpdateStatusBar();
	drawingPanel->Refresh();

	finiteItem->Check(!settings.isToroidal);
	toroidalItem->Check(settings.isToroidal);
	showNeighborCountItem->Check(settings.showNeighborCount);
	showGridItem->Check(settings.showGrid);
	showGrid10x10Item->Check(settings.showGrid10x10);
	showHUDItem->Check(settings.showHUD);

	wxMessageBox("Settings have been reset to their default values.", "Settings Reset", wxOK);
}

void MainWindow::OnImport(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Import Game Board"), "", "", "Cells Files (*.cells)|*.cells",
								wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	wxString path = openFileDialog.GetPath();
	ImportGameBoard(path);
}

void MainWindow::ImportGameBoard(const wxString& path)
{
	std::ifstream file(path.ToStdString());
	if (!file.is_open())
	{
		wxMessageBox("The file could not be opened.", "Error", wxOK | wxICON_ERROR);
		return;
	}

	std::vector<std::string> patternLines;
	std::string line;

	// Read the imported file, skip comment lines and collect pattern lines
	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == '!') // Skip comments and empty lines
			continue;
		patternLines.push_back(line);
	}
	file.close();

	int patternRows = static_cast<int>(patternLines.size());
	int patternCols = 0;
	for (const auto& patternLine : patternLines)
	{
		patternCols = std::max(patternCols, static_cast<int>(patternLine.size()));
	}

	//// Check if current grid size is large enough
	//if (patternRows > settings.gridSize || patternCols > settings.gridSize)
	//{
	//	int newGridSize = std::max(patternRows, patternCols);
	//	settings.gridSize = newGridSize;
	//	InitializeGrid();
	//}

	// Calculation to center the pattern on game board
	int startRow = (settings.gridSize - patternRows) / 2;
	int startCol = (settings.gridSize - patternCols) / 2;

	// Import pattern into game board at centered position
	for (int row = 0; row < patternRows; ++row)
	{
		for (int col = 0; col < patternLines[row].size(); ++col)
		{
			// Only import cells that fit current grid size
			if (startRow + row < settings.gridSize && startRow + row >= 0 &&
				startCol + col < settings.gridSize && startCol + col >= 0)
			{
				gameBoard[startRow + row][startCol + col] = (patternLines[row][col] == '*');
			}
		}
	}
	drawingPanel->Refresh();
}

void MainWindow::InitializeGrid()
{
	gameBoard.resize(settings.gridSize);

	for (int i = 0; i < settings.gridSize; ++i)
	{
		gameBoard[i].resize(settings.gridSize, false);
	}

	if (drawingPanel)
	{
		drawingPanel->SetGridSize(settings.gridSize);
		drawingPanel->Refresh();
	}
}

void MainWindow::UpdateStatusBar()
{
	// Recalculate the number of living cells
	livingCellsCount = 0;
	for (int row = 0; row < settings.gridSize; ++row)
	{
		for (int column = 0; column < settings.gridSize; ++column)
		{
			if (gameBoard[row][column])
			{
				livingCellsCount++;
			}
		}
	}
	// Update text on status bar
	wxString statusText = wxString::Format("Generations: %d | Living Cells: %d", generationCount, livingCellsCount);
	statusBar->SetStatusText(statusText);
}

// Main window constructor
MainWindow::MainWindow()
	: wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(600, 600)), finiteItem(nullptr), toroidalItem(nullptr)
{
	settings.Load(); // Load settings
	InitializeGrid(); // Initialize game board
	statusBar = this->CreateStatusBar(); // Create status bar
	UpdateStatusBar(); // Initialize status bar
	timer = new wxTimer(this, ID_Timer);
	toolBar = this->CreateToolBar(); // Initialize tool bar

	wxMenuBar* menuBar = new wxMenuBar;

	// File menu bar
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(ID_Import, "Import...", "Import game board");
	fileMenu->Append(ID_New, "New", "Create new game board");
	fileMenu->Append(ID_Open, "Open", "Open saved game board");
	fileMenu->Append(ID_Save, "Save", "Save gameboard");
	fileMenu->Append(ID_SaveAs, "Save As", "Save game board as new file");
	fileMenu->Append(ID_Exit, "Exit", "Exit game");
	menuBar->Append(fileMenu, "File");

	// Options menu bar
	wxMenu* optionsMenu = new wxMenu;
	optionsMenu->Append(ID_Randomize, "Randomize", "Randomize the grid");
	optionsMenu->Append(ID_RandomizeWithSeed, "Randomize with seed", "Randomize the grid with a seed");
	optionsMenu->Append(ID_Settings, "Change Settings...", "Change game settings");
	optionsMenu->Append(ID_ResetSettings, "Restore Default Settings", "Restore settings to default values");
	menuBar->Append(optionsMenu, "Options");

	// View menu bar
	wxMenu* viewMenu = new wxMenu;
	showNeighborCountItem = new wxMenuItem(viewMenu, ID_NeighborCount, "Show Neighbor Count", "", wxITEM_CHECK);
	viewMenu->Append(showNeighborCountItem);
	showNeighborCountItem->Check(settings.showNeighborCount);

	finiteItem = new wxMenuItem(viewMenu, ID_Finite, "Finite", "", wxITEM_CHECK);
	finiteItem->SetCheckable(true);
	viewMenu->Append(finiteItem);
	finiteItem->Check(!settings.isToroidal);

	toroidalItem = new wxMenuItem(viewMenu, ID_Toroidal, "Toroidal", "", wxITEM_CHECK);
	toroidalItem->SetCheckable(true);
	viewMenu->Append(toroidalItem);
	toroidalItem->Check(settings.isToroidal);

	showGridItem = new wxMenuItem(viewMenu, ID_ShowGrid, "Show Grid", "", wxITEM_CHECK);
	showGridItem->SetCheckable(true);
	viewMenu->Append(showGridItem);
	showGridItem->Check(settings.showGrid);

	showGrid10x10Item = new wxMenuItem(viewMenu, ID_ShowGrid10x10, "Show Grid 10x10", "", wxITEM_CHECK);
	showGrid10x10Item->SetCheckable(true);
	viewMenu->Append(showGrid10x10Item);
	showGrid10x10Item->Check(settings.showGrid10x10);

	showHUDItem = new wxMenuItem(viewMenu, ID_ShowHUD, "Show HUD", "", wxITEM_CHECK);
	showHUDItem->SetCheckable(true);
	viewMenu->Append(showHUDItem);
	showHUDItem->Check(settings.showHUD);

	menuBar->Append(viewMenu, "View"); // Add view menu to menu bar

	SetMenuBar(menuBar);

	// Create icons for toolbar buttons
	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap clearIcon(trash_xpm);

	// Add buttons to tool bar
	toolBar->AddTool(ID_Play, "Play", playIcon);
	toolBar->AddTool(ID_Pause, "Pause", pauseIcon);
	toolBar->AddTool(ID_Next, "Next", nextIcon);
	toolBar->AddTool(ID_Clear, "Clear", clearIcon);

	toolBar->Realize(); // Render tool bar on window

	// Create drawing panel and pass gameboard reference
	drawingPanel = new DrawingPanel(this, gameBoard, &settings);
	drawingPanel->SetGridSize(settings.gridSize); // Set grid size in drawing panel

	this->Layout(); // Refresh layout to make sure status bar is visible
}

// Main window default deconstructor
MainWindow::~MainWindow()
{
	// Delete timer
	if (timer)
	{
		delete timer;
	}
}

int MainWindow::CountLivingNeighbors(int row, int column)
{
	int livingNeighbors = 0;

	// Loop through neighboring cells
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			// Skip initial cell
			if (i == 0 && j == 0) continue;

			int neighborRow = row + i;
			int neighborColumn = column + j;

			// Toroidal logic
			if (settings.isToroidal)
			{
				if (neighborRow < 0) neighborRow = settings.gridSize - 1;
				if (neighborRow >= settings.gridSize) neighborRow = 0;
				if (neighborColumn < 0) neighborColumn = settings.gridSize - 1;
				if (neighborColumn >= settings.gridSize) neighborColumn = 0;
			}
			else
			{
				if (neighborRow < 0 || neighborRow >= settings.gridSize || neighborColumn < 0 || neighborColumn >= settings.gridSize)
				{
					continue; // Skip out of bounds cell in finite mode
				}
			}

			if (gameBoard[neighborRow][neighborColumn]) // Check if neighbor is alive
			{
				livingNeighbors++;
			}
		}
	}
	return livingNeighbors;
}

void MainWindow::CalculateNextGeneration()
{
	// Make sandbox to store next generation
	std::vector<std::vector<bool>> sandbox;
	sandbox.resize(settings.gridSize);

	for (int i = 0; i < settings.gridSize; ++i)
	{
		sandbox[i].resize(settings.gridSize, false);
	}

	// Iterate over each cell in game board
	livingCellsCount = 0;
	for (int row = 0; row < settings.gridSize; ++row)
	{
		for (int column = 0; column < settings.gridSize; ++column)
		{
			int livingNeighbors = CountLivingNeighbors(row, column);
			if (gameBoard[row][column]) // Cell is currently alive
			{
				if (livingNeighbors < 2 || livingNeighbors > 3)
				{
					sandbox[row][column] = false; // Cell dies
				}
				else
				{
					sandbox[row][column] = true; // Cell lives
					livingCellsCount++;
				}
			}
			else // Cell is currently dead
			{
				if (livingNeighbors == 3)
				{
					sandbox[row][column] = true; // Dead cell becomes alive
					livingCellsCount++;
				}
			}
		}
	}
	// Move sandbox values to game board
	gameBoard.swap(sandbox);
	generationCount++;
	UpdateStatusBar();
	drawingPanel->Refresh();
}

void MainWindow::OnPlay(wxCommandEvent& evt)
{
	timer->Start(settings.timerInterval);
}

void MainWindow::OnPause(wxCommandEvent& evt)
{
	timer->Stop();
}

void MainWindow::OnNext(wxCommandEvent& evt)
{
	CalculateNextGeneration();
	timer->Stop();
	UpdateStatusBar();
}

void MainWindow::OnClear(wxCommandEvent& evt)
{
	// Reset game board to all values being false (dead)
	for (int row = 0; row < settings.gridSize; ++row)
	{
		for (int column = 0; column < settings.gridSize; ++column)
		{
			gameBoard[row][column] = false;
		}
	}

	// Reset living cell count and generation count
	livingCellsCount = 0;
	generationCount = 0;

	timer->Stop();
	UpdateStatusBar();
	drawingPanel->Refresh();
}

void MainWindow::OnTimer(wxTimerEvent& evt)
{
	CalculateNextGeneration();
}

void MainWindow::OnOpenSettings(wxCommandEvent& evt)
{
	SettingsDialog dialogWindow(this, &settings);
	if (dialogWindow.ShowModal() == wxID_OK)
	{
		// Apply Changes when dialog box is closed with ok and refresh window
		drawingPanel->Refresh();
	}
}

void MainWindow::OnToggleShowNeighborCount(wxCommandEvent& evt)
{
	settings.showNeighborCount = showNeighborCountItem->IsChecked();
	settings.Save();
	drawingPanel->Refresh();
}
void MainWindow::OnToggleShowGrid(wxCommandEvent& event)
{
	settings.showGrid = showGridItem->IsChecked();
	settings.Save();
	drawingPanel->Refresh();
}

void MainWindow::OnToggleShowGrid10x10(wxCommandEvent& event)
{
	settings.showGrid10x10 = showGrid10x10Item->IsChecked();
	settings.Save();
	drawingPanel->Refresh();
}

void MainWindow::OnToggleShowHUD(wxCommandEvent& event)
{
	settings.showHUD = showHUDItem->IsChecked();
	settings.Save();
	drawingPanel->Refresh();
}
