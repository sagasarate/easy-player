/****************************************************************************/
/*                                                                          */
/*      �ļ���:    FileManagerWnd.h                                         */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CMainFrame;
class CFileManagerWnd :
	public CD3DWnd
{
protected:
	struct FILE_INFO
	{
		CEasyString	FilePath;
		CEasyString FileName;
		CEasyString ShowText;
		BOOL		IsDirectory;
		BOOL		IsDots;
		static int Sort(const void * p1,const void * p2)
		{
			FILE_INFO * pInfo1=(FILE_INFO *)p1;
			FILE_INFO * pInfo2=(FILE_INFO *)p2;
			if(pInfo1->IsDirectory==pInfo2->IsDirectory)
			{
				return stricmp(pInfo1->FileName,pInfo2->FileName);
			}
			else
			{
				if(pInfo1->IsDirectory==pInfo2->IsDirectory)
					return 0;
				else if(pInfo1->IsDirectory>pInfo2->IsDirectory)
					return -1;
				else return 1;
			}
		}
	};

	CMainFrame *			m_pMain;
		
	CD3DSimpleList*			m_pFileList;
	CEasyArray<FILE_INFO>	m_CurFileInfoList;
	CODBCDatabase			m_Database;
	CODBCConnection			m_DBConnection;
	CEasyArray<CEasyString>	m_CurHistoryList;
	CEasyString				m_SelectedDir;

	DECLARE_CLASS_INFO(CFileManagerWnd)
public:
	CFileManagerWnd(void);
	CFileManagerWnd(CD3DGUI * pGUI);
	virtual ~CFileManagerWnd(void);

	virtual BOOL OnMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void UpdateRects();

	void SetMainWnd(CMainFrame * pMain)
	{
		m_pMain=pMain;
	}
	void AddHistory(LPCTSTR Path);
	void FillFileList(LPCTSTR Dir);
protected:	
	void OnSelectItem(int Index);
	void LoadHistory(LPCTSTR Path);
	bool IsInHistory(LPCTSTR FileName);
	void CheckDBFile();
};
