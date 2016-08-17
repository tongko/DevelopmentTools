#pragma once

#include "stdafx.h"
#include <malloc.h>

#define	MAX_STR_LEN	1024

class WinApiException
{
	public:
		WinApiException(LPCTSTR pszWhere, DWORD dwError) noexcept
		{
			size_t szLen = 0;
			HRESULT hResult;
			
			if (FAILED((hResult = StringCchLength(pszWhere, MAX_STR_LEN, &szLen))))
				szLen = MAX_STR_LEN;

			szLen *= sizeof(TCHAR);
			m_pszWhere = static_cast<LPTSTR>(malloc(szLen));
			SecureZeroMemory(m_pszWhere, szLen);
			if (FAILED((hResult = StringCchCopy(m_pszWhere, szLen, pszWhere))))
				m_pszMessage = TEXT("\0");
		}

		~WinApiException() noexcept
		{
			if (m_pszWhere != NULL)
				free(m_pszWhere);
			if (m_pszMessage != NULL)
				free(m_pszMessage);
		}

	public:
		LPCTSTR	Message() noexcept
		{
			if (m_pszMessage == NULL)
				MakeMessage();

			return m_pszMessage;
		}

		DWORD LastError() noexcept
		{
			return m_dwError;
		}

	private:
		VOID MakeMessage()
		{
			size_t szLen = MAX_STR_LEN * sizeof(TCHAR);
			m_pszMessage = static_cast<LPTSTR>(malloc(szLen));
			SecureZeroMemory(m_pszMessage, szLen);

			LPVOID pszBuff;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				m_dwError,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&pszBuff,
				0,
				NULL);

			StringCchPrintf(m_pszMessage, LocalSize(m_pszMessage) / sizeof(TCHAR),
				TEXT("[ERROR] %s failed with %d: %s"), m_pszWhere, m_dwError, pszBuff);

			LocalFree(pszBuff);
		}

	private:
		DWORD		m_dwError;
		LPTSTR		m_pszWhere;
		LPTSTR		m_pszMessage;
};