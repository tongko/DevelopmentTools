#include "stdafx.h"
#include "NamedMem.h"
#include "AutoLock.h"


template <typename T>
CAutoLock<T>::CAutoLock(CNamedMem<T>* namedMem)
{
	m_pMem = namedMem;
	if (m_pMem != NULL)
		m_pMem->LockMemory();
}


template <typename T>
CAutoLock<T>::~CAutoLock()
{
	if (m_pMem != NULL)
		m_pMem->ReleaseMemory();
}
