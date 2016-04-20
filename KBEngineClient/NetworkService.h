//
// Copyright (C) Mei Jun 2011
//

#pragma once

#include <WinSock2.h>
#include <MSWSock.h>

#include <memory>
#include <queue>
#include <string>
#include <stack>
#include <list>

#define MAX_BUFFER_SIZE	 4096
#define MAX_PACKAGE_SIZE 1024

struct HANDLE_DATA
{
	SOCKET s = NULL;
	DWORD  dwIOSend = 0;
	DWORD  dwIORecv = 0;
	DWORD  dwSizeSend = 0;
	DWORD  dwSizeRecv = 0;
	BYTE   BufSend[MAX_BUFFER_SIZE];
	BYTE   BufRecv[MAX_BUFFER_SIZE];
	std::shared_ptr<std::queue<std::string> > listRecv =
		std::make_shared<std::queue<std::string> >();
};

struct IO_DATA
{
#define OP_ACCEPT	1
#define	OP_READ		2
#define OP_WRITE	3
#define OP_CLOSE	4
	OVERLAPPED ol;
	SOCKET	   s;
	DWORD	   dwFlags;
	DWORD	   op;
	DWORD	   dwBufSize;
	BYTE	   BufPackage[MAX_PACKAGE_SIZE];
};

class NetworkService
{
public:
	HANDLE		m_hIOCP = NULL;
	BOOL		m_bServer = FALSE;

	BOOL		m_bInWorking = FALSE;
	BOOL		m_bWorkingEnd = FALSE;

	SOCKET		m_sServer = 123;
	SOCKET		m_sListen = NULL;

	std::shared_ptr<HANDLE_DATA> m_ServerHandleData;
	std::list<std::shared_ptr<HANDLE_DATA> > m_lstHandleData;

	std::list<std::shared_ptr<IO_DATA> > m_lstIoDatasUsed;
	std::stack<std::shared_ptr<IO_DATA> > m_lstIoDatasUnused;

	std::shared_ptr<CRITICAL_SECTION> cs = std::make_shared<CRITICAL_SECTION>();

public:
	NetworkService( BYTE minorVer = 2, BYTE majorVer = 2 );
	~NetworkService(void);

private:
	static VOID WINAPI ThreadServerWorker( LPVOID lParam );

	static VOID WINAPI ThreadClientWorker( LPVOID lParam );

	IO_DATA* GetIOData();

	VOID ReleaseIOData( IO_DATA* pData );
 
	BOOL CreateServer( const char* addr, short port );

	BOOL CreateClient( const char* addr, short port );

	VOID PostAccept( SOCKET s );

	VOID PostSend( SOCKET s, char* pDataBuf, DWORD dwDataSize );

	VOID PostRecv( SOCKET s );

	void RemoveHandleData(HANDLE_DATA* pData);

public:
	BOOL Start(const char* addr, short port, BOOL bServer = TRUE);

	VOID SendMsg( char* pMsg, unsigned int dwSize );

	void RecvMsg(std::string& msg);
};

