// stdafx.cpp : source file that includes just the standard includes
// CommandProcessorHook.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

VOID Throw(LPCTSTR module, DWORD error)
{
	wchar_t numStr[21];		//	up to 64 bits max value number
	swprintf_l(numStr, "%d", error);

	wstring buff = sstm.str();

	LPTSTR buffer = NULL;
	DWORD length = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
		(LPTSTR)&buffer,
		0,
		NULL);
	if (length > 0)
		buff += buffer;


	return VOID();
}
