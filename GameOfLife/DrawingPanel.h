#pragma once
#include "wx/wx.h"
#include <vector>

class DrawingPanel : public wxPanel
{
private:
	// Declare events
	void OnPaint(wxPaintEvent& evt);
	void OnSize(wxSizeEvent& evt);
	void OnMouseUp(wxMouseEvent& evt);

	int gridSize = 15;
	std::vector<std::vector<bool>>& gameBoard;

	wxDECLARE_EVENT_TABLE();

public:
	DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& board);
	~DrawingPanel();
	// Declare method to set grid size
	void SetGridSize(int size);
};

