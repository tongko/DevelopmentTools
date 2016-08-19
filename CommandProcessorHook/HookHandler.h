#pragma once

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

		//std::chrono::system_clock::time_point	m_tpLastCheck;

		//	Named mem
		CNamedMem<CMDPROC_SETTINGS>			m_settings;
		CNamedMem<CMDPROC_INFO>				m_info;
		CNamedMem<CMDPROC_LAYOUT_INFO>		m_layoutInfo;
		CNamedMem<CMDPROC_COPY_INFO>		m_copyInfo;
		CNamedMem<CONSOLE_CURSOR_INFO>		m_cursorInfo;
		CNamedMem<CHAR_INFO>				m_buffer;
		CNamedMem<MOUSE_EVENT_RECORD>		m_mouseEvents;
		CNamedMem<SIZE>						m_scrollPos;

		CStdOut								m_hStdOut;
};

