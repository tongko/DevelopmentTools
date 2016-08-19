#include "stdafx.h"
#include "StdOut.h"

CStdOut::CStdOut(BOOL bReadOnly)
{
	m_hStdOut = CreateFile(
		L"CONOUT$",
		bReadOnly ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE,
		bReadOnly ? FILE_SHARE_READ : FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		0);

	if (m_hStdOut == INVALID_HANDLE_VALUE)
	{
		WinApiException ex(_T("CStdOut::ctor::CreateFile"), GetLastError());
		TRACE(_T("CreateFile returns error (%lu): %S\r\n"), ex.LastError(), ex.Message());
	}
}

CStdOut::~CStdOut()
{
	if (m_hStdOut && m_hStdOut != INVALID_HANDLE_VALUE)
		CloseHandle(m_hStdOut);
}

CStdOut::operator HANDLE()
{
	return m_hStdOut;
}


