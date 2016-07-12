#pragma once

class CHookHandler
{
	public:
		CHookHandler();
		~CHookHandler();

	public:
		DWORD WINAPI StartMonitorAsync();
		VOID WINAPI StopMonitorAsync();

	private:
		static DWORD WINAPI MonitorHandler(LPVOID lpParam);

		DWORD Monitoring();

	private:
		HANDLE			m_hMonitor;
		HANDLE			m_hStopMonitor;
};

