/*
	Here should be defined all test cases as a separate functions 

	@HOW TO CREATE TEST CASE
	1) Write test function (below apropriate comment)
		-Function must return void and have 0 parameters (type void())
	2) Add name and TestCase object to static map on 
		the bottom of this file
	
	TODO: Maybe one day we should compile it as .dll?
*/
#include "TCManager.hpp"
#include "GUIManager.hpp"
#include "SerialCom.hpp"

#include "../ext/serial-master/ceSerial.h"

#include <thread>
#include <cstdint>


namespace tb
{

	constexpr uint8_t cmd_set = (1 << 3);
	constexpr uint8_t cmd_rst = ~(1 << 3);
	constexpr uint8_t cmd_PORTB = 0b00000000;
	constexpr uint8_t cmd_PORTC = 0b00010000;
	constexpr uint8_t cmd_PORTD = 0b00100000;
	constexpr uint8_t cmd_PORTE = 0b00110000;
	constexpr uint8_t cmd_WRITE = 0b01000000;

enum TypeOfTestMASKS : uint8_t
{
	GPIO= 0b00010000,
	I2C	= 0b00100000,
	SPI	= 0b00110000,
	ADC	= 0b01000000
};

enum SelectedPortMASKS : uint8_t
{
	PORTA = 0b00000010,
	PORTB = 0b00000100,
	PORTC = 0b00000110,
	PORTD = 0b00001000,
	PORTE = 0b00001010
};

enum RWFlagMASK : uint8_t
{
	READ	= 0x00,
	WRITE	= 0x01
};
///////////////////////////////////////////////////////////////////////////
// Helper functions 
///////////////////////////////////////////////////////////////////////////

void TestPortW(uint8_t portMask, uint8_t startPin, uint8_t numOfPins,
				SerialCom* AVR, uint8_t SRFlag = 0)
{
	uint8_t command, message;
	command = 0;
	command = portMask | cmd_WRITE;
	for (uint8_t i{ startPin }; i < startPin + numOfPins; ++i)
	{
		message = command | i;

		AVR->WriteByte(message | SRFlag);
		//GUIManager::PrintConsoleInfo("Sending message: " + std::to_string(message | SRFlag));
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

///////////////////////////////////////////////////////////////////////////
// Test case functions 
///////////////////////////////////////////////////////////////////////////

void ExampleTest()
{
	GUIManager::PrintTestState("Przyk�adowy test", TestResult::PASS);
}

void TestSerialCommunication()
{
	std::string avr{ GUIManager::GetSelectedCOM(ConnectedDevice::AVR) };
	std::string stm{ GUIManager::GetSelectedCOM(ConnectedDevice::STM) };

	if (avr == stm)
	{
		GUIManager::PrintConsoleInfo("Specified 2 same ports, pinging once");
		SerialCom COM(avr);
		if (COM.PingCOM())
		{
			GUIManager::PrintTestState("Succesful ping", TestResult::PASS);
			return;
		}
		GUIManager::PrintTestState("Unsuccesful ping", TestResult::FAIL);
		return;
	}

	if (stm == "COM-1")
	{
		GUIManager::PrintConsoleInfo("Pinging only avr");
		SerialCom COM(avr);
		COM.EnableDTR();
		if (COM.PingCOM())
		{
			GUIManager::PrintTestState("Succesful ping", TestResult::PASS);
			return;
		}
		GUIManager::PrintTestState("Unsuccesful ping", TestResult::FAIL);
		return;
	}
	if(avr == "COM-1")
	{
		GUIManager::PrintConsoleInfo("Pinging only stm");
		SerialCom COM(stm);
		if (COM.PingCOM())
		{
			GUIManager::PrintTestState("Succesful ping", TestResult::PASS);
			return;
		}
		GUIManager::PrintTestState("Unsuccesful ping", TestResult::FAIL);
		return;
	}

	SerialCom STM_COM(stm);
	SerialCom AVR_COM(avr);

	if (STM_COM.PingCOM() && AVR_COM.PingCOM())
	{
		GUIManager::PrintTestState("Succesful ping", TestResult::PASS);
		return;
	}

	GUIManager::PrintTestState("Unsuccesful ping", TestResult::FAIL);
}

void TestGPIO()
{
	constexpr char STM_READ_AVR_AL[] = { "RDAL" };
	constexpr char EXPECTED_BITS_IN_PB[] = { "111111" };
	constexpr char EXPECTED_BITS_IN_PC[] = { "111111" };
	constexpr char EXPECTED_BITS_IN_PD[] = { "00111111" };
	constexpr char EXPECTED_BITS_IN_AL[] = { "111111" "111111" "00111111""0000" };


	bool testVerdict{ true };

	std::string messageBuff{};

	std::string avr{ GUIManager::GetSelectedCOM(ConnectedDevice::AVR) };
	std::string stm{ GUIManager::GetSelectedCOM(ConnectedDevice::STM) };

	if (avr == "COM-1" || stm == "COM-1")
	{
		GUIManager::PrintConsoleError("Invalid coms");
	}
	//*
	SerialCom AVR_COM(avr);
	AVR_COM.EnableDTR();


	TestPortW(cmd_PORTB, 0, 6, &AVR_COM, cmd_set);
	TestPortW(cmd_PORTC, 0, 6, &AVR_COM, cmd_set);
	TestPortW(cmd_PORTD, 2, 6, &AVR_COM, cmd_set);
	//testVerdict = true;
	SerialCom STM_COM(stm);

//*
	STM_COM.Write2COM(STM_READ_AVR_AL);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	STM_COM.ReadCOM(messageBuff);
	GUIManager::PrintConsoleInfo("Got message " + messageBuff);
	try
	{
		for (int i{ 0 }; i < messageBuff.size(); ++i)
		{
			if (messageBuff[i] != EXPECTED_BITS_IN_AL[i])
			{
				GUIManager::PrintConsoleError("Unexpected state on pin " + std::to_string(i));
				GUIManager::PrintConsoleInfo("Expected " + EXPECTED_BITS_IN_AL[i]);
				testVerdict = false;
			}
		}
	}
	catch (std::exception& exc) // catch oob exception from string
	{
		GUIManager::PrintTestState("Errors detected " + std::string{exc.what()}, TestResult::FAIL);
		return;
	}//*/
	if (testVerdict)
	{
		GUIManager::PrintTestState("GPIO works", TestResult::PASS);
		return;
	}
	GUIManager::PrintTestState("Errors detected", TestResult::FAIL);
}


void TestAVR()
{
	constexpr uint8_t g_Command = 0;
	constexpr uint8_t g_typeOfCommand = (g_Command >> 6);
	constexpr uint8_t g_gpioPort = ((g_Command >> 4) & 3);
	constexpr uint8_t g_gpioSR = ((g_Command >> 3) | 1); //1 = set; 0 = reset;
	constexpr uint8_t g_gpioPin = (g_Command & 7);


	uint8_t command = 0;

	std::string avr = GUIManager::GetSelectedCOM(ConnectedDevice::AVR);
	if (avr.empty())
	{
		GUIManager::PrintConsoleError("Wybierz port!");
		return;
	}

	SerialCom AVR(avr);
	AVR.EnableDTR(); // avr works with DTR enabled
	char buf = 0;
	char message = 0;
	bool operationSuccesful{ true };

	// Iterate PORTB
	command = 0;
	command = cmd_PORTB | cmd_WRITE;
	for (uint8_t i{ 0 }; i < 0x6; ++i)
	{
		message = command | i;

		GUIManager::PrintConsoleInfo("Sending message: " + std::to_string(message));
		AVR.WriteByte(message | cmd_set);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		AVR.WriteByte(message);
	}

	// Iterate PORTC
	command = 0;
	command = cmd_PORTC | cmd_WRITE;
	for (uint8_t i{ 0 }; i < 0x6; ++i)
	{
		message = command | i;

		GUIManager::PrintConsoleInfo("Sending message: " + std::to_string(message));
		AVR.WriteByte(message | cmd_set);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		AVR.WriteByte(message);
	}

	// Iterate PORTD
	command = 0;
	command = cmd_PORTD | cmd_WRITE;
	for (uint8_t i{ 2 }; i < 0x8; ++i)
	{
		message = command | i;

		GUIManager::PrintConsoleInfo("Sending message: " + std::to_string(message));
		AVR.WriteByte(message | cmd_set);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		AVR.WriteByte(message);
	}

	/*
	for (uint8_t i{ 1 }; i < 0xf; ++i)
	{
		message = i;

		GUIManager::PrintConsoleInfo("Sending message: " + std::to_string(message));
		AVR.WriteByte(message);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		AVR.ReadByte(buf);
		GUIManager::PrintConsoleInfo("Recvd message: " + std::to_string(buf));
	}
	*/
	GUIManager::PrintTestState("Works :)", TestResult::PASS);
}

///////////////////////////////////////////////////////////////////////////
// Table of all test cases used by TCManager 
///////////////////////////////////////////////////////////////////////////

TCMap TCManager::listOfTCs{
	{"ExampleTest", TestCase("ExampleTest", ExampleTest)},
	{"TestSerialCommunication", TestCase("TestSerialCommunication", TestSerialCommunication)},
	{"TestGPIO", TestCase("TestGPIO", TestGPIO)},
	{"TestAVR", TestCase("TestAVR", TestAVR)}
};

} // namespace tb
