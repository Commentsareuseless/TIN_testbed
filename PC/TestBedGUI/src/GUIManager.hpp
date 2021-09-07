#pragma once

#include "TestCase.hpp"

#include <string>
#include <functional>
#include <bitset>

namespace tb
{

/*
	Since it is quite inconvenient to separate GUI style from functional
	code in wxWidgets, this class is supposed to be kinda facade over GUI
	If you want to modify sth directly in GUI, use this class istead of including MainWindow
	or wx/wx.h directly
	If there is sth that you want to use and this class does not provide it, plz implement
	this in new method
*/

enum class LogLevel : unsigned
{
	TC_RESULT_ONLY,		/* Print only wheather test passed or failed */
	INFO,				/* Print [INFO] messages, general info, may be in relase version */
	ERR,				/* Print [ERROR] messages, for more precise info about test fail reason */
	DBG,				/* Print [DEBUG] messages, for debugging, turn off for relase */
	NUM_OF_OPTIONS
};

enum class TestResult : unsigned
{
	PASS,
	FAIL
};

enum class ConnectedDevice : unsigned
{
	STM,
	AVR
};

class GUIManager
{
public:
	//////////////////////////////////////////////////////
	// Print to GUI's "console" window
	//////////////////////////////////////////////////////
	static void SetLoggingLevel(const LogLevel llvl, bool val);
	static void PrintConsoleInfo(const std::string& message);
	static void PrintConsoleError(const std::string& message);
	static void PrintConsoleDebug(const std::string& message);
	static void PrintTestState(
		const std::string& message, 
		const TestResult result);
	//////////////////////////////////////////////////////

	static void AddCOMPort(const std::string& name);
	static std::string GetSelectedCOM(ConnectedDevice dev);

	static void AddTestCase(TestCase& tc);
	// TODO: add menu option for this xD
	static void SaveTestLogToFile(const std::string& fileName);
	static void ClearTestCasesTable();

private:
	// Is this even necessary? lul
	static std::bitset<(size_t)LogLevel::NUM_OF_OPTIONS> logLvl;
};

} //namespace tb
