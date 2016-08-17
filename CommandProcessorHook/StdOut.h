#pragma once

class CStdOut
{
	public:
		CStdOut(BOOL bReadOnly);
		~CStdOut();

	public:
		operator HANDLE();

	private:
		HANDLE		m_hStdOut;
};

