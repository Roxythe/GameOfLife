#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"

enum
{
	ID_Play = 10001,
	ID_Pause,
	ID_Next,
	ID_Clear,
	ID_Timer = 10005
};
// Event Table for tool bar
wxBEGIN_EVENT_TABLE(MainWindow,wxFrame)
EVT_MENU(ID_Play, MainWindow::OnPlay)
EVT_MENU(ID_Pause, MainWindow::OnPause)
EVT_MENU(ID_Next, MainWindow::OnNext)
EVT_MENU(ID_Clear, MainWindow::OnClear)
EVT_TIMER(ID_Timer, MainWindow::OnTimer)
wxEND_EVENT_TABLE()


void MainWindow::InitializeGrid()
{
	gameBoard.resize(gridSize);

	for (int i = 0; i < gridSize; ++i)
	{
		gameBoard[i].resize(gridSize, false);
	}
}

void MainWindow::UpdateStatusBar()
{
	// Recalculate the number of living cells
	livingCellsCount = 0;
	for (int row = 0; row < gridSize; ++row)
	{
		for (int column = 0; column < gridSize; ++column)
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
MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(300, 300))
{
	InitializeGrid(); // Initialize game board
	statusBar = this->CreateStatusBar(); // Create status bar
	UpdateStatusBar(); // Initialize status bar

	timer = new wxTimer(this, ID_Timer);

	toolBar = this->CreateToolBar(); // Initialize tool bar

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
	drawingPanel = new DrawingPanel(this, gameBoard);
	drawingPanel->SetGridSize(gridSize); // Set grid size in drawing panel

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
			if (i == 0 && j == 0)
				continue;

			int neighborRow = row + i;
			int neighborColumn = column + j;

			// Check if neighbor cell is within bounds
			if (neighborRow >= 0 && neighborRow < gridSize && neighborColumn >= 0 && neighborColumn < gridSize)
			{
				if (gameBoard[neighborRow][neighborColumn]) // Check if neighbor is alive
				{
					livingNeighbors++;
				}
			}
		}
	}
	return livingNeighbors;
}

void MainWindow::CalculateNextGeneration()
{
	// Make sandbox to store next generation
	std::vector<std::vector<bool>> sandbox;
	sandbox.resize(gridSize);

	for (int i = 0; i < gridSize; ++i)
	{
		sandbox[i].resize(gridSize, false);
	}

	// Iterate over each cell in game board
	livingCellsCount = 0;
	for (int row = 0; row < gridSize; ++row)
	{
		for (int column = 0; column < gridSize; ++column)
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
	timer->Start(timerInterval);
}
void MainWindow::OnPause(wxCommandEvent& evt)
{
	timer->Stop();
}
void MainWindow::OnNext(wxCommandEvent& evt)
{
	CalculateNextGeneration();
}
void MainWindow::OnClear(wxCommandEvent& evt)
{
	// Reset game board to all values being false (dead)
	for (int row = 0; row < gridSize; ++row)
	{
		for (int column = 0; column < gridSize; ++column)
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

