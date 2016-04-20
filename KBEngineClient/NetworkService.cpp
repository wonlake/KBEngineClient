//
// Copyright (C) Mei Jun 2011
//

#include "NetworkService.h"
#include <sstream>
#include <algorithm>
#include <iostream>

#pragma comment( lib, "MSWSock.lib" )
#pragma comment( lib, "WS2_32.lib" )

NetworkService::NetworkService( BYTE minorVer, BYTE majorVer )
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(minorVer, majorVer);
	if( WSAStartup(sockVersion,&wsaData) != 0 )
	{
		ExitProcess( -1 );
	}

	InitializeCriticalSection(cs.get());
}


NetworkService::~NetworkService(void)
{
	if (m_bInWorking)
	{
		PostQueuedCompletionStatus(m_hIOCP, 0, 0, NULL);
	}

	while(m_bInWorking && !m_bWorkingEnd)
	{
		Sleep(1);
	}
	WSACleanup();
}

BOOL NetworkService::Start( const char* addr, short port, BOOL bServer  )
{ 
	if( bServer )
		return CreateServer( addr, port );
	else
		return CreateClient( addr, port );
}

BOOL NetworkService::CreateServer( const char* addr, short port )
{
	m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, NULL, 0 );
	if( m_hIOCP == NULL )
	{
		std::ostringstream ss;
		ss << "完成端口创建失败:" << __FILE__ << "," << __LINE__ << std::endl;
		std::cout << ss.str();
		return FALSE;
	}

	SOCKET sListen;
	sListen = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED );
	m_sListen = sListen;

	SOCKADDR_IN si;
	si.sin_family	   = AF_INET;
	si.sin_port		   = htons(port);
	si.sin_addr.s_addr = inet_addr( addr );
	if( bind(sListen, (sockaddr*)&si, sizeof(si) ) != 0 )
	{
		std::ostringstream ss;
		ss << "端口绑定失败 " << __FILE__ << ", " << __LINE__ << std::endl;
		std::cout << ss.str();
		closesocket( sListen );
		return FALSE;
	}
	listen( sListen, 2 );

	m_lstHandleData.push_back(std::make_shared<HANDLE_DATA>());
	HANDLE_DATA* pListen = m_lstHandleData.back().get(); 

	pListen->s = sListen;
	CreateIoCompletionPort( (HANDLE)pListen->s, m_hIOCP,
		(ULONG_PTR)pListen, 0 );

	PostAccept( sListen );

	DWORD dwThreadId = 0;
	CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadServerWorker,
		(LPVOID)this, 0, &dwThreadId );
	return TRUE;
}

BOOL NetworkService::CreateClient( const char* addr, short port )
{
	m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, NULL, 0 );
	if( m_hIOCP == NULL )
	{
		std::ostringstream ss;
		ss << "完成端口创建失败:" << __FILE__ << "," << __LINE__ << std::endl;
		std::cout << ss.str();
		return FALSE;
	}

	SOCKET sServer;
	sServer = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED );

	SOCKADDR_IN si;
	si.sin_family	   = AF_INET;
	si.sin_port		   = htons( port );
	si.sin_addr.s_addr = inet_addr( addr );

	if( connect(sServer, (sockaddr*)&si, sizeof(si) ) != 0 )
	{
		std::ostringstream ss;
		ss << "无法连接服务器:" << __FILE__ << "," << __LINE__ << std::endl;
		std::cout << ss.str();
		closesocket( sServer );
		return FALSE;
	}

	m_sServer	   = sServer;

	m_lstHandleData.push_back(std::make_shared<HANDLE_DATA>());
	m_ServerHandleData = m_lstHandleData.back();
	HANDLE_DATA* pServer = m_lstHandleData.back().get();

	pServer->s = sServer;
	CreateIoCompletionPort( (HANDLE)pServer->s, m_hIOCP,
		(ULONG_PTR)pServer, 0 );

	PostRecv( pServer->s );
	DWORD dwThreadId = 0;
	CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadClientWorker,
		(LPVOID)this, 0, &dwThreadId );

	return TRUE;
}

VOID NetworkService::PostAccept( SOCKET s)
{
	IO_DATA* pData = GetIOData();
	pData->s = socket( AF_INET, SOCK_STREAM, 0 );
	pData->op = OP_ACCEPT;
	AcceptEx( s, pData->s, pData->BufPackage, 0, 
		sizeof(sockaddr) + 16, sizeof(sockaddr) + 16, 
		&pData->dwBufSize, &pData->ol );
}

VOID NetworkService::PostSend( SOCKET s, char* pDataBuf, DWORD dwDataSize )
{
	IO_DATA* pData = GetIOData();
	WSABUF WSABuf;
	WSABuf.buf	= pDataBuf;
	WSABuf.len	= dwDataSize;
	pData->op	= OP_WRITE;
	WSASend( s, &WSABuf, 1, &pData->dwBufSize, pData->dwFlags, &pData->ol, NULL );
}

VOID NetworkService::SendMsg( char* pMsg, unsigned int dwSize )
{
	if( m_sServer )
	{		
		PostSend( m_sServer, pMsg, dwSize );
	}
}

void NetworkService::RecvMsg(std::string& msg)
{
	if (TryEnterCriticalSection(cs.get()))
	{
		if (!m_ServerHandleData->listRecv->empty())
		{
			msg = std::move(m_ServerHandleData->listRecv->front());
			m_ServerHandleData->listRecv->pop();
		}
		LeaveCriticalSection(cs.get());
	}
}

VOID NetworkService::PostRecv( SOCKET s )
{
	IO_DATA* pData = GetIOData();
	WSABUF WSABuf;
	WSABuf.buf	= (CHAR*)pData->BufPackage;
	WSABuf.len	= sizeof(pData->BufPackage);
	pData->op	= OP_READ;
	WSARecv( s, &WSABuf, 1, &pData->dwBufSize, &pData->dwFlags, &pData->ol, NULL );
}

IO_DATA* NetworkService::GetIOData()
{
	EnterCriticalSection(cs.get());
	IO_DATA* pData = nullptr;

	if (m_lstIoDatasUnused.empty())
	{
		m_lstIoDatasUsed.push_back(std::make_shared<IO_DATA>());
	}
	else
	{
		m_lstIoDatasUsed.push_back(m_lstIoDatasUnused.top());
		m_lstIoDatasUnused.pop();
	}
	pData = m_lstIoDatasUsed.back().get();
	ZeroMemory(pData, sizeof(IO_DATA));
	
	LeaveCriticalSection(cs.get());
	return pData;
}

VOID NetworkService::ReleaseIOData( IO_DATA* pData )
{
	EnterCriticalSection(cs.get());
	auto t = std::find_if(m_lstIoDatasUsed.begin(), m_lstIoDatasUsed.end(), 
		[&](std::shared_ptr<IO_DATA> data)
	{
		if (data.get() == pData)
			return true;
		return false;
	}
	);
	if (t != m_lstIoDatasUsed.end())
		m_lstIoDatasUsed.erase(t);
	LeaveCriticalSection(cs.get());
}

void NetworkService::RemoveHandleData(HANDLE_DATA* pData)
{
	auto t = std::find_if(m_lstHandleData.begin(), m_lstHandleData.end(),
		[&](std::shared_ptr<HANDLE_DATA> data)
	{
		if (data.get() == pData)
			return true;
		return false;
	}
	);
	if (t != m_lstHandleData.end())
		m_lstHandleData.erase(t);
}

VOID WINAPI NetworkService::ThreadServerWorker( LPVOID lParam )
{
	NetworkService* pNetworkService = (NetworkService*)lParam;
	pNetworkService->m_bInWorking = TRUE;

	while( !pNetworkService->m_bWorkingEnd )
	{
		HANDLE_DATA* pHandle = NULL;
		IO_DATA*	 pData	 = NULL;
		DWORD dwSizeTransfer = 0;

		int ret = GetQueuedCompletionStatus( pNetworkService->m_hIOCP, &dwSizeTransfer,
			(PULONG_PTR)&pHandle, (LPOVERLAPPED*)&pData, INFINITE );

		if( ret == 0 )
		{
			closesocket( pHandle->s );
			pNetworkService->RemoveHandleData(pHandle);
			pNetworkService->ReleaseIOData( pData );
			continue;
		}
		else if( dwSizeTransfer == 0 )
		{
			if (pHandle != NULL)
			{
				closesocket(pHandle->s);
				pNetworkService->RemoveHandleData(pHandle);				
			}
			else
			{
				closesocket(pNetworkService->m_sListen);
				break;
			}			
			continue;
		}
		switch( pData->op )
		{
		case OP_ACCEPT:
			{
				pNetworkService->m_lstHandleData.push_back(std::make_shared<HANDLE_DATA>());
				HANDLE_DATA* pNewHandle = pNetworkService->m_lstHandleData.back().get();
				
				pNewHandle->s = pData->s;
				CreateIoCompletionPort( (HANDLE)pNewHandle->s, pNetworkService->m_hIOCP, 
					(ULONG_PTR)pNewHandle, 0 );
				pNetworkService->ReleaseIOData( pData );
				pNetworkService->PostRecv( pNewHandle->s);
				++pNewHandle->dwIORecv;
				pNetworkService->PostAccept( pHandle->s );
				break;
			}
		case OP_READ:
			{
				--pHandle->dwIORecv;
				memcpy( pHandle->BufRecv, 
					pData->BufPackage, dwSizeTransfer );
				pHandle->dwSizeRecv = dwSizeTransfer;
				
				pHandle->BufRecv[pHandle->dwSizeRecv] = 0;
				pNetworkService->ReleaseIOData( pData );
				std::cout << "收到信息,数据大小:" << pHandle->listRecv->back().size();
				pNetworkService->PostRecv( pHandle->s);
				pNetworkService->PostSend( pHandle->s,
					(LPSTR)pHandle->BufRecv, pHandle->dwSizeRecv );

				break;
			}
		case OP_WRITE:
			{
				pNetworkService->ReleaseIOData( pData );
				break;
			}
		case OP_CLOSE:
			{
				break;
			}
		}
	}

	pNetworkService->m_bInWorking = FALSE;
	pNetworkService->m_bWorkingEnd = TRUE;

	pNetworkService->m_sListen = NULL;
}

VOID WINAPI NetworkService::ThreadClientWorker( LPVOID lParam )
{
	NetworkService* pNetworkService = (NetworkService*)lParam;
	pNetworkService->m_bInWorking = TRUE;
	while( !pNetworkService->m_bWorkingEnd )
	{
		HANDLE_DATA* pHandle = NULL;
		IO_DATA*	 pData	 = NULL;
		DWORD dwSizeTransfer = 0;

		int ret = GetQueuedCompletionStatus( pNetworkService->m_hIOCP, &dwSizeTransfer,
			(PULONG_PTR)&pHandle, (LPOVERLAPPED*)&pData, INFINITE );

		if( ret == 0 )
		{
			closesocket( pHandle->s );
			pNetworkService->RemoveHandleData(pHandle);
			pNetworkService->ReleaseIOData( pData );
			break;
		}
		else if( dwSizeTransfer == 0 ) 
		{
			closesocket(pNetworkService->m_sServer);
			pNetworkService->RemoveHandleData(pHandle);
			break;
		}
		switch( pData->op )
		{
		case OP_READ:
			{
				--pHandle->dwIORecv;
				memcpy(pHandle->BufRecv,
					pData->BufPackage, dwSizeTransfer);
				pHandle->dwSizeRecv = dwSizeTransfer;
				EnterCriticalSection(pNetworkService->cs.get());
				pHandle->listRecv->push(std::string());
				pHandle->listRecv->back().assign(pHandle->BufRecv,
					pHandle->BufRecv + pHandle->dwSizeRecv);
				LeaveCriticalSection(pNetworkService->cs.get());
				pNetworkService->ReleaseIOData(pData);
				std::cout << "收到数据大小:" << pHandle->listRecv->back().size() << std::endl;
				pNetworkService->PostRecv(pHandle->s);

				break;
			}
		case OP_WRITE:
			{
				pNetworkService->ReleaseIOData( pData );
				break;
			}
		case OP_CLOSE:
			{
				break;
			}
		}
	}

	pNetworkService->m_bInWorking = FALSE;
	pNetworkService->m_bWorkingEnd = TRUE;
}