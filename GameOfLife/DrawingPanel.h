#pragma once
#include "wx/wx.h"
#include <vector>
#include "Settings.h"

class DrawingPanel : public wxPanel
{
private:
	// Declare events
	void OnPaint(wxPaintEvent& evt);
	void OnSize(wxSizeEvent& evt);
	void OnMouseUp(wxMouseEvent& evt);
	void CalculateNeighborCounts();

	std::vector<std::vector<int>> neighborCounts;
	std::vector<std::vector<bool>>& gameBoard;
	Settings* settings;

	wxDECLARE_EVENT_TABLE();

public:
	DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& board, Settings* settings);
	~DrawingPanel();
	// Declare method to set grid size
	void SetGridSize(int size);
};

