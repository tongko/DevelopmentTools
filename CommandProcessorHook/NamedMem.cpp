#include "stdafx.h"
#include "NamedMem.h"
#include "AutoLock.h"


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
CNamedMem<T>::CNamedMem()
	: m_szName(T(""))
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
	: m_szName(szName)
	, m_dwSize(dwSize)
	, m_hLock()
	, m_hRequest()
	, m_hRespond()
	, m_hView()
	, m_pMapped()
{
	if (bCreate)
		Create(szName, dwSize);
	else
		Open(szName);
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
CNamedMem<T>::~CNamedMem()
{
	if (m_pMapped != NULL)
		UnmapViewOfFile(m_pMapped);
	if (m_hView != NULL)
		CloseHandle(m_hView);
	if (m_hLock != NULL)
		CloseHandle(m_hLock);
	if (m_hRequest != NULL)
		CloseHandle(m_hRequest);
	if (m_hRespond != NULL)
		CloseHandle(m_hRespond);
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::Create(LPCTSTR szName, DWORD dwSize, LPCTSTR szUser)
{
	m_szName = szName;
	m_dwSize = dwSize;

	unique_ptr<SECURITY_ATTRIBUTES>		sa;
	EXPLICIT_ACCESS						ea[2];
	SID_IDENTIFIER_AUTHORITY			SIDAuthCreator = SECURITY_CREATOR_SID_AUTHORITY;
	PSID								tmpSID = NULL;
	unique_ptr<void>					creatorSID;
	PACL								tmpACL = NULL;
	unique_ptr<ACL>						acl;
	unique_ptr<void>					sd;

	ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));

	if (_tcslen(szUser) > 0)
	{
		ea[0].grfAccessPermissions = GENERIC_ALL;
		ea[0].grfAccessMode = SET_ACCESS;
		ea[0].grfInheritance = NO_INHERITANCE;
		ea[0].Trustee.TrusteeForm = TRUSTEE_IS_NAME;
		ea[0].Trustee.TrusteeType = TRUSTEE_IS_USER;
		ea[0].Trustee.ptstrName = szUser;

		if (!AllocateAndInitializeSid(
			&SIDAuthCreator,
			1,
			SECURITY_CREATOR_OWNER_RID,
			0, 0, 0, 0, 0, 0, 0,
			&tmpSID))
			throw WinApiException(T("CNamedMem<T>::Create@AllocateAndInitializeSid"), GetLastError());

		creatorSID.reset(tmpSID, ::FreeSid);

		ea[1].grfAccessPermissions = GENERIC_ALL;
		ea[1].grfAccessMode = SET_ACCESS;
		ea[1].grfInheritance = NO_INHERITANCE;
		ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
		ea[1].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
		ea[1].Trustee.ptstrName = (LPTSTR)creatorSID.get();

		if (SetEntriesInAcl(2, ea, NULL, &tmpACL) != ERROR_SUCCESS)
			throw WinApiException(T("CNamedMem<T>::Create@SetEntriesInAcl"), GetLastError());

		acl.reset(tmpACL, LocalFree);
		sd.reset(LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH), LocalFree);

		if (!sd)
			throw WinApiException(T("CNamedMem<T>::Create@LocalAlloc"), GetLastError());

		if (!InitializeSecurityDescriptor(sd.get(), SECURITY_DESCRIPTOR_REVISION))
			throw WinApiException(T("CNamedMem<T>::Create@InitializeSecurityDescriptor"), GetLastError());

		if (!SetSecurityDescriptorDacl(
			sd.get(),
			TRUE,		// bDaclPresent flag   
			acl.get(),
			FALSE))		// not a default DACL 
			throw WinApiException(T("CNamedMem<T>::Create@SetSecurityDescriptorDacl"), GetLastError());

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

	if (!m_hView)
		throw WinApiException(T("CNamedMem<T>::Create@CreateFileMapping"), GetLastError());

	m_pMapped = static_cast<T*>(MapViewOfFile(
		m_hView,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0));

	if (!m_pMapped)
		throw WinApiException(T("CNamedMem<T>::Create@MapViewOfFile", GetLastError());

	ZeroMemory(m_pMapped, m_dwSize * sizeof(T));

	InitSyncObjects(sa, strName);
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::Open(LPCTSTR szName)
{
	return VOID();
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::LockMemory(VOID)
{
	return VOID();
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::ReleaseMemory(VOID)
{
	return VOID();
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::SetRequest(VOID)
{
	return VOID();
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::SetRespond(VOID)
{
	return VOID();
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
HANDLE CNamedMem<T>::GetRequest(VOID) const
{
	return HANDLE();
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
HANDLE CNamedMem<T>::GetRespond(VOID) const
{
	return HANDLE();
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
T * CNamedMem<T>::Get(VOID) const
{
	return nullptr;
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
T & CNamedMem<T>::operator[](size_t index) const
{
	// TODO: insert return statement here
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
T * CNamedMem<T>::operator->(VOID) const
{
	return nullptr;
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
T & CNamedMem<T>::operator*(VOID) const
{
	// TODO: insert return statement here
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
CNamedMem & CNamedMem<T>::operator=(const T & value)
{
	// TODO: insert return statement here
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

template<typename T>
VOID CNamedMem<T>::InitSyncObjects(const SECURITY_ATTRIBUTES * sa, LPCTSTR lpszName)
{
	return VOID();
}



