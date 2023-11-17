// RemoteCtrl.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "RemoteCtrl.h"
#include "ServerSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

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
