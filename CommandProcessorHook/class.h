#pragma once


class CAutoHandle
{
	public:
		CAutoHandle() : m_handle() {}
		CAutoHandle(HANDLE handle) : m_handle(handle) {}
		~CAutoHandle() { if (IsValid()) CloseHandle(m_handle); }

	public:
		inline BOOL IsValid() { return (m_handle) && (m_handle != INVALID_HANDLE_VALUE); }
		//inline operator HANDLE&() { return m_handle; }
		inline operator HANDLE() const { return m_handle; }
		inline CAutoHandle& operator =(const HANDLE& handle)
		{
			m_handle = handle;

			return *this;
		}

	private:
		HANDLE		m_handle;
};
