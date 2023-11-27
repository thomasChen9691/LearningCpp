#pragma once
#include "pch.h"
#include "ServerSocket.h"


#pragma pack(push)
#pragma pack(1)

using namespace std;

class CPacket
{
public:
	CPacket():sHead(0), nLength(0), sCmd(0), sSum(0){}
	CPacket(WORD nCmd, const BYTE* pData, size_t nSize) {
		sHead = 0xFEFF;
		nLength = nSize + 4;
		sCmd = nCmd;
		strData.resize(nSize);
		memcpy((void*)strData.c_str(), pData, nSize);
		sSum = 0;
		for (size_t j = 0; j < strData.size(); j++)
		{
			sSum += BYTE(strData[j]) & 0xFF;
		}
	}
	CPacket(const CPacket& pack){
		sHead = pack.sHead;
		nLength = pack.nLength;
		sCmd = pack.sCmd;
		sSum = pack.sSum;
		strData = pack.strData;
	}
	CPacket(const BYTE* pData, size_t& nSize){
		size_t i = 0;
		for (; i < nSize; i++)
		{

			if (*(WORD*)(pData + i) == 0xFEFF) {
				sHead = *(WORD*)(pData + i);
				i += 2;
				break;
			}
		}

		if (i + 4 + 2 + 2 > nSize) {//4---nLength , 2---sCmd , 2---sSum package data may not full,or not accpet full package head 
			nSize = 0;
			return;
		}
		nLength = *(WORD*)(pData + i); i += 4;
		if (nLength + i > nSize) {//not accept full package 
			nSize = 0;
			return;
		}
		sCmd = *(WORD*)(pData + i); i += 2;
		if (nLength > 4)
		{
			strData.resize(nLength - 2 - 2);//2---sCmd , 2---sSum
			memcpy((void*)strData.c_str(), pData + i, nLength - 4);
			i += nLength - 4;
		}
		sSum = *(WORD*)(pData + i); i += 2;
		WORD sum = 0;
		for (size_t j = 0; j < strData.size(); j++)
		{
			sum += BYTE(strData[j]) & 0xFF;
		}
		if (sum == sSum)
		{
			nSize = i;// sHead---2 nLength---4 pData....
			return;
		}
		nSize = 0;
	}
	~CPacket(){}
	CPacket& operator=(const CPacket& pack){
		if (this != & pack)
		{
			sHead = pack.sHead;
			nLength = pack.nLength;
			sCmd = pack.sCmd;
			sSum = pack.sSum;
			strData = pack.strData;
		}
		return *this;
	}

	int Size() {//package data size
		return nLength + 6;
	}

	const char* Data() {
		strOut.resize(nLength + 6);
		BYTE* pData = (BYTE*)strOut.c_str();
		*(WORD*)pData = sHead; pData += 2;
		*(DWORD*)pData = nLength; pData += 4;
		*(WORD*)pData = sCmd; pData += 2;
		memcpy(pData, strData.c_str(), strData.size()); pData += strData.size();

		*(WORD*)pData = sSum;
		return strOut.c_str();

	}

public:
	WORD sHead;//fixed position FEFF
	DWORD nLength;//package length(from sCmd to sSum)
	WORD sCmd;//command
	WORD sSum;//sum check
	string strData;//package data
	string strOut;//whole package data;

private:

};





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
		
	}
#define BUFFER_SIZE 4096
	int dealCommand() {
		if (m_client == -1) return -1;
		//char buffer[1024]="";
		char* buffer = new char[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		size_t index = 0;
		while (true)
		{
			size_t len = recv(m_client, buffer+index, BUFFER_SIZE - index, 0);
			if (len <= 0) {
				return -1;
			}
			index += len;//maybe 2000
			len = index;
			m_packet = CPacket((BYTE*)buffer, len);//maybe receive 1000 per time
			if (len>0)
			{
				memmove(buffer, buffer + len, BUFFER_SIZE - len);//move back to front
				index -= len;// 1000
				return m_packet.sCmd;
			}
		}
		return -1;
	}

	bool Send(const char* pData, int nSize) {
		return send(m_client, pData, nSize, 0) > 0;
	}
	bool Send( CPacket& pack) {
		if (m_client == -1) return false;
		return send(m_client, pack.Data(), pack.Size(), 0) > 0;
	}

private:
	SOCKET m_sock;
	SOCKET m_client;
	CPacket m_packet;
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

extern CServerSocket server;
