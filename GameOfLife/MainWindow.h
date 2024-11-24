#pragma once
#include <vector>
#include "wx/wx.h"
#include "DrawingPanel.h"


class DrawingPanel;

class MainWindow : public wxFrame
{
private:
	friend class DrawingPanel; // Allows DrawingPanel to access UpdateStatusBar method
	friend class SettingsDialog; // Allows SettingsDialog to access InitializeGrid method

	void UpdateStatusBar();
	void InitializeGrid();
	int CountLivingNeighbors(int row, int column);
	void CalculateNextGeneration();
	void OnToggleShowNeighborCount(wxCommandEvent& evt);
	void OnRandomize(wxCommandEvent& evt);
	void OnRandomizeWithSeed(wxCommandEvent& evt);
	void RandomizeGrid(int seed);

	void OnNew(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSaveAs(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void SaveFile(const std::string& fileName);

	void OnSelectFinite(wxCommandEvent& event);
	void OnSelectToroidal(wxCommandEvent& event);
	void OnResetSettings(wxCommandEvent& event);
	void OnImport(wxCommandEvent& event);
	void ImportGameBoard(const wxString& path);
	void OnToggleShowGrid(wxCommandEvent& event);
	void OnToggleShowGrid10x10(wxCommandEvent& event);
	void OnToggleShowHUD(wxCommandEvent& event);

	std::vector<std::vector<bool>> gameBoard;

	DrawingPanel* drawingPanel;
	Settings settings;
	wxMenuItem* finiteItem;
	wxMenuItem* toroidalItem;
	wxMenuItem* showNeighborCountItem;
	wxMenuItem* showGridItem;
	wxMenuItem* showGrid10x10Item;
	wxMenuItem* showHUDItem;
	wxStatusBar* statusBar;
	wxTimer* timer;
	wxToolBar* toolBar;

	// Events for toolbar
	void OnPlay(wxCommandEvent& evt);
	void OnPause(wxCommandEvent& evt);
	void OnNext(wxCommandEvent& evt);
	void OnClear(wxCommandEvent& evt);
	void OnTimer(wxTimerEvent& evt);
	void OnOpenSettings(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

public:
	MainWindow();
	~MainWindow();

	int generationCount = 0;
	int livingCellsCount = 0;
};

