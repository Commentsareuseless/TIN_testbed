#include "wx/wx.h"
#include "Launcher.hpp"
#include "GUIManager.hpp"

namespace tb
{

wxIMPLEMENT_APP(Launcher);

bool Launcher::OnInit()
{
	mainFrame = new MainWindow();
	mainFrame->Show();

	managerInst = std::make_shared<GUIManager>(GUIManager());
	managerInst->PrintConsoleInfo("Konsola dzia�a");
	managerInst->PrintConsoleDebug("Debug dzia�a");
	managerInst->PrintConsoleError("Error dzia�a");
	managerInst->PrintTestState("Test state dziala", TestResult::PASS);

	return true;
}

} // namespace tb

