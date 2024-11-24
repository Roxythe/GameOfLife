#pragma once
#include <vector>
#include "wx/wx.h"
#include "DrawingPanel.h"

class DrawingPanel;

class MainWindow : public wxFrame
{
private:
	friend class DrawingPanel; // Allows DrawingPanel to access UpdateStatusBar method
	void UpdateStatusBar();
	void InitializeGrid();
	int CountLivingNeighbors(int row, int column);
	void CalculateNextGeneration();

	std::vector<std::vector<bool>> gameBoard;
	int gridSize = 15;

	int generationCount = 0;
	int livingCellsCount = 0;
	wxStatusBar* statusBar;
	wxToolBar* toolBar;
	DrawingPanel* drawingPanel;
	wxTimer* timer;
	int timerInterval = 50;

	// Events for toolbar
	void OnPlay(wxCommandEvent& evt);
	void OnPause(wxCommandEvent& evt);
	void OnNext(wxCommandEvent& evt);
	void OnClear(wxCommandEvent& evt);
	void OnTimer(wxTimerEvent& evt);

	wxDECLARE_EVENT_TABLE();

public:
	MainWindow();
	~MainWindow();

};

