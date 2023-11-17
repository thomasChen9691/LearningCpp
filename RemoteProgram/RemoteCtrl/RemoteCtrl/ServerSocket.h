#pragma once
#include "pch.h"
#include "ServerSocket.h"
class CServerSocket
{
public:
	static CServerSocket* getInstace(){
		if (m_instance == NULL) {
			m_instance == new CServerSocket();
		}//static value no this pointer, so cannot get variables
		return m_instance;
	}
	bool InitSocket() {
		
		if (m_sock == -1) return -1;
		//TODO: Check
		sockaddr_in serv_adr;
		memset(&serv_adr, 0, sizeof(serv_adr));
		serv_adr.sin_family = AF_INET;
		serv_adr.sin_addr.s_addr = INADDR_ANY;
		serv_adr.sin_port = htons(9527);
		//bind
		if (bind(m_sock, (sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) return false;//TODO
		//TODO
		if(listen(m_sock, 1)==-1) return false;
		return true;
	}

	bool acceptClient() {
		sockaddr_in client_adr;
		
		int cli_sz = sizeof(client_adr);
		m_client = accept(m_sock, (sockaddr*)&client_adr, &cli_sz);
		if (m_client == -1) return false;
		return true;
		//
		//
		
	}

	int dealCommand() {
		if (m_client == -1) return false;
		char buffer[1024]="";
		while (true)
		{
			int len = recv(m_client, buffer, sizeof(buffer), 0);
			if (len <= 0) {
				return -1;
			}
		}
	}

	bool Send(const char* pData, int nSize) {
		return send(m_client, pData, nSize, 0) > 0;
	}

private:
	SOCKET m_sock;
	SOCKET m_client;
	CServerSocket& operator=(const CServerSocket& ss){}
	CServerSocket(const CServerSocket& ss) {
		m_sock = ss.m_sock;
		m_client = ss.m_client;
	}
	CServerSocket(){
		m_client = INVALID_SOCKET;//-1
		if (InitSockEnv() == FALSE) {
			MessageBox(NULL, _T("Initial environment error! Check networking setting"), _T("Initial error!"), MB_OK | MB_ICONERROR);
			exit(0);
		}
		m_sock = socket(PF_INET, SOCK_STREAM, 0);
	}
	~CServerSocket(){
		closesocket(m_sock);
		WSACleanup();
	}
	BOOL InitSockEnv() {
		WSADATA data;
		if (WSAStartup(MAKEWORD(1, 1), &data) != 0) {
			return FALSE;
		} // TODO: return value
		return TRUE;
	}
	static void releaseInstance() {
		if (m_instance!=NULL)
		{
			CServerSocket* temp = m_instance;
			m_instance = NULL;
			delete temp;
		}
	}
	static CServerSocket* m_instance;
	class CHelper {
	public:
		CHelper() {
			CServerSocket::getInstace();
		}
		~CHelper() {
			CServerSocket::releaseInstance();
		}
	};
	static CHelper m_helper;
};

//extern CServerSocket server;
