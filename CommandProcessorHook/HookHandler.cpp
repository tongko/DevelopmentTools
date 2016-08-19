#include "stdafx.h"
#include <chrono>

//typedef	system_clock::time_point TimePoint;

#include "StdOut.h"
#include "NamedMem.h"
#include "HookHandler.h"

///////////////////////////////////////////////////////////////////////////////
//	Init
///////////////////////////////////////////////////////////////////////////////

CHookHandler::CHookHandler()
	: m_hStopMonitor(CreateEvent(NULL, FALSE, FALSE, NULL))
	, m_hStdOut(FALSE)
	, m_buffer()
	, m_copyInfo()
	, m_cursorInfo()
	, m_dwBufferSize(0)
	, m_dwWaitElapse(0)
	, m_hMonitor()
	, m_info()
	, m_layoutInfo()
	, m_mouseEvents()
	, m_scrollPos()
	, m_settings()
	, m_szConsoleTitle(_T(" "))
	//, m_tpLastCheck()
{
}


CHookHandler::~CHookHandler()
{
}

///////////////////////////////////////////////////////////////////////////////
//	Public operation
///////////////////////////////////////////////////////////////////////////////

DWORD CHookHandler::StartMonitorAsync(VOID)
{
	DWORD threadId = 0;
	m_hMonitor = CreateThread(NULL, 0, MonitorHandler, static_cast<LPVOID>(this),
		0, &threadId);

	return m_hMonitor == INVALID_HANDLE_VALUE ? -1 : threadId;
}

VOID CHookHandler::StopMonitorAsync(VOID)
{
	SetEvent(m_hStopMonitor);
	WaitForSingleObject(m_hStopMonitor, 10000);
}

///////////////////////////////////////////////////////////////////////////////
//	Private operation
///////////////////////////////////////////////////////////////////////////////

DWORD CHookHandler::MonitorHandler(LPVOID lpParam)
{
	CHookHandler* pThis = static_cast<CHookHandler*>(lpParam);
	
	return pThis->Monitoring();
}

DWORD CHookHandler::Monitoring()
{
	//	The actual monitoring process
	return 0;
}

VOID CHookHandler::ReadConsoleBuffer(VOID)
{
	//auto nowTime = std::chrono::system_clock::now();
	//DWORD	dwElapseTime = static_cast<DWORD>(
	//	std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - m_tpLastCheck).count());
	//DWORD	dwRefreshInterval;
}

VOID CHookHandler::PerformReadConsoleBuffer(VOID)
{
	return VOID();
}
