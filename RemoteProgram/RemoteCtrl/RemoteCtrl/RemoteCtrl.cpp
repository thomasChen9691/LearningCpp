// RemoteCtrl.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "RemoteCtrl.h"
#include "ServerSocket.h"
#include <direct.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*
#pragma comment( linker, "/subsystem:windows /entry:WinMainCRTStartup")
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup")
#pragma comment( linker, "/subsystem:console /entry:mainCRTStartup")
#pragma comment( linker, "/subsystem:console /entry:WinMainCRTStartup")
*/

// The one and only application object

CWinApp theApp;

using namespace std;
void Dump(BYTE* pData, size_t nSize) {
    string strOut;
    for (size_t i = 0; i < nSize; i++)
    {
        char buf[8] = "";
        if (i >0&& (i%16==0))
        {
            strOut += "\n";
        }
        snprintf(buf, sizeof(buf), "%02X", pData[i] & 0xFF);
        strOut += buf;
    }
    strOut += "\n";
    OutputDebugStringA(strOut.c_str());
}

int MakeDriverInfo() {//1==>A 2==>B
    string result;
    for (int i = 1; i <= 26; i++)
    {
        if (_chdrive(i) == 0) {//==0 change success, change different driver
            if (result.size()>0)
            {
                result += ',';
            }
            result += 'A' + i - 1; //get one drive 
        }
    }
    CPacket pack(1, (BYTE*)result.c_str(), result.size());
    Dump((BYTE*)pack.Data(), pack.Size());

    //CServerSocket::getInstace()->Send(pack);
    return 0;
}

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: code your application's behavior here. socket, bind, listen, accept, read, write, close
            CServerSocket* pserver = CServerSocket::getInstace();
            int count = 0;
            while (CServerSocket::getInstace() !=NULL)
            {
                if (pserver->InitSocket() == false) {
                    MessageBox(NULL, _T("Initial network error!"), _T("Check networking setting"), MB_OK | MB_ICONERROR);
                }
                if (pserver->acceptClient() == false) {
                    if (count>=3)
                    {
                        MessageBox(NULL, _T("try to accept network 3 times"), _T("end program"), MB_OK | MB_ICONERROR);
                    }
                    MessageBox(NULL, _T("accept network error!"), _T(" Check accept setting"), MB_OK | MB_ICONERROR);
                    count++;
                }
                int ret = pserver->dealCommand();

            }
            int nCmd = 1;
            switch (nCmd)
            {
            case 1://check driver
                MakeDriverInfo();
                break;
            default:
                break;
            }

            
         }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
