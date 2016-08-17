#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
//	Init
///////////////////////////////////////////////////////////////////////////////

CHookHandler::CHookHandler()
	: m_hStopMonitor(CreateEvent(NULL, FALSE, FALSE, NULL))
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
	pThis->Monitoring();
}

DWORD CHookHandler::Monitoring()
{
	//	The actual monitoring process
}

VOID CHookHandler::ReadConsoleBuffer(VOID)
{
	auto nowTime = system_clock::now();
	DWORD	dwElapseTime = duration_cast<milliseconds>(nowTime - m_tpLastCheck).count();
	DWORD	dwRefreshInterval;
}

VOID CHookHandler::PerformReadConsoleBuffer(VOID)
{
	return VOID();
}
