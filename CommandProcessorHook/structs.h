#pragma once

#define		MAX_SELECT_PART		256

///////////////////////////////////////////////////////////////////////////////
//	CMDPROC_SETTINGS
///////////////////////////////////////////////////////////////////////////////

struct CMDPROC_SETTINGS
{
	CMDPROC_SETTINGS()
		: dwRefreshInterval(0)
		, dwHookThreadId(0)
		, dwParentProcId(0)
		, crdRowCol {0,0}
		, crdBufferRowCol {0, 0}
		, crdMaxRowCol {0, 0}
		, hwndCmdProcWindow(NULL)
	{
	}

	CMDPROC_SETTINGS(const CMDPROC_SETTINGS& other)
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


///////////////////////////////////////////////////////////////////////////////
//	CMDPROC_LAYOUT_INFO
///////////////////////////////////////////////////////////////////////////////

struct CMDPROC_LAYOUT_INFO
{
	CMDPROC_LAYOUT_INFO()
		: dwColumns(0)
		, dwRows(0)
		, dwResizeWindowEdge(0)
	{
	}

	DWORD	dwColumns;
	DWORD	dwRows;
	DWORD	dwResizeWindowEdge;
};


///////////////////////////////////////////////////////////////////////////////
//	NewlineCar
///////////////////////////////////////////////////////////////////////////////

enum NewlineChar
{
	CRLF	= 0,
	LF		= 1
};

///////////////////////////////////////////////////////////////////////////////
//	SelectionType
///////////////////////////////////////////////////////////////////////////////

enum SelectionType
{
	Text = 0,
	Column = 1
};

///////////////////////////////////////////////////////////////////////////////
//	CMDPROC_COPY_INFO
///////////////////////////////////////////////////////////////////////////////

struct CMDPROC_COPY_INFO
{
	CMDPROC_COPY_INFO()
		: crdStart()
		, crdEnd()
		, bNoWrap(false)
		, dwEOLSpaces(1)
		, bTrimSpaces(false)
		, bRTF(true)
		, enNewlineChar(CRLF)
		, enSelectionType(Text)
		, bBold(false)
		, bItalic(false)
		, dwSize(24)
		, bClipboard(true)
		, dwSelectionPartOffset(0) 
	{
		clrColors[0] = 0x00000000;
		clrColors[1] = 0x800000;
		clrColors[2] = 0x008000;
		clrColors[3] = 0x808000;
		clrColors[4] = 0x000080;
		clrColors[5] = 0x800080;
		clrColors[6] = 0x008080;
		clrColors[7] = 0xC0C0C0;
		clrColors[8] = 0x808080;
		clrColors[9] = 0xFF0000;
		clrColors[10] = 0x00FF00;
		clrColors[11] = 0xFFFF00;
		clrColors[12] = 0x0000FF;
		clrColors[13] = 0xFF00FF;
		clrColors[14] = 0x00FFFF;
		clrColors[15] = 0xFFFFFF;

		szFontName[0] = 0;
	}

	COORD			crdStart;
	COORD			crdEnd;
	BOOL			bNoWrap;
	BOOL			bTrimSpaces;
	BOOL			bRTF;
	BOOL			bBold;
	BOOL			bItalic;
	NewlineChar		enNewlineChar;
	SelectionType	enSelectionType;
	DWORD			dwSize;
	DWORD			dwEOLSpaces;
	TCHAR			szFontName[256];
	COLORREF		clrColors[16];
	BOOL			bClipboard;
	TCHAR			szSelectionPart[MAX_SELECT_PART + 1]; /* text + zero terminated char */
	DWORD			dwSelectionPartOffset;
};


///////////////////////////////////////////////////////////////////////////////
//	CMDPROC_INFO
///////////////////////////////////////////////////////////////////////////////

struct CMDPROC_INFO
{
	CMDPROC_INFO()
		: csbi()
		, ullProgressCompleted(0ULL)
		, ullProgressTotal(0ULL)
		, bTextChanged(FALSE)
		, bTitleChanged(FALSE)
		, bBufferInfoChanged(FALSE)
		, bProgressChanged(FALSE)
	{
	}

	CONSOLE_SCREEN_BUFFER_INFO	csbi;
	UINT64						ullProgressCompleted;
	UINT64						ullProgressTotal;
	BOOL						bTextChanged;
	BOOL						bTitleChanged;
	BOOL						bBufferInfoChanged;
	BOOL						bProgressChanged;
};


///////////////////////////////////////////////////////////////////////////////
//	CMDPROC_CHAR_INFO
///////////////////////////////////////////////////////////////////////////////

struct CMDPROC_CHAR_INFO
{
	CMDPROC_CHAR_INFO()
		: bChanged(FALSE)
	{
		*(reinterpret_cast<DWORD*>(&ciInfo)) = 0x00000020;	// 0, ' '
	}

	CHAR_INFO	ciInfo;
	BOOL		bChanged;
};
