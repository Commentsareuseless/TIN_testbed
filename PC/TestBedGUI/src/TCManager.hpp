#pragma once

#include "TestCase.hpp"

#include <unordered_map>
#include <vector>
#include <string>

namespace tb 
{

class TCManager
{
public:
	void NewTestCase(
		std::string& tcName,
		tc_callback& tcFunc,
		std::string& tcNameOfAVRhex);

	// This is invoked from MainWindow after pressing "Test!"
	void RunTestCase(const std::string& tcName);
	void RunAllTestCases();

	void TestConsoleOut();

	/*
	* Funkcja do czytania txt �eby zrobi� tc 
	* (mo�e by� w innej klasie czy cu�)
	*/
	void ParseTxt();

private:
	// jakis obiekt przechowuj�cy plick txt odczytany przez Parsetxt()
	// jakisObiekt obiekt{};

	// The optimalest mapa
	std::unordered_map<std::string, TestCase> listOfTCs;
};

} // namespace tb
