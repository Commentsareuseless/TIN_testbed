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
	SerialCom AVR_COM(GUIManager::GetSelectedCOM(ConnectedDevice::AVR));

	AVR_COM.PingCOM();
}

void TestGPIO()
{
	uint8_t controlByte = 0;
	uint8_t dataByte = 0xff;

	std::string message;
	message.reserve(2);

	SerialCom AVR_COM(GUIManager::GetSelectedCOM(ConnectedDevice::AVR));

	// Set message type to GPIO operation
	controlByte |= GPIO;
	// Select port A
	controlByte |= PORTA;
	// Write flag ON
	controlByte |= WRITE;

	message[0] = controlByte;
	message[1] = dataByte;

	GUIManager::PrintConsoleInfo("Writting to AVR " + (char)controlByte + (char)dataByte);
	AVR_COM.Write2COM(message);

	GUIManager::PrintTestState("Idk if works hehe", TestResult::PASS);
}

///////////////////////////////////////////////////////////////////////////
// Table of all test cases used by TCManager 
///////////////////////////////////////////////////////////////////////////

TCMap TCManager::listOfTCs{
	{"ExampleTest", TestCase("ExampleTest", ExampleTest)},
	{"TestSerialCommunication", TestCase("TestSerialCommunication", TestSerialCommunication)},
	{"TestGPIO", TestCase("TestGPIO", TestGPIO)}
};

} // namespace tb
