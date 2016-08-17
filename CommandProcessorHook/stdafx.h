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

using namespace std;


// TODO: reference additional headers your program requires here
#include "structs.h"
#include "WinApiException.h"
#include "HookHandler.h"


//	For Debugging
#ifdef _DEBUG
void Trace(LPCTSTR pszFormat, ...);
#define	TRACE	Trace
#else
#define TRACE	__noop
#endif // _DEBUG
