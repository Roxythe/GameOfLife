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
DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& board, Settings* settings)
	: wxPanel(parent, wxID_ANY), gameBoard(board), settings(settings)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

// Default deconstructor
DrawingPanel::~DrawingPanel()
{}

void DrawingPanel::SetGridSize(int size)
{
	if (settings)
	{
		settings->gridSize = size;
		Refresh();
	}
}

void DrawingPanel::CalculateNeighborCounts()
{
	if (neighborCounts.size() != settings->gridSize)
	{
		neighborCounts.resize(settings->gridSize, std::vector<int>(settings->gridSize, 0));
	}

	for (int row = 0; row < settings->gridSize; ++row)
	{
		if (neighborCounts[row].size() != settings->gridSize)
		{
			neighborCounts[row].resize(settings->gridSize, 0);
		}
		for (int column = 0; column < settings->gridSize; ++column)
		{
			if (neighborCounts[column].size() != settings->gridSize)
			{
				neighborCounts[column].resize(settings->gridSize, 0);
			}
			int count = 0;
			// Calculate number of living neighbors
			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					if (i == 0 && j == 0) continue;

					int neighborRow = row + i;
					int neighborCol = column + j;

					// Toroidal logic
					if (settings->isToroidal)
					{
						if (neighborRow < 0) neighborRow = settings->gridSize - 1;
						if (neighborRow >= settings->gridSize) neighborRow = 0;
						if (neighborCol < 0) neighborCol = settings->gridSize - 1;
						if (neighborCol >= settings->gridSize) neighborCol = 0;
					}
					else
					{
						if (neighborRow < 0 || neighborRow >= settings->gridSize || neighborCol < 0 || neighborCol >= settings->gridSize)
						{
							continue; // Skip out of bounds cell in finite mode
						}
					}

					count += gameBoard[neighborRow][neighborCol];
				}
			}
			neighborCounts[row][column] = count;
		}
	}
}

// OnPaint method
void DrawingPanel::OnPaint(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();
	wxGraphicsContext* context = wxGraphicsContext::Create(dc);
	if (!context) return;



	// Get panel size
	int panelWidth, panelHeight;
	GetClientSize(&panelWidth, &panelHeight);

	if (settings)
	{
		// Calculate the cell size
		float cellWidth = static_cast<float>(panelWidth) / settings->gridSize;
		float cellHeight = static_cast<float>(panelHeight) / settings->gridSize;

		CalculateNeighborCounts(); // Get neighbor count

		// Draw cells
		for (int row = 0; row < settings->gridSize; ++row)
		{
			for (int column = 0; column < settings->gridSize; ++column)
			{
				float x = column * cellWidth;
				float y = row * cellHeight;

				if (gameBoard[row][column])
				{
					context->SetBrush(wxBrush(settings->GetLivingCellColor())); // Fill alive cell
				}
				else
				{
					context->SetBrush(wxBrush(settings->GetDeadCellColor())); // Fill dead cell
				}
				context->DrawRectangle(x, y, cellWidth, cellHeight);

				if (settings->showNeighborCount && neighborCounts[row][column] > 0)
				{
					wxString text = wxString::Format("%d", neighborCounts[row][column]);
					double textWidth, textHeight;
					context->SetFont(wxFontInfo(16), *wxRED); // Set format for text size and color
					context->GetTextExtent(text, &textWidth, &textHeight);
					context->DrawText(text, x + (cellWidth - textWidth) / 2, y + (cellHeight - textHeight) / 2);
				}
			}
		}

		// Draw grid lines if show show grid enabled
		if (settings->showGrid)
		{
			dc.SetPen(wxPen(wxColor(0, 0, 0, 64))); // Light color for normal grid lines

			// Normal grid lines
			for (int row = 1; row < settings->gridSize; ++row)
			{
				float y = row * cellHeight;
				dc.DrawLine(wxPoint(0, y), wxPoint(panelWidth, y));
			}

			for (int column = 1; column < settings->gridSize; ++column)
			{
				float x = column * cellWidth;
				dc.DrawLine(wxPoint(x, 0), wxPoint(x, panelHeight));
			}

			// Draw thicker 10x10 grid lines if show grid 10x10 enabled
			if (settings->showGrid10x10)
			{
				dc.SetPen(wxPen(*wxBLACK, 2)); // Thicker black lines
				int solidLines = settings->gridSize / 10;

				for (int i = 1; i <= solidLines; ++i)
				{
					float x = i * 10 * cellWidth;
					dc.DrawLine(wxPoint(x, 0), wxPoint(x, panelHeight));

					float y = i * 10 * cellHeight;
					dc.DrawLine(wxPoint(0, y), wxPoint(panelWidth, y));
				}
			}
		}
		// Draw HUD if enabled
		if (settings->showHUD)
		{
			context->SetFont(wxFontInfo(16), *wxRED); // Font and color for HUD
			wxString hudText;

			//Info for HUD
			hudText += wxString::Format("Generations %d\n", static_cast<MainWindow*>(GetParent())->generationCount);
			hudText += wxString::Format("Living Cells: %d\n", static_cast<MainWindow*>(GetParent())->livingCellsCount);
			hudText += wxString::Format("Boundary Type %s\n", settings->isToroidal ? "Toroidal" : "Finite");
			hudText += wxString::Format("Universe Size %dx%d\n", settings->gridSize, settings->gridSize);

			double textWidth, textHeight;
			context->GetTextExtent(hudText, &textWidth, &textHeight);
			context->DrawText(hudText, 10, panelHeight - textHeight - 10); // Position HUD in bottom left
		}
	}
	delete context;
}

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
	float cellWidth = static_cast<float>(panelWidth) / settings->gridSize;
	float cellHeight = static_cast<float>(panelHeight) / settings->gridSize;

	// Determine which cell was clicked
	int column = static_cast<int>(mouseX / cellWidth);
	int row = static_cast<int>(mouseY / cellHeight);

	// Toggle cell
	if (row >= 0 && row < settings->gridSize && column >= 0 && column < settings->gridSize)
	{
		gameBoard[row][column] = !gameBoard[row][column];

		static_cast<MainWindow*>(GetParent())->UpdateStatusBar();
	}
	Refresh(); // Redraw panel to reflect changes
}

