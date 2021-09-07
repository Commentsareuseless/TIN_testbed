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

#include <thread>
#include <cstdint>


namespace tb
{
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


///////////////////////////////////////////////////////////////////////////
// Test case functions 
///////////////////////////////////////////////////////////////////////////

void ExampleTest()
{
	GUIManager::PrintTestState("Przyk³adowy test", TestResult::PASS);
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

	if (stm.empty())
	{
		GUIManager::PrintConsoleInfo("Pinging only avr");
		SerialCom COM(avr);
		if (COM.PingCOM())
		{
			GUIManager::PrintTestState("Succesful ping", TestResult::PASS);
			return;
		}
		GUIManager::PrintTestState("Unsuccesful ping", TestResult::FAIL);
		return;
	}
	if(avr.empty())
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
	constexpr char AVR_SET_PINS_ON_PA[] = { "STPA" };
	constexpr char STM_READ_AVR_PA[] = { "RDPA" };
	constexpr char EXPECTED_BITS_IN_PA[] = { "11111" };

	bool testVerdict{ true };

	std::string messageBuff{};

	SerialCom AVR_COM(GUIManager::GetSelectedCOM(ConnectedDevice::AVR));
	SerialCom STM_COM(GUIManager::GetSelectedCOM(ConnectedDevice::STM));

	AVR_COM.Write2COM(AVR_SET_PINS_ON_PA);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));	// Make sure avr finished (with some BIG margin hehe)
	STM_COM.Write2COM(STM_READ_AVR_PA);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	STM_COM.ReadCOM(messageBuff);

	try
	{
		for (int i{ 0 }; i < messageBuff.size(); ++i)
		{
			if (messageBuff[i] != EXPECTED_BITS_IN_PA[i])
			{
				GUIManager::PrintConsoleError("Unexpected state on pin " + std::to_string(i));
				GUIManager::PrintConsoleInfo("Expected " + EXPECTED_BITS_IN_PA[i]);
				testVerdict = false;
			}
		}
	}
	catch (std::exception& exc) // catch oob exception from string
	{
		GUIManager::PrintTestState("Errors detected " + std::string{exc.what()}, TestResult::FAIL);
		return;
	}
	if (testVerdict)
	{
		GUIManager::PrintTestState("GPIO works", TestResult::PASS);
		return;
	}
	GUIManager::PrintTestState("Errors detected", TestResult::FAIL);
}

void TestAVR()
{
	char message = 0;
	constexpr char WRITE_MASK	= 0b00111111;
	constexpr char PORTB_MASK	= 0b11001111;
	constexpr char SET_MASK		= 0b00001000;
	constexpr char RES_MASK		= 0b11110111;
	constexpr char PORTX_MASK	= 0b11111000;

	SerialCom AVR_COM(GUIManager::GetSelectedCOM(ConnectedDevice::AVR));

	// #1 message -> set pin 1 of PB
	message = (WRITE_MASK & PORTB_MASK) | (SET_MASK) | (~PORTX_MASK) & 0x00;
	AVR_COM.WriteByte(message);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	GUIManager::PrintConsoleInfo("Sending message: " + std::to_string(message));

	//for (uint8_t i{ 1 }; i < 0x7; ++i)
	//{
	//	// Set next pin 
	//	message = (WRITE_MASK & PORTB_MASK) | (SET_MASK) | (~PORTX_MASK) & i;
	//	AVR_COM.WriteByte(message);
	//	GUIManager::PrintConsoleInfo("Sending message: " + std::to_string(message));
	//	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	//	// reset prev pin
	//	message = (WRITE_MASK & PORTB_MASK & RES_MASK) & ((~PORTX_MASK) & ~(i - 1));
	//	AVR_COM.WriteByte(message);
	//	GUIManager::PrintConsoleInfo("Sending message: " + std::to_string(message));
	//	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	//}
	char buf = 0;
	for (uint8_t i{ 0 }; i < 0xf; ++i)
	{
		message = i;
		AVR_COM.WriteByte(message);
		GUIManager::PrintConsoleInfo("Sending message: " + std::to_string(message));
		AVR_COM.ReadByte(&buf);
		GUIManager::PrintConsoleInfo("Recvd message: " + std::to_string(buf));
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	GUIManager::PrintTestState("Nie wiem czy dzia³ bo nie odpowiada hehe", TestResult::PASS);
}

///////////////////////////////////////////////////////////////////////////
// Table of all test cases used by TCManager 
///////////////////////////////////////////////////////////////////////////

TCMap TCManager::listOfTCs{
	{"ExampleTest", TestCase("ExampleTest", ExampleTest)},
	{"TestSerialCommunication", TestCase("TestSerialCommunication", TestSerialCommunication)},
	{"TestGPIO", TestCase("TestGPIO", TestGPIO)},
	{"TestAVR", TestCase("TestGPIO", TestAVR)}

};

} // namespace tb
