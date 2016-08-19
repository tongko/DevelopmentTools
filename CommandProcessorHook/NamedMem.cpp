#include "stdafx.h"

#include <AccCtrl.h>
#include <AclAPI.h>
#include <memory>

#include "NamedMem.h"
#include "AutoLock.h"


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
CNamedMem<T>::CNamedMem()
	: m_szName(_T("\0"))
	, m_dwSize(0)
	, m_hLock()
	, m_hRequest()
	, m_hRespond()
	, m_hView()
	, m_pMapped()
{
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
CNamedMem<T>::CNamedMem(LPCTSTR szName, DWORD dwSize, BOOL bCreate)
	: m_szName(_T("\0"))
	, m_dwSize(dwSize)
	, m_hLock()
	, m_hRequest()
	, m_hRespond()
	, m_hView()
	, m_pMapped()
{
	SetName(szName);
	if (bCreate)
		Create(szName, dwSize, NULL);
	else
		Open(szName);
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
CNamedMem<T>::~CNamedMem()
{
	if (m_szName != NULL)
		free(m_szName);
	if (m_pMapped != NULL)
		UnmapViewOfFile(m_pMapped);
	//if (m_hView != NULL)
	//	CloseHandle(m_hView);
	//if (m_hLock != NULL)
	//	CloseHandle(m_hLock);
	//if (m_hRequest != NULL)
	//	CloseHandle(m_hRequest);
	//if (m_hRespond != NULL)
	//	CloseHandle(m_hRespond);
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::Create(LPCTSTR szName, DWORD dwSize, LPCTSTR szUser)
{
	m_dwSize = dwSize;

	unique_ptr<SECURITY_ATTRIBUTES>		sa;
	EXPLICIT_ACCESS						ea[2];
	SID_IDENTIFIER_AUTHORITY			SIDAuthCreator = SECURITY_CREATOR_SID_AUTHORITY;
	PSID								tmpSID = NULL;
	shared_ptr<void>					creatorSID;
	PACL								tmpACL = NULL;
	shared_ptr<ACL>						acl;
	shared_ptr<void>					sd;

	SecureZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));
	size_t len = _tcslen(szUser);
	if (len > 0)
	{
		TCHAR* userName = new TCHAR[len];
		_tcscpy_s(userName, len, szUser);

		ea[0].grfAccessPermissions = GENERIC_ALL;
		ea[0].grfAccessMode = SET_ACCESS;
		ea[0].grfInheritance = NO_INHERITANCE;
		ea[0].Trustee.TrusteeForm = TRUSTEE_IS_NAME;
		ea[0].Trustee.TrusteeType = TRUSTEE_IS_USER;
		ea[0].Trustee.ptstrName = userName;

		if (!AllocateAndInitializeSid(
			&SIDAuthCreator,
			1,
			SECURITY_CREATOR_OWNER_RID,
			0, 0, 0, 0, 0, 0, 0,
			&tmpSID))
			throw WinApiException(_T("CNamedMem<T>::Create@AllocateAndInitializeSid"), GetLastError());

		creatorSID.reset(tmpSID, FreeSid);

		ea[1].grfAccessPermissions = GENERIC_ALL;
		ea[1].grfAccessMode = SET_ACCESS;
		ea[1].grfInheritance = NO_INHERITANCE;
		ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
		ea[1].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
		ea[1].Trustee.ptstrName = (LPTSTR)creatorSID.get();

		if (SetEntriesInAcl(2, ea, NULL, &tmpACL) != ERROR_SUCCESS)
			throw WinApiException(_T("CNamedMem<T>::Create@SetEntriesInAcl"), GetLastError());

		acl.reset(tmpACL, LocalFree);
		sd.reset(LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH), LocalFree);

		if (!sd)
			throw WinApiException(_T("CNamedMem<T>::Create@LocalAlloc"), GetLastError());

		if (!InitializeSecurityDescriptor(sd.get(), SECURITY_DESCRIPTOR_REVISION))
			throw WinApiException(_T("CNamedMem<T>::Create@InitializeSecurityDescriptor"), GetLastError());

		if (!SetSecurityDescriptorDacl(
			sd.get(),
			TRUE,		// bDaclPresent flag   
			acl.get(),
			FALSE))		// not a default DACL 
			throw WinApiException(_T("CNamedMem<T>::Create@SetSecurityDescriptorDacl"), GetLastError());

		sa.reset(new SECURITY_ATTRIBUTES);
		sa->nLength = sizeof(SECURITY_ATTRIBUTES);
		sa->lpSecurityDescriptor = sd.get();
		sa->bInheritHandle = FALSE;
	}

	m_hView = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		sa.get(),
		PAGE_READWRITE,
		0,
		m_dwSize * sizeof(T),
		m_szName);

	if (!m_hView.IsValid())
		throw WinApiException(_T("CNamedMem<T>::Create@CreateFileMapping"), GetLastError());

	m_pMapped = static_cast<T*>(MapViewOfFile(
		m_hView,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0));

	if (!m_pMapped)
		throw WinApiException(_T("CNamedMem<T>::Create@MapViewOfFile"), GetLastError());

	ZeroMemory(m_pMapped, m_dwSize * sizeof(T));

	InitSyncObjects(sa.get(), m_szName);
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::Open(LPCTSTR szName)
{
	m_hView = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		m_szName);
	if (!m_hView.IsValid()) //(!m_hView || m_hView == INVALID_HANDLE_VALUE)
	{
		DWORD dwLastError = GetLastError();
		NamedMemTrace(_T("Error opening named memory %s, error: %d\n"), m_szName, dwLastError);
		throw WinApiException(_T("CNamedMem<T>::Open@OpenFileMapping"), dwLastError);
	}

	m_pMapped = static_cast<T*>(MapViewOfFile(
		m_hView,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0));

	if (!m_pMapped)
	{
		DWORD dwLastError = GetLastError();
		NamedMemTrace(_T("Error mapping shared mem %s, error: %d\n"), m_szName, dwLastError);
		throw WinApiException(_T("CNamedMem<T>::Open@MapViewOfFile"), dwLastError);
	}

	InitSyncObjects(unique_ptr<SECURITY_ATTRIBUTES>().get(), m_szName);
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::LockMemory(VOID)
{
	if (!m_hLock.IsValid()) //(!m_hLock)
		return;

	WaitForSingleObject(m_hLock, INFINITE);
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::ReleaseMemory(VOID)
{
	if (!m_hLock.IsValid()) //(!m_hLock)
		return;

	ReleaseMutex(m_hLock);
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::SetRequest(VOID)
{
	if (!m_hRequest.IsValid()) //(!m_hRequest)
	{
		NamedMemTrace(_T("Request event '%s' is null.\n"), m_szName);
		return;
	}

	if (!SetEvent(m_hRequest))
		NamedMemTrace(_T("Set request event '%s' failed, error: %d\n"), m_szName, GetLastError());
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::SetRespond(VOID)
{
	if (!m_hRespond.IsValid()) //(!m_hRespond)
	{
		NamedMemTrace(_T("Respond event '%s' is null.\n"), m_szName);
		return;
	}

	if (!SetEvent(m_hRespond))
		NamedMemTrace(_T("Set respond event '%s' failed, error: %d\n"), m_szName, GetLastError());
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
HANDLE CNamedMem<T>::GetRequest(VOID) const
{
	HANDLE h = m_hRequest;
	return h;
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
HANDLE CNamedMem<T>::GetRespond(VOID) const
{
	return (HANDLE)m_hRespond;
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
T * CNamedMem<T>::Get(VOID) const
{
	return m_pMapped;
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
T & CNamedMem<T>::operator[](size_t index) const
{
	return *(m_pMapped + index);
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
T * CNamedMem<T>::operator->(VOID) const
{
	return m_pMapped;
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
T & CNamedMem<T>::operator*(VOID) const
{
	return *m_pMapped;
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
CNamedMem<T> & CNamedMem<T>::operator=(const T & value)
{
	*m_pMapped = value;

	return *this;
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::InitSyncObjects(const SECURITY_ATTRIBUTES * sa, LPCTSTR lpszName)
{
	size_t nLen = _tcslen(lpszName);
	_tstring szName(lpszName);

	m_hLock = CreateMutex((LPSECURITY_ATTRIBUTES)sa, FALSE, &(szName[0] + _T("_mutex"))[0]);
	if (!m_hLock.IsValid())
		throw WinApiException(_T("CNamedMem<T>::InitSyncObjects@CreateMutex"), GetLastError());

	NamedMemTrace(_T("m_hLock '%s': %#010X\n"), szName, reinterpret_cast<DWORD_PTR>((HANDLE)m_hLock));

	m_hRequest = CreateEvent((LPSECURITY_ATTRIBUTES)sa, FALSE, FALSE, &(szName + _T("_request"))[0]);
	if (!m_hRequest.IsValid())
		throw WinApiException(_T("CNamedMem<T>::InitSyncObjects@CreateEvent"), GetLastError());

	NamedMemTrace(_T("m_hRequest '%s': %#010X\n"), szName, reinterpret_cast<DWORD_PTR>((HANDLE)m_hRequest));

	m_hRespond = CreateEvent((LPSECURITY_ATTRIBUTES)sa, FALSE, FALSE, &(szName + _T("_respond"))[0]);
	if (!m_hRespond.IsValid())
		throw WinApiException(_T("CNamedMem<T>::InitSyncObjects@CreateEvent"), GetLastError());

	NamedMemTrace(_T("m_hRespond '%s': %#010X\n"), szName, reinterpret_cast<DWORD_PTR>((HANDLE)m_hRespond));
}


///////////////////////////////////////////////////////////////////////////////
//	SetName
///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::SetName(LPCTSTR szName)
{
	size_t nLen = _tcslen(szName);
	if (nLen > 0)
	{
		m_szName = (LPTSTR)malloc(nLen);
		SecureZeroMemory(m_szName, nLen);
		_tcscpy_s(m_szName, nLen, szName);
	}
}



///////////////////////////////////////////////////////////////////////////////
//	To prevent LNK2019 - unresolved external for template class
///////////////////////////////////////////////////////////////////////////////

template class CNamedMem<CMDPROC_SETTINGS>;
template class CNamedMem<CMDPROC_INFO>;
template class CNamedMem<CMDPROC_LAYOUT_INFO>;
template class CNamedMem<CMDPROC_COPY_INFO>;
template class CNamedMem<CONSOLE_CURSOR_INFO>;
template class CNamedMem<CHAR_INFO>;
template class CNamedMem<MOUSE_EVENT_RECORD>;
template class CNamedMem<SIZE>;
