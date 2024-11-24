#pragma once
#include <wx/colour.h>
#include <fstream>

struct Settings
{
	// Default member settings
	unsigned int livingCellRed;
	unsigned int livingCellGreen;
	unsigned int livingCellBlue;
	unsigned int livingCellAlpha;

	unsigned int deadCellRed;
	unsigned int deadCellGreen;
	unsigned int deadCellBlue;
	unsigned int deadCellAlpha;

	int gridSize;
	int timerInterval;
	bool showNeighborCount;
	bool isToroidal;
	bool showGrid;
	bool showGrid10x10;
	bool showHUD;

	Settings()
	{
		ResetToDefaults();
	}

	void ResetToDefaults()
	{
		gridSize = 15;
		timerInterval = 50;
		showNeighborCount = false;
		isToroidal = false;
		showGrid = true;
		showGrid10x10 = false;
		showHUD = false;
		livingCellRed = 128;
		livingCellGreen = 128;
		livingCellBlue = 128;
		livingCellAlpha = 255;
		deadCellRed = 255;
		deadCellGreen = 255;
		deadCellBlue = 255;
		deadCellAlpha = 255;
	}

	// Getter for living/dead cell color
	wxColor GetLivingCellColor() const
	{
		return wxColor(livingCellRed, livingCellGreen, livingCellBlue, livingCellAlpha);
	}

	wxColor GetDeadCellColor() const
	{
		return wxColor(deadCellRed, deadCellGreen, deadCellBlue, deadCellAlpha);
	}

	// Setters for living/Dead cell color
	void SetLivingCellColor(const wxColor& color)
	{
		livingCellRed = color.Red();
		livingCellGreen = color.Green();
		livingCellBlue = color.Blue();
		livingCellAlpha = color.Alpha();
	}

	void SetDeadCellColor(const wxColor& color)
	{
		deadCellRed = color.Red();
		deadCellGreen = color.Green();
		deadCellBlue = color.Blue();
		deadCellAlpha = color.Alpha();
	}

	// Save and load settings to bin file
	void Save(const std::string& filename = "settings.bin") const
	{
		std::ofstream file(filename, std::ios::out | std::ios::binary);
		if (file.is_open())
		{
			file.write((char*)this, sizeof(Settings));
			file.close();
		}
	}

	void Load(const std::string& filename = "settings.bin")
	{
		std::ifstream file(filename, std::ios::binary | std::ios::in);
		if (file.is_open())
		{
			file.read((char*)this, sizeof(Settings));
			file.close();
		}
		else
		{
			ResetToDefaults(); // If no file exists load default settings
		}
	}
};