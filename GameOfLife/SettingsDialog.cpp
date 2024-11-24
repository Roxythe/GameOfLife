#include "SettingsDialog.h"
#include "MainWindow.h"

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
	EVT_BUTTON(wxID_OK, SettingsDialog::OnOk)
	EVT_BUTTON(wxID_CANCEL, SettingsDialog::OnCancel)
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* parent, Settings* settings)
	: wxDialog(parent, wxID_ANY, "Settings", wxDefaultPosition, wxSize(250, 250)), settings(settings)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// Grid Size
	wxBoxSizer* gridSizeSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* gridSizeLabel = new wxStaticText(this, wxID_ANY, "Grid Size:");
	gridSizeCtrl = new wxSpinCtrl(this, wxID_ANY);
	gridSizeCtrl->SetRange(5, 100); // Adjust to change range
	gridSizeCtrl->SetValue(settings->gridSize);
	gridSizeSizer->Add(gridSizeLabel, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	gridSizeSizer->Add(gridSizeCtrl, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mainSizer->Add(gridSizeSizer, 0, wxEXPAND | wxALL, 5);

	// Interval
	wxBoxSizer* intervalSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* intervalLabel = new wxStaticText(this, wxID_ANY, "Interval (ms):");
	intervalCtrl = new wxSpinCtrl(this, wxID_ANY);
	intervalCtrl->SetRange(10, 1000); // Adjust to change range
	intervalCtrl->SetValue(settings->timerInterval);
	intervalSizer->Add(intervalLabel, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	intervalSizer->Add(intervalCtrl, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mainSizer->Add(intervalSizer, 0, wxEXPAND | wxALL, 5);

	// Living Cell Color
	wxBoxSizer* livingCellColorSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* livingCellColorLabel = new wxStaticText(this, wxID_ANY, "Cell Color (Living):");
	livingCellColorPicker = new wxColourPickerCtrl(this, wxID_ANY, settings->GetLivingCellColor());
	livingCellColorSizer->Add(livingCellColorLabel, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	livingCellColorSizer->Add(livingCellColorPicker, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mainSizer->Add(livingCellColorSizer, 0, wxEXPAND | wxALL, 5);

	// Dead Cell Color
	wxBoxSizer* deadCellColorSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* deadCellColorLabel = new wxStaticText(this, wxID_ANY, "Cell Color (Dead):");
	deadCellColorPicker = new wxColourPickerCtrl(this, wxID_ANY, settings->GetDeadCellColor());
	deadCellColorSizer->Add(deadCellColorLabel, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	deadCellColorSizer->Add(deadCellColorPicker, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mainSizer->Add(deadCellColorSizer, 0, wxEXPAND | wxALL, 5);

	// OK/Cancel buttons
	wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
	mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);

	SetSizer(mainSizer);
	mainSizer->Fit(this);
	Center();
}

void SettingsDialog::OnOk(wxCommandEvent& event)
{
	// Update the settings object with the new values
	settings->gridSize = gridSizeCtrl->GetValue();
	settings->timerInterval = intervalCtrl->GetValue();
	settings->SetLivingCellColor(livingCellColorPicker->GetColour());
	settings->SetDeadCellColor(deadCellColorPicker->GetColour());

	// Save updated settings
	settings->Save();

	MainWindow* mainWindow = dynamic_cast<MainWindow*>(GetParent());
	if (mainWindow)
	{
		mainWindow->InitializeGrid();
		mainWindow->UpdateStatusBar();
	}

	EndModal(wxID_OK);
}

void SettingsDialog::OnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}