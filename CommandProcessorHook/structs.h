#pragma once

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

struct CmdProcSettings
{
	CmdProcSettings()
		: dwRefreshInterval(0)
		, dwHookThreadId(0)
		, dwParentProcId(0)
		, crdRowCol {0,0}
		, crdBufferRowCol {0, 0}
		, crdMaxRowCol {0, 0}
		, hwndCmdProcWindow(NULL)
	{
	}

	CmdProcSettings(const CmdProcSettings& other)
		: dwRefreshInterval(other.dwRefreshInterval)
		, dwHookThreadId(other.dwHookThreadId)
		, dwParentProcId(other.dwParentProcId)
		, crdRowCol(other.crdRowCol)
		, crdBufferRowCol(other.crdBufferRowCol)
		, crdMaxRowCol(other.crdMaxRowCol)
		, hwndCmdProcWindow(other.hwndCmdProcWindow)
	{
	}

	DWORD		dwParentProcId;
	DWORD		dwHookThreadId;

	DWORD		dwRefreshInterval;

	COORD		crdRowCol;
	COORD		crdBufferRowCol;
	COORD		crdMaxRowCol;

	HWND		hwndCmdProcWindow;
};