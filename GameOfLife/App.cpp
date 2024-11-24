#include "App.h"
#include "MainWindow.h"

wxIMPLEMENT_APP(App);

App::App() {

}

App::~App() {

}

bool App::OnInit() 
{
	// Open window
	mainWindow = new MainWindow();
	mainWindow->Show(true);
	return true;
}