#include "MainWindow.hpp"
#include "GUIManager.hpp"
#include "SerialCom.hpp"

namespace tb
{
	constexpr char _WINDOW_TITLE[] = "AVR TestBed v" AVR_TB_VERSION; 

	MainWindow* MainWindow::instance = nullptr;

MainWindow::MainWindow() : wxFrame(
	nullptr, wxID_ANY, _WINDOW_TITLE, wxDefaultPosition, wxSize(840, 480))
{
	if (instance == nullptr)
	{
		instance = this;
	}
	else
	{
		return;
	}

	constexpr int HSpacing		= 2;
	constexpr int VSpacing		= 2;
	constexpr int BorderWidth	= 5;
	constexpr int numOfRows		= 3;
	constexpr int numOfCols		= 2;

	wxPanel* background			= new wxPanel(this, wxID_ANY);
	wxBoxSizer* bgBox			= new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* terminalBox		= new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* UIBox			= new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* selectionBox	= new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* buttonBox		= new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* chipViewBox		= new wxBoxSizer(wxVERTICAL);

	////////////////////////////////////////////////////////////////
	// Menu on top ribbon
	////////////////////////////////////////////////////////////////

	wxMenuBar* menu = new wxMenuBar();
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(ID_PROGRAM_AVR_MENU, "Program AVR");
	fileMenu->Append(ID_REFRESH_COM_MENU, "&Refresh COM ports");
	fileMenu->Append(ID_REFRESH_TC_MENU, "Refresh test cases");
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, "&Quit");
	menu->Append(fileMenu, "File");

	Bind(wxEVT_MENU, &MainWindow::OnQuit, this, wxID_EXIT);
	//Bind();

	////////////////////////////////////////////////////////////////
	// COM port selection field
	////////////////////////////////////////////////////////////////

	wxStaticText* COMLabel_AVR = new wxStaticText(background, wxID_ANY, "COM selection (AVR)");
	COMsel_AVR = new wxChoice(background, ID_COM_PORT_SEL_AVR);
	wxStaticText* COMLabel_STM = new wxStaticText(background, wxID_ANY, "COM selection (STM)");
	COMsel_STM = new wxChoice(background, ID_COM_PORT_SEL_STM);

	////////////////////////////////////////////////////////////////
	// Test case selection area
	////////////////////////////////////////////////////////////////

	wxStaticText* tcLabel = new wxStaticText(background, wxID_ANY, "Test cases");
	tcList = new wxListBox(background, ID_TC_LIST);

	selectButton = new wxButton(background, ID_SELECT_BUTTON, "Select");
	beginTestB = new wxButton(background, ID_BEGIN_TEST_BUTTON, "Test!");

	////////////////////////////////////////////////////////////////
	// "Terminal" window (displays info about tests)
	////////////////////////////////////////////////////////////////

	terminalWindow = new wxRichTextCtrl(
		background, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxRE_READONLY | wxRE_MULTILINE);
	terminalWindow->SetBackgroundColour(wxColour(*wxBLACK));
	terminalWindow->SetDefaultStyle(wxTextAttr(*wxLIGHT_GREY));
	terminalWindow->WriteText("======= ");
	terminalWindow->WriteText(_WINDOW_TITLE);
	terminalWindow->WriteText(" =======");
	terminalWindow->Newline();
	wxStaticText* terminalLabel = new wxStaticText(background, wxID_ANY, "Test output");

	////////////////////////////////////////////////////////////////
	// Unused - this might be chip visualisation in the future 
	// also this may be replaced by bigger terminal window
	////////////////////////////////////////////////////////////////

	wxPanel* p4 = new wxPanel(background, wxID_ANY);
	p4->SetBackgroundColour(wxColor(0x00000a));

	////////////////////////////////////////////////////////////////
	// Layout management (plz don't touch :))
	////////////////////////////////////////////////////////////////

	buttonBox->Add(selectButton, 1, wxALL, BorderWidth);
	buttonBox->Add(beginTestB, 1, wxALL, BorderWidth);

	selectionBox->Add(COMLabel_AVR, 0, wxALIGN_CENTER_HORIZONTAL);
	selectionBox->Add(COMsel_AVR, 0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxLEFT | wxRIGHT, 14);
	selectionBox->Add(COMLabel_STM, 0, wxALIGN_CENTER_HORIZONTAL);
	selectionBox->Add(COMsel_STM, 0, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxLEFT | wxRIGHT, 14);
	selectionBox->Add(tcLabel, 0, wxALIGN_CENTER_HORIZONTAL);
	selectionBox->Add(tcList, 1, wxEXPAND | wxALL, BorderWidth);
	selectionBox->Add(buttonBox, 0,  wxALL | wxALIGN_CENTER , BorderWidth);
	selectionBox->Add(terminalLabel, 0,  wxALL | wxALIGN_LEFT , BorderWidth - 5);
	chipViewBox->Add(p4, 1, wxEXPAND | wxALL, BorderWidth);

	UIBox->Add(selectionBox, 1, wxEXPAND | wxALL, BorderWidth);
	UIBox->Add(chipViewBox, 2, wxEXPAND | (wxALL & ~wxBOTTOM), BorderWidth);

	terminalBox->Add(terminalWindow, 1, wxEXPAND | (wxALL & ~wxTOP), BorderWidth);

	bgBox->Add(UIBox, 0, wxEXPAND | (wxUP | wxRIGHT | wxLEFT), BorderWidth);
	bgBox->Add(terminalBox, 2, wxEXPAND | wxDOWN | wxRIGHT | wxLEFT, BorderWidth);

	background->SetSizer(bgBox);

	SetMenuBar(menu);
	Centre();
	SetMinClientSize(wxSize(400, 280));
	SetMaxClientSize(wxSize(841, 681));

	Bind(wxEVT_BUTTON, &MainWindow::OnTCSelected, this, ID_SELECT_BUTTON);
	Bind(wxEVT_BUTTON, &MainWindow::OnTestBegin, this, ID_BEGIN_TEST_BUTTON);
	Bind(wxEVT_MENU, &MainWindow::OnRefreshCOMtable, this, ID_REFRESH_COM_MENU);
	Bind(wxEVT_MENU, &MainWindow::OnRefreshTCtable, this, ID_REFRESH_TC_MENU);

	RefreshTCtable();
	//RefreshCOMtable();
}

void MainWindow::RefreshTCtable()
{
	const auto& allTcs = TCManager::GetAvailableTests();

	if (allTcs.empty())
	{
		GUIManager::PrintConsoleError("List of test cases is empty :(");
		return;
	}

	tcList->Clear();
	tcList->Append("RUN ALL");

	for (auto& tc : allTcs)
	{
		tcList->Append(tc.first);
	}

	GUIManager::PrintConsoleInfo("Wykryto " + std::to_string(tcList->GetCount() - 1) + (" aktywnych scenariuszy"));
}

void MainWindow::RefreshCOMtable()
{
	std::vector<std::string> COMs2Add{};
	SerialCom::RefreshAvailableCOMPorts(COMs2Add);
	GUIManager::PrintConsoleInfo("Odœwierzono dostêpne porty szeregowe");

	if (COMs2Add.empty())
	{
		GUIManager::PrintConsoleError("Nie znaleziono ¿adnych otwartych portów COM");
		return;
	}

	// Firstly clear current COM list
	COMsel_AVR->Clear();
	COMsel_STM->Clear();

	for (auto& item : COMs2Add)
	{
		COMsel_AVR->AppendString(item);
		COMsel_STM->AppendString(item);
	}
}

void MainWindow::OnRefreshCOMtable(wxCommandEvent& evt)
{
	RefreshCOMtable();
	evt.Skip();
}

void MainWindow::OnRefreshTCtable(wxCommandEvent& evt)
{
	RefreshTCtable();
	evt.Skip();
}

void MainWindow::OnTestBegin(wxCommandEvent& evt)
{
	if (selTestcase.empty())
	{
		GUIManager::PrintConsoleInfo("No test selected (mark test on list and click \"Select\"");
		evt.Skip();
		return;
	}
	GUIManager::PrintConsoleInfo("Running test ->" + selTestcase);
	TCManager::RunTestCase(selTestcase);
	evt.Skip();
}

void MainWindow::OnTCSelected(wxCommandEvent& evt)
{
	selTestcase = tcList->GetStringSelection();

	if (selTestcase.empty())
	{
		GUIManager::PrintConsoleInfo("Select test case on list before clicking \"Select\"");
		evt.Skip();
		return;
	}

	GUIManager::PrintConsoleInfo("Test \"" + selTestcase + "\" selected");
	evt.Skip();
}

void MainWindow::OnQuit(wxCommandEvent& evt)
{
	WXUNUSED(evt);
	Close(true);
}

} // namespace tb
