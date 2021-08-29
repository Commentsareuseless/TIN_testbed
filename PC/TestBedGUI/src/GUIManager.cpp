#include "GUIManager.hpp"

// kinda cheaty way to make everything work, functions used by wxWidgets are 
// deprecated thus VS complains about it
#pragma warning(disable : 4996)
#include "MainWindow.hpp"

#include <cstdio>

namespace tb
{
    std::bitset<(size_t)LogLevel::NUM_OF_OPTIONS> GUIManager::logLvl = 0xf;

    // #doinWhatICanNotToIncludeWxWidgetsInHpp
    static void PrintConsole(
        const std::string& message,
        LogLevel llvl,
        const wxColour* color)
    {
        wxRichTextCtrl* terminal = MainWindow::GetInstance()->GetTerminal();
        char logPrefix[20] = {0};
        std::string logLabel;

        switch (llvl)
        {
        case LogLevel::INFO:
            sprintf_s(logPrefix, "[INFO]\t\t");
            break;
        case LogLevel::ERR:
            sprintf_s(logPrefix, "[ERROR]\t");
            break;
        case LogLevel::DEBUG:
            sprintf_s(logPrefix, "[DEBUG]\t");
            break;
        }

        logLabel = logPrefix;

        terminal->SetDefaultStyle(wxTextAttr(*color));
        terminal->WriteText(logLabel);
        terminal->SetDefaultStyle(wxTextAttr(*wxLIGHT_GREY));
        terminal->WriteText(message);
        terminal->Newline();
    }

    void GUIManager::SetLoggingLevel(const LogLevel llvl, bool val)
    {
        logLvl.set((unsigned)llvl, val);
    }

    void GUIManager::PrintConsoleInfo(const std::string& message)
    {
        if (logLvl.test((unsigned)LogLevel::INFO))
            PrintConsole(message, LogLevel::INFO, wxWHITE);
    }

    void GUIManager::PrintConsoleError(const std::string& message)
    {
        if (logLvl.test((unsigned)LogLevel::ERR))
            PrintConsole(message, LogLevel::ERR, wxRED);
    }

    void GUIManager::PrintConsoleDebug(const std::string& message)
    {
        if (logLvl.test((unsigned)LogLevel::DEBUG))
            PrintConsole(message, LogLevel::DEBUG, wxYELLOW);
    }

    void GUIManager::PrintTestState(const std::string& message, const TestResult result)
    {
        wxRichTextCtrl* terminal = MainWindow::GetInstance()->GetTerminal();
        char labelBuff[15] = { 0 };

        if (result == TestResult::PASS)
        {
            sprintf_s(labelBuff, "[PASSED]\t");
            terminal->SetDefaultStyle(wxTextAttr(*wxGREEN));
            terminal->WriteText(labelBuff);
        }
        else
        {
            sprintf_s(labelBuff, "[FAILED]\t");
            terminal->SetDefaultStyle(wxTextAttr(*wxRED));
            terminal->WriteText(labelBuff);
        }

        terminal->SetDefaultStyle(wxTextAttr(*wxLIGHT_GREY));
        terminal->WriteText(message);
        terminal->Newline();
    }

    void GUIManager::AddCOMPort(const std::string& name)
    {
        auto comList = MainWindow::GetInstance()->GetAVRCOMList();

        comList->AppendString(name);
    }

    std::string GUIManager::GetSelectedCOM(ConnectedDevice dev)
    {
        std::string selectedCOM{};
        if (dev == ConnectedDevice::AVR)
        {
            selectedCOM = MainWindow::GetInstance()->GetAVRCOMList()->GetStringSelection();
        }

        if (dev == ConnectedDevice::STM)
        {
            selectedCOM = MainWindow::GetInstance()->GetSTMCOMList()->GetStringSelection();
        }

        if (selectedCOM.empty())
        {
            PrintConsoleError("COM port not selected, default to COM-1"); //idk why lul
            return "COM-1";
        }
        return selectedCOM;
    }

    void GUIManager::AddTestCase(TestCase& tc)
    {
        auto tcList = MainWindow::GetInstance()->GetTCList();
        tcList->Append(tc.name);
    }

    void GUIManager::SaveTestLogToFile(const std::string& fileName)
    {
        auto terminal = MainWindow::GetInstance()->GetTerminal();
        std::string logFileName = fileName + ".log";
        terminal->SaveFile(logFileName);
        PrintConsoleInfo("Saved log to " + logFileName);
    }

    void GUIManager::ClearTestCasesTable()
    {
        auto tcList = MainWindow::GetInstance()->GetTCList();
        tcList->Clear();
    }

} // namespace tb