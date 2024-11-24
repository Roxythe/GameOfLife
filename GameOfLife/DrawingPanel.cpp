#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include "MainWindow.h"

// Create event panel
wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_SIZE(DrawingPanel::OnSize)
EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()
// Drawing Panel constructor
DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& board)
	: wxPanel(parent, wxID_ANY), gameBoard(board)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}
// Default deconstructor
DrawingPanel::~DrawingPanel()
{}

void DrawingPanel::SetGridSize(int size)
{
	gridSize = size;
	Refresh(); // Redraw panel with new grid size
}

// OnPaint method
void DrawingPanel::OnPaint(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();
	wxGraphicsContext* context = wxGraphicsContext::Create(dc);
	if (!context) return;
	// Set boarder color/fill panel color
	context->SetPen(*wxBLACK_PEN);

	// Get panel size
	int panelWidth, panelHeight;
	GetClientSize(&panelWidth, &panelHeight);

	// Calculate the cell size
	float cellWidth = static_cast<float>(panelWidth) / gridSize;
	float cellHeight = static_cast<float>(panelHeight) / gridSize;

	for (int row = 0; row < gridSize; ++row)
	{
		for (int column = 0; column < gridSize; ++column)
		{
			float x = column * cellWidth;
			float y = row * cellHeight;

			// Darw cell border
			context->DrawRectangle(x, y, cellWidth, cellHeight);

			// Test if cell click works
			if (gameBoard[row][column])
			{
				context->SetBrush(*wxLIGHT_GREY_BRUSH); // Fill alive cell
			}
			else
			{
				context->SetBrush(*wxWHITE_BRUSH); // Fill dead cell
			}
			context->DrawRectangle(x, y, cellWidth, cellHeight);
		}
	}
	delete context;
}
// OnSize method
void DrawingPanel::OnSize(wxSizeEvent& evt)
{
	Refresh(); // Redraw panel when resized
	evt.Skip();// Continue processing event for future resizing
}

void DrawingPanel::OnMouseUp(wxMouseEvent& evt)
{
	// Get mouse coordinates of click
	int mouseX = evt.GetX();
	int mouseY = evt.GetY();
	// Get panel size
	int panelWidth, panelHeight;
	GetClientSize(&panelWidth, &panelHeight);
	// Calculate cell size
	float cellWidth = static_cast<float>(panelWidth) / gridSize;
	float cellHeight = static_cast<float>(panelHeight) / gridSize;

	// Determine which cell was clicked
	int column = static_cast<int>(mouseX / cellWidth);
	int row = static_cast<int>(mouseY / cellHeight);

	// Toggle cell
	if (row >= 0 && row < gridSize && column >= 0 && column < gridSize)
	{
		gameBoard[row][column] = !gameBoard[row][column];

		static_cast<MainWindow*>(GetParent())->UpdateStatusBar();
	}
	Refresh(); // Redraw panel to reflect changes
}
