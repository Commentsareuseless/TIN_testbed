#pragma once

#include "TestCase.hpp"

#include <string>
#include <vector>

namespace tb
{

class SerialCom
{
public:
	static void RefreshAvailableCOMPorts(std::vector<std::string>& listOfCOMs);
	// static void ProgramAVR(const std::string& hexFile);	// Programming AVR from this 
	// static void ProgramAVR(TestCase& tc);				// app is quite hard :(
	
	/* To use this api, just create localy SerialCom object and 
		invoke functions of your choice :) */
	SerialCom(const std::string& portName);
	~SerialCom();
	bool PingCOM();
	void ReadCOM(std::string& messageBuff);
	void Write2COM(const std::string& message);

private:
	SerialCom(SerialCom&) = delete;	// No copying, One COM = One class instance
	std::string COMPort;
	bool isPortValid = false;
};

}
