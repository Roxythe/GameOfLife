#pragma once
#include "wx/wx.h"
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>
#include "Settings.h"

class SettingsDialog : public wxDialog
{
public:
	SettingsDialog(wxWindow* parent, Settings* settings);

private:
	Settings* settings;

	wxSpinCtrl* gridSizeCtrl;
	wxSpinCtrl* intervalCtrl;
	wxColourPickerCtrl* livingCellColorPicker;
	wxColourPickerCtrl* deadCellColorPicker;

	void OnOk(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};

