/****************************************************************************/
/*                                                                          */
/*      文件名:    FileManagerWnd.cpp                                       */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\filemanagerwnd.h"
#include <algorithm>

IMPLEMENT_CLASS_INFO(CFileManagerWnd,CD3DWnd);
CFileManagerWnd::CFileManagerWnd(void):CD3DWnd()
{
	m_pMain=NULL;
	m_pFileList=NULL;
	
}

CFileManagerWnd::CFileManagerWnd(CD3DGUI * pGUI):CD3DWnd(pGUI)
{
	m_pMain=NULL;
	m_pFileList=NULL;
	
}

CFileManagerWnd::~CFileManagerWnd(void)
{
}

BOOL CFileManagerWnd::OnMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(CD3DWnd::OnMessage(pWnd,msg,wParam,lParam))
		return true;

	switch(msg)	
	{
	case WM_KEYUP:
		switch(wParam)
		{
		case VK_RETURN:
			if(pWnd==m_pFileList)
			{
				OnSelectItem(m_pFileList->GetFirstSelectedItem());
				return true;
			}
			break;
		case VK_ESCAPE:
			SetVisible(false);
			return true;
		}
		break;
	case WM_D3DGUI_WND_LOADED:
		if(pWnd==this)
		{
			m_Database.Init();
			m_DBConnection.Init(&m_Database);
			CheckDBFile();
			CEasyString DBFileName=MakeModuleFullPath(NULL,"History.mdb");
			CEasyString ConnectStr;
			ConnectStr.Format("DRIVER=Microsoft Access Driver (*.mdb);UID=admin;ExtendedAnsiSQL=1;UserCommitSync=Yes;Threads=3;PageTimeout=5;MaxScanRows=8;MaxBufferSize=2048;DriverId=25;DBQ=%s",
				(LPCTSTR)DBFileName);
			if(m_DBConnection.Connect(ConnectStr)==DBERR_SUCCEED)
			{
			}
		}
		return true;
	case WM_D3DGUI_CHILD_LOADED:
		if((CD3DWnd*)lParam==this)
		{
			m_pFileList=(CD3DSimpleList *)GetChildWndByName("FileList");
			CEasyString RecentPath=(LPCTSTR)theApp.GetProfileString("History","RecentPlayFile","");
			if(GetFileAttributes(RecentPath)!=INVALID_FILE_ATTRIBUTES)
			{
				RecentPath=GetPathDirectory(RecentPath);
				RecentPath+="\\*.*";
			}
			else
			{
				RecentPath="*.*";
			}
			
			FillFileList(RecentPath);
		}
		return true;
	case WM_D3DGUI_SIMPLE_LIST_LDBCLICK:
		if((CD3DWnd*)lParam==m_pFileList)
		{			
			OnSelectItem((int)wParam);
		}
		return true;
	case WM_D3DGUI_VISIBLE_CHANGE:
		if(wParam)
		{
			m_pFileList->SetFocus();
		}
		return true;
	}

	return false;
}

void CFileManagerWnd::UpdateRects()
{
	CD3DWnd::UpdateRects();
	CRect ClientRect=GetClientRect();
	
	if(m_pFileList)
	{
		CRect Rect;
		Rect=ClientRect;
		Rect.top+=5;
		Rect.bottom-=5;
		Rect.right-=5;
		Rect.left+=5;
		m_pFileList->SetRect(Rect);
	}

}

void CFileManagerWnd::FillFileList(LPCTSTR Dir)
{
	CEasyString SearchDir;
	SearchDir.Resize(MAX_PATH);
	_fullpath(SearchDir,Dir,MAX_PATH);
	SearchDir.TrimBuffer();
	if(m_pFileList)
	{
		CFileSearcher FileFind;

		LoadHistory(SearchDir);

		FileFind.FindFirst(SearchDir);

		m_pFileList->DeleteAllItem();
		m_CurFileInfoList.Clear();

		bool IsInDirectory=false;

		while(FileFind.FindNext())
		{
			FILE_INFO FileInfo;

			FileInfo.FilePath=FileFind.GetFilePath();
			FileInfo.FileName=FileFind.GetFileName();
			FileInfo.IsDirectory=FileFind.IsDirectory();
			FileInfo.IsDots=FileFind.IsDots();
			FileInfo.ShowText=FileInfo.FileName;
			FileInfo.ShowText.Replace("&","&amp;");
			FileInfo.ShowText.Replace("<","&lt;");
			FileInfo.ShowText.Replace(">","&gt;");
			if(IsInHistory(FileInfo.FileName))
				FileInfo.ShowText="<color=0xff808080>"+FileInfo.ShowText;			
			if(FileFind.IsDots())
				IsInDirectory=true;
			m_CurFileInfoList.Add(FileInfo);			
		}
		qsort(m_CurFileInfoList.GetBuffer(),m_CurFileInfoList.GetCount(),sizeof(FILE_INFO),FILE_INFO::Sort);
		if(!IsInDirectory)
		{
			SearchDir.MakeUpper();
			FileFind.FetchLogicalDrive();
			for(int i=0;i<FileFind.GetLogicalDriveCount();i++)
			{
				FILE_INFO FileInfo;
				FileInfo.FilePath=FileFind.GetLogicDriveName(i);				
				if(SearchDir.Find(FileInfo.FilePath)>=0)
					FileInfo.ShowText="<color=0xffff80ff>&lt;"+FileInfo.FilePath+"&gt;";
				else
					FileInfo.ShowText="<color=0xff80ffff>&lt;"+FileInfo.FilePath+"&gt;";
				FileInfo.IsDirectory=TRUE;
				FileInfo.IsDots=FALSE;
				m_CurFileInfoList.Insert(i,FileInfo);
			}
		}
		int SelectIndex=0;
		for(int i=0;i<(int)m_CurFileInfoList.GetCount();i++)
		{
			int Index;			

			Index=m_pFileList->AddItem(m_CurFileInfoList[i].ShowText);
			if(m_SelectedDir.CompareNoCase(m_CurFileInfoList[i].FilePath)==0)
				SelectIndex=Index;

			m_pFileList->SetItemData(Index,(LPVOID)i);
		}
		m_pFileList->SelectItem(SelectIndex);
		m_pFileList->MakeItemVisible(SelectIndex);
	}
}

void CFileManagerWnd::OnSelectItem(int Index)
{
	if(Index<0||Index>=m_pFileList->GetItemCount())
		return;
	UINT InfoIndex=(UINT)m_pFileList->GetItemData(Index);
	if(InfoIndex<m_CurFileInfoList.GetCount())
	{
		if(m_CurFileInfoList[InfoIndex].IsDirectory)
		{
			if(!m_CurFileInfoList[InfoIndex].IsDots)
				m_SelectedDir=m_CurFileInfoList[InfoIndex].FilePath;
			FillFileList(m_CurFileInfoList[InfoIndex].FilePath+"\\*.*");
		}
		else
		{
			SetVisible(false);
			m_pMain->OpenFile(m_CurFileInfoList[InfoIndex].FilePath);						
		}
	}
}

void CFileManagerWnd::LoadHistory(LPCTSTR Path)
{
	CODBCRecordSet RecordSet;

	m_CurHistoryList.Clear();

	CEasyString Directory=GetPathDirectory(Path);
	Directory.MakeUpper();

	CEasyString SQL;
	SQL.Format("Select FileName From History Where Directory='%s'",(LPCTSTR)Directory);
	if(m_DBConnection.ExecuteSQL(SQL)==DBERR_SUCCEED)
	{
		m_DBConnection.GetResults(&RecordSet);
		while(!RecordSet.IsEOF())
		{
			m_CurHistoryList.Add((LPCTSTR)RecordSet.GetField(0));
			RecordSet.MoveNext();
		}
	}


}
void CFileManagerWnd::AddHistory(LPCTSTR Path)
{
	CEasyString Directory=GetPathDirectory(Path);
	CEasyString FileName=GetPathFileName(Path);

	if(!IsInHistory(FileName))
	{	
		Directory.MakeUpper();
		FileName.MakeUpper();

		CEasyString SQL;
		SQL.Format("Insert into History(Directory,FileName) Values('%s','%s')",
			(LPCTSTR)Directory,(LPCTSTR)FileName);
		m_DBConnection.ExecuteSQL(SQL);

		for(int j=0;j<m_CurFileInfoList.GetCount();j++)
		{

			if(m_CurFileInfoList[j].FileName.CompareNoCase(FileName)==0)
			{
				for(int i=0;i<m_pFileList->GetItemCount();i++)
				{
					CEasyString ItemText;
					if(m_pFileList->GetItemText(i,ItemText))
					{
						if(ItemText.CompareNoCase(m_CurFileInfoList[j].ShowText)==0)
						{
							m_CurFileInfoList[j].ShowText="<color=0xff808080>"+m_CurFileInfoList[j].ShowText;
							m_pFileList->SetItemText(i,m_CurFileInfoList[j].ShowText);
							break;
						}
					}
				}
			}
		}

		
	}
}

bool CFileManagerWnd::IsInHistory(LPCTSTR FileName)
{	
	for(size_t i=0;i<m_CurHistoryList.GetCount();i++)
	{
		if(m_CurHistoryList[i].CompareNoCase(FileName)==0)
		{
			return true;
		}
	}
	return false;
}

void CFileManagerWnd::CheckDBFile()
{
	CWinFileAccessor HistoryFile;
	CResourceFileAccessor ResourceFile;

	CEasyString DBFileName=MakeModuleFullPath(NULL,"History.mdb");

	if(!HistoryFile.Open(DBFileName,IFileAccessor::modeRead|IFileAccessor::shareShareAll))
	{
		CEasyString ResourceName;
		ResourceName.Format("Module=%I64u;Type=DB;ID=%u",(INT64)NULL,IDR_DB_HISTORY);
		if(ResourceFile.Open(ResourceName,0))
		{
			if(HistoryFile.Open(DBFileName,IFileAccessor::modeCreate|IFileAccessor::modeWrite))
			{
				char Buffer[10240];
				int Size=10240;
				while(Size>=10240)
				{
					Size=ResourceFile.Read(Buffer,10240);
					HistoryFile.Write(Buffer,Size);
				}
			}
		}
	}
}