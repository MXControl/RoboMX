#include "stdafx.h"
#include ".\logfile.h"
#include "Settings.h"
#include "util.h"

extern CSettings g_sSettings;

CLogFile::CLogFile(void)
{

	m_bFileOpened = FALSE;
}

CLogFile::~CLogFile(void)
{
	
	if(m_bFileOpened){

		TRY{

			m_fLog.Flush();
			m_fLog.Close();
		}
		CATCH(CFileException, e){

		}END_CATCH;

	}
}

void CLogFile::SetRoom(CString strRoom)
{

	m_strRoom = Util::MakeValidFilename(strRoom);
}

void CLogFile::Open()
{

	ShellExecute(0, "open", 
				g_sSettings.GetLogDir() + "\\" +  m_strRoom + Util::GetDateString() + ".txt",
                0, 0, SW_SHOW);
}

void CLogFile::Log(CString strName, CString strMsg)
{

	ASSERT(!m_strRoom.IsEmpty());

	m_strFilename = g_sSettings.GetLogDir() + "\\" +  m_strRoom + Util::GetDateString() + ".txt";

	TRY{

		Util::CreateDirs(g_sSettings.GetLogDir() + "\\");
		m_bFileOpened = m_fLog.Open(m_strFilename, CFile::shareDenyNone|CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::typeText);
		if(m_bFileOpened){

			m_fLog.SeekToEnd();
			CString strTime = Util::GetMyLocalTime();
			CString strLog;

			if(strMsg.IsEmpty()){

				strLog.Format("[%s] %s\n", strTime, strName);
			}
			else{
				
				strLog.Format("[%s] %s %s\n", strTime, strName, strMsg);
			}

			m_fLog.WriteString(strLog);
			m_fLog.Flush();
			m_fLog.Close();
			m_bFileOpened = FALSE;
		}
	}
	CATCH(CFileException, e){

		m_bFileOpened = FALSE;
	}END_CATCH;
}

BOOL CLogFile::GetPreviousJoinDate(CString& strDate)
{

	CFileFind finder;

	strDate.Empty();

	BOOL bFound = finder.FindFile(GetLogFilter());
	
	CString strFile;
	
	while(bFound){

		bFound = finder.FindNextFile();
		strFile = finder.GetFileTitle();
	}

	if(strFile.GetLength()){

		CTime time;
		finder.GetCreationTime(time);
		strDate = time.Format("%m/%d/%Y");
	}

	return !strDate.IsEmpty();
}

CString CLogFile::GetLogFilter()
{

	CString strFilter;;

	strFilter = g_sSettings.GetLogDir() + "\\" +  m_strRoom + "*.txt";

	return strFilter;
}


void CLogFile::GetLast50Lines(CString& strLogLines)
{
	CFileFind finder;

	strLogLines.Empty();

	BOOL bFound = finder.FindFile(GetLogFilter());
	
	CString strFile;
	
	while(bFound){

		bFound = finder.FindNextFile();
		strFile = finder.GetFilePath();
	}
	
	CTime time;
	finder.GetCreationTime(time);
	
	CString strDate = time.Format("[%m/%d/%Y] ");

	if(strFile.GetLength()){

		TRY{

			CStdioFile file;
			if(file.Open(strFile, CFile::shareExclusive|CFile::typeText|CFile::modeNoTruncate|CFile::modeRead|CFile::modeCreate, 0)){

				int nLines = 0;
				CString strLine;

				// count lines in file
				while(file.ReadString(strLine)){
					
					nLines++;
				}
				
				file.SeekToBegin();

				// if we have more then 50 lines read until there are only 50 lines left
				if(nLines > 50){

					int nCurrent = 0;
					while((nLines - nCurrent) > 50){

						file.ReadString(strLine);
						nCurrent++;
					}
				}

				while(file.ReadString(strLine)){
					
					strLogLines += strDate + strLine + "\n";
				}

				strLogLines.Remove('\r');
				file.Close();
				//strLogLines.Replace("\n", "\r\n");

			}
		}
		CATCH(CFileException, e){

		}END_CATCH;
	}

}
