#pragma once

#include <chrono>

using namespace chrono;
typedef	system_clock::time_point TimePoint;

class CHookHandler
{
	public:
		CHookHandler();
		~CHookHandler();

	public:		//	Operations
		DWORD WINAPI StartMonitorAsync(VOID);
		VOID WINAPI StopMonitorAsync(VOID);

	private:	//	Operations
		static DWORD WINAPI MonitorHandler(LPVOID lpParam);

		DWORD Monitoring();

		VOID	ReadConsoleBuffer(VOID);
		VOID	PerformReadConsoleBuffer(VOID);

	private:	//	Members
		HANDLE		m_hMonitor;
		HANDLE		m_hStopMonitor;

		TCHAR		m_szConsoleTitle[1024];

		DWORD		m_dwBufferSize;
		DWORD		m_dwWaitElapse;

		TimePoint	m_tpLastCheck;
};

