#include "SerialCom.hpp"

#include "../ext/enumser/stdafx.h"
#include "../ext/enumser/enumser.h"

#include "../ext/serial-master/ceSerial.h"

#include "GUIManager.hpp"

#include <cstdio>
#include <memory>
#include <unordered_map>
#include <stdio.h>

namespace tb
{
    static constexpr char PING_REQ[]    = "PING";
    static constexpr char PING_RESP[]   = "PONG";

    static std::unordered_map<std::string, std::unique_ptr<ce::ceSerial>> serialComInstanceList;

static void SygnalizeError(const std::string& errMessage, HRESULT errCode)
{
    GUIManager::PrintConsoleError(errMessage + " Error code: " + std::to_string(errCode));
}

void SerialCom::RefreshAvailableCOMPorts(std::vector<std::string>& listOfCOMs)
{
    CEnumerateSerial::CPortsArray ports;

    if (CEnumerateSerial::UsingCreateFile(ports))
    {
        for (auto item : ports)
        {
            listOfCOMs.push_back("COM" + std::to_string(item));
        }
    }
    else
    {
        SygnalizeError("CEnumerateSerial::UsingCreateFile failed", GetLastError());
    }
}

SerialCom::SerialCom(const std::string& portName)
    : COMPort(portName)
{
    if (portName == "COM-1")
    {
        GUIManager::PrintConsoleError("Invalid COM port");
        isPortValid = false;
        return;
    }
    //char buff[20] = {};
    //sprintf_s(buff, "\\\\.\\%s", portName.c_str());
    //std::string sPort{ "\\\\.\\COM3" };
    //GUIManager::PrintConsoleError("Port " + sPort);
    //serialComInstanceList[COMPort] = std::make_unique<CSerialCommHelper>();
    //auto& COMInst = serialComInstanceList[COMPort];

 /*   FILE* comP = NULL;
    comP = fopen(sPort.c_str(), "r+");
    if (comP == NULL)
    {
        GUIManager::PrintConsoleError("Sth went wrong during port creation, with error " + std::to_string(GetLastError()));
        return;
    }*/
    /*GUIManager::PrintConsoleInfo("Dzia³a lul");
    fclose(comP);
    return;*/


    //HANDLE hCom;
    //hCom = ::CreateFile((LPCWSTR)sPort.c_str(),
    //    GENERIC_READ | GENERIC_WRITE,//access ( read and write)
    //    0,	//(share) 0:cannot share the COM port						
    //    nullptr,	//security  (None)				
    //    OPEN_EXISTING,// creation : open_existing
    //    0,//FILE_FLAG_OVERLAPPED,// we want overlapped operation
    //    nullptr// no templates file for COM port...
    //);

    //if (hCom == INVALID_HANDLE_VALUE)
    //{
    //    GUIManager::PrintConsoleError("Sth went wrong during port creation, with error " + std::to_string(GetLastError()));
    //}
    //    isPortValid = false;
    //return;
    /*if (COMInst->Init(sPort) != S_OK)
    {
        GUIManager::PrintConsoleError("Sth went wrong during port creation, with error " + std::to_string(GetLastError()));
        isPortValid = false;
        return;
    }
    COMInst->Start();
    */

    char buff[20] = {};
    sprintf_s(buff, "\\\\.\\%s", portName.c_str());
    std::string sPort{ buff };
    GUIManager::PrintConsoleError("Port " + sPort);
    serialComInstanceList[COMPort] = std::make_unique<ce::ceSerial>(sPort, 9600, 8, 'N', 1);

    isPortValid = true;
    GUIManager::PrintConsoleInfo("Created new serial interface on " + COMPort);
}

SerialCom::~SerialCom()
{
    if (isPortValid)
    {
        auto& COMInst = serialComInstanceList[COMPort];
        COMInst->Close();
        COMInst.release();
        GUIManager::PrintConsoleInfo("Relased serial interface on " + COMPort);
    }
}

bool SerialCom::PingCOM()
{
    std::string buff;
    auto& COMInst = serialComInstanceList[COMPort];

    GUIManager::PrintConsoleInfo("Pinging port " + COMPort);

    if (!COMInst->IsOpened())
    {
        COMInst->Open();
    }

    Write2COM(PING_REQ);
    Sleep(100); // wait for uC
    ReadCOM(buff);

    GUIManager::PrintConsoleDebug("On COM, got " + buff);

    if (buff == PING_RESP)
    {
        GUIManager::PrintConsoleInfo("Found valid device on " + COMPort);
        return true;
    }

    if (buff.empty())
    {
        GUIManager::PrintConsoleError("No device found on " + COMPort);
    }
    else
    {
        GUIManager::PrintConsoleError("Invalid device on " + COMPort);
    }
    return false;
}

void SerialCom::ReadCOM(std::string& messageBuff)
{
    auto& COMInst = serialComInstanceList[COMPort];
    bool succeededRead = true;
    int i = 0;
    char buff[40] = {0};

    //messageBuff.reserve(20); // wild guess xD
    if (!COMInst)
    {
        GUIManager::PrintConsoleError("Port communication uninitialized");
        return;
    }
    
    do{
        buff[i] = COMInst->ReadChar(succeededRead);
        ++i;
    } while (succeededRead && i < 40);
    buff[i - 1] = '\0';     // do not read last byte bcs its garbage :)
    messageBuff = std::move(std::string{ buff });
}

void SerialCom::Write2COM(const std::string& message)
{
    auto& COMInst = serialComInstanceList[COMPort];
    if (!COMInst)
    {
        GUIManager::PrintConsoleError("Port communication uninitialized");
        return;
    }

    COMInst->Write((char*)message.c_str(), message.size());
}

}