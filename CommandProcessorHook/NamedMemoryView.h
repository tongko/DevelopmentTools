#pragma once

typedef struct _NAMED_MEM_PARAMS
{
	LPTSTR		lpszName;
	DWORD		dwSize;

} NAMED_MEM_PARAMS, *PNAMED_MEM_PARAMS;


template <typename T>
class CNamedMemoryView
{
	public:
		CNamedMemoryView();
		~CNamedMemoryView();

	private:
		BOOL		Create(LPCTSTR lpszName, DWORD size);

	private:
		HANDLE		m_hFile;
		LPVOID		m_pView;
		LPTSTR		m_lpszName;
		DWORD		m_dwSize;
};

template<typename T>
inline CNamedMemoryView<T>::CNamedMemoryView()
{
}

template<typename T>
inline CNamedMemoryView<T>::~CNamedMemoryView()
{
}

template<typename T>
inline BOOL CNamedMemoryView<T>::Create(LPCTSTR lpszName, DWORD size)
{
	m_lpszName = lpszName;
	m_dwSize = dwSize;

	m_hFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, m_dwSize * sizeof(T), m_lpszName);
	if (!m_hFile)
	{

	}

	return 0;
}
