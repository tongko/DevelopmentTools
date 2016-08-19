#pragma once

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG

#define NamedMemTrace TRACE

#else

#define NameMemTrace __noop

#endif

///////////////////////////////////////////////////////////////////////////////

template <typename T>
class CNamedMem
{
	public:
		CNamedMem();
		CNamedMem(LPCTSTR szName, DWORD dwSize, BOOL bCreate = TRUE);
		~CNamedMem();

	public:
		VOID	Create(LPCTSTR szName, DWORD dwSize, LPCTSTR szUser);
		VOID	Open(LPCTSTR szName);

		VOID	LockMemory(VOID);
		VOID	ReleaseMemory(VOID);

		VOID	SetRequest(VOID);
		VOID	SetRespond(VOID);
		HANDLE	GetRequest(VOID) const;
		HANDLE	GetRespond(VOID) const;

		T*		Get(VOID) const;
		T&		operator[](size_t index) const;
		T*		operator->(VOID) const;
		T&		operator*(VOID) const;
		
		CNamedMem& operator=(const T& value);

	private:
		VOID		InitSyncObjects(const SECURITY_ATTRIBUTES* sa, LPCTSTR lpszName);
		VOID		SetName(LPCTSTR szName);

	private:
		LPTSTR			m_szName;
		DWORD			m_dwSize;

		CAutoHandle		m_hView;
		T*				m_pMapped;

		CAutoHandle		m_hLock;
		CAutoHandle		m_hRequest;
		CAutoHandle		m_hRespond;
};