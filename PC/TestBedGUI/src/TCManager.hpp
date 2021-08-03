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

	void RunTestCase(const std::string& tcName);
	void RunAllTestCases();

	void TestConsoleOut();

	/*
	* Funkcja do czytania txt �eby zrobi� tc 
	* (mo�e by� w innej klasie czy cu�)
	*/

private:
	// The optimalest mapa
	std::unordered_map<std::string, TestCase> listOfTCs;
};

} // namespace tb
