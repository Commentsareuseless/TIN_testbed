#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>

#include <memory>
#include <string>

#include "TCManager.hpp"

#define AVR_TB_VERSION "0.7.2"

namespace tb
{

enum WidgetID : int
{
	unused = 1000,
	ID_BEGIN_TEST_BUTTON,
	ID_SELECT_BUTTON,
	ID_TC_LIST,
	ID_TERMINAL,
	ID_COM_PORT_SEL_AVR,
	ID_COM_PORT_SEL_STM,
	ID_REFRESH_COM_MENU,
	ID_REFRESH_TC_MENU,
	ID_PROGRAM_AVR_MENU
};

class MainWindow : public wxFrame
{
public:
	MainWindow();

	static MainWindow* GetInstance() { return instance; }

	wxRichTextCtrl* GetTerminal() { return terminalWindow; }
	wxChoice* GetAVRCOMList() { return COMsel_AVR; }
	wxChoice* GetSTMCOMList() { return COMsel_STM; }
	wxListBox* GetTCList() { return tcList; }


private:
	static MainWindow* instance;

	wxButton* beginTestB = nullptr;
	wxRichTextCtrl* terminalWindow = nullptr;
	wxChoice* COMsel_AVR = nullptr;
	wxChoice* COMsel_STM = nullptr;
	wxListBox* tcList = nullptr;
	wxButton* selectButton = nullptr;

	std::string selTestcase;

	void BindAvailableEvents();

	void RefreshTCtable();
	void RefreshCOMtable();

	void OnRefreshCOMtable(wxCommandEvent& evt);
	void OnRefreshTCtable(wxCommandEvent& evt);
	void OnTestBegin(wxCommandEvent& evt);
	void OnTCSelected(wxCommandEvent& evt);
	void OnQuit(wxCommandEvent& evt);

};

} // namespace tb
