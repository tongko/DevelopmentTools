// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <strsafe.h>
#include <debugapi.h>
#include <tchar.h>
#include <string>

using namespace std;


// TODO: reference additional headers your program requires here
#ifdef _UNICODE
typedef	wstring _tstring;
#else
typedef string _tstring;
#endif

//	For Debugging
#ifdef _DEBUG
void Trace(LPCTSTR pszFormat, ...);
#define	TRACE	Trace
#else
#define TRACE	__noop
#endif // _DEBUG

#include "structs.h"
#include "class.h"
#include "WinApiException.h"
