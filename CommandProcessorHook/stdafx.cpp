// stdafx.cpp : source file that includes just the standard includes
// CommandProcessorHook.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file


//	For Debugging
#ifdef _DEBUG

void Trace(LPCTSTR pszFormat, ...)
{
	va_list valist;
	va_start(valist, pszFormat);
	TCHAR strMessage[2048];
	StringCchVPrintf(strMessage, 2048, pszFormat, valist);
	OutputDebugString(strMessage);
}

#endif // _DEBUG
