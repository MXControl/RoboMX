#pragma once

class CLogFile
{
public:
	CLogFile(void);
	~CLogFile(void);

public:
	void SetRoom(CString strRoom);
	void Open();
	void Log(CString strName, CString strMsg);
	BOOL GetPreviousJoinDate(CString& strDate);
	CString GetLogFilter();

protected:
	CStdioFile	m_fLog;
	CString		m_strFilename;
	CString		m_strRoom;
	BOOL		m_bFileOpened;
public:
	void GetLast50Lines(CString& strLogLines);
};
