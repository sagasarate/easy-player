/****************************************************************************/
/*                                                                          */
/*      文件名:    MainFrm.cpp                                              */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "EasyPlayer.h"

#include "MainFrm.h"
#include ".\mainfrm.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FULL_SCREEN, OnFullScreen)
	ON_WM_SIZING()
	ON_WM_ERASEBKGND()

	ON_COMMAND(ID_SHOW_FRAME_RATE, OnShowFrameRate)
	ON_UPDATE_COMMAND_UI(ID_SHOW_FRAME_RATE, OnUpdateShowFrameRate)
	ON_COMMAND(ID_SHOW_CONTROL_BAR, OnShowControlBar)
	ON_UPDATE_COMMAND_UI(ID_SHOW_CONTROL_BAR, OnUpdateShowControlBar)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_PLAY_PAUSE, OnPlayPause)
	ON_COMMAND(ID_STOP, OnStop)
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()

	ON_COMMAND(ID_HIDE_CONTROL_BAR, OnHideControlBar)
	ON_COMMAND(ID_TOP_CONTROL_BAR, OnTopControlBar)
	ON_UPDATE_COMMAND_UI(ID_HIDE_CONTROL_BAR, OnUpdateHideControlBar)
	ON_UPDATE_COMMAND_UI(ID_TOP_CONTROL_BAR, OnUpdateTopControlBar)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_DISABLE_WHEEL, &CMainFrame::OnDisableWhell)
	ON_UPDATE_COMMAND_UI(ID_DISABLE_WHEEL, &CMainFrame::OnUpdateDisableWhell)
	ON_COMMAND(ID_FORCE_LOAD_DIRECT_VOB_SUB, &CMainFrame::OnForceLoadDirectVobSub)
	ON_UPDATE_COMMAND_UI(ID_FORCE_LOAD_DIRECT_VOB_SUB, &CMainFrame::OnUpdateForceLoadDirectVobSub)
END_MESSAGE_MAP()


// CMainFrame 构造/析构



CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码

	m_pD3DDevice=NULL;
	m_pUIRender=NULL;	
	m_pObjectCreator=NULL;
	m_pGUI=NULL;
	m_pVideoRect=NULL;
	m_SavedMenu=NULL;

	m_pFrameRateWnd=NULL;
	m_pControlBar=NULL;
	m_pFileManagerWnd=NULL;
	m_pActionPromptWnd=NULL;


	m_IsSizing=false;
	m_IsFullScreen=false;

	m_UpdateStartTime=0;

	m_hCursor=NULL;
	m_ShowCursor=true;

	m_IsDisableWheel=false;


	m_ForceLoadDirectVobSub=false;
}

CMainFrame::~CMainFrame()
{
	SAFE_RELEASE(m_pControlBar);
	SAFE_RELEASE(m_pFileManagerWnd);
	SAFE_RELEASE(m_pFrameRateWnd);
	SAFE_RELEASE(m_pActionPromptWnd);
	if(m_SavedMenu)
		SetMenu(m_SavedMenu);
	if(m_hCursor)
		DeleteObject(m_hCursor);
	ReleaseD3D();
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect WindowRect,ClientRect;
	GetWindowRect(&WindowRect);
	GetClientRect(&ClientRect);
	WindowRect.right+=400-ClientRect.Width();
	WindowRect.bottom+=300-ClientRect.Height();
	MoveWindow(&WindowRect);	

	if(!InitD3D())
		return -1;
	
	m_pVideoRect=new CVideoRect();
	m_pUIRender->AddObject(m_pVideoRect);

	CResourceFileAccessor ResourceFile;
	CEasyString ResourceName;
	ResourceName.Format("Module=%I64u;Type=USO;ID=%u",(INT64)NULL,IDR_USO_UI);
	ResourceFile.Open(ResourceName,0);

	CUSOResourceManager USOFile;
	CUSOGUIObjectCreateFilter GUIFilter(m_pGUI);
	CUSOD3DObjectCreateFilter D3DFilter(m_pD3DDevice);

	USOFile.AddObjectCreateFilter(&GUIFilter);
	USOFile.AddObjectCreateFilter(&D3DFilter);
	USOFile.AddObjectCreateInfoByName("ControlBar",&GET_CLASS_INFO(CVideoControlBar));
	USOFile.AddObjectCreateInfoByName("FileWnd",&GET_CLASS_INFO(CFileManagerWnd));
	USOFile.AddObjectCreateInfoByName("ActionPrompt",&GET_CLASS_INFO(CActionPromptWnd));
	if(USOFile.Import(&ResourceFile))
	{
		m_pFrameRateWnd=dynamic_cast<CD3DWnd *>(USOFile.GetObject("FrameRate"));		
		m_pFrameRateWnd->AddUseRef();
		m_pFrameRateWnd->SetVisible(false);

		m_pControlBar=dynamic_cast<CVideoControlBar *>(USOFile.GetObject("ControlBar"));
		m_pControlBar->AddUseRef();
		m_pControlBar->SetVideo(m_pVideoRect);
		m_pControlBar->SetVisible(false);

		m_pFileManagerWnd=dynamic_cast<CFileManagerWnd *>(USOFile.GetObject("FileWnd"));		
		m_pFileManagerWnd->AddUseRef();
		m_pFileManagerWnd->SetMainWnd(this);
		m_pFileManagerWnd->SetVisible(false);

		m_pActionPromptWnd=dynamic_cast<CActionPromptWnd *>(USOFile.GetObject("ActionPrompt"));		
		m_pActionPromptWnd->AddUseRef();
		m_pActionPromptWnd->SetVisible(false);
	}
	else
	{
		return -1;
	}


	



	m_hCursor=LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW));

	m_UpdateStartTime=timeGetTime();

	m_FrameCount=0;
	m_FrameRateTestTimer.SaveTime();
	m_IsSizing=true;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);
	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	//m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CMainFrame::InitD3D()
{	
	m_pD3DDevice=new CD3DDevice();

	D3DDEVICE_PARAMS D3DParams;
	
	D3DParams.PresentParams.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
	D3DParams.hFocusWindow=GetSafeHwnd();

	if(!m_pD3DDevice->Create(D3DParams))
		return false;

	m_pUIRender=new CD3DUIRender();	

	m_pUIRender->Create(m_pD3DDevice);

	m_pObjectCreator=new CD3DGUIObjectCreator(m_pUIRender);

	m_pGUI=new CD3DGUI(m_pObjectCreator,GetSafeHwnd());

	

	return TRUE;
}

void CMainFrame::ReleaseD3D()
{
	SAFE_RELEASE(m_pVideoRect);
	SAFE_RELEASE(m_pGUI);
	SAFE_RELEASE(m_pObjectCreator);
	SAFE_RELEASE(m_pUIRender);	
	SAFE_RELEASE(m_pD3DDevice);	
}



void CMainFrame::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog Dlg(true,NULL,NULL,0,"Video Files(*.avi;*.rmvb;*.wma;*.asf;*.mkv;*.rm;*.ogg)|*.avi;*.rmvb;*.wma;*.asf;*.mkv;*.rm;*.ogg|All Files(*.*)|*.*||");

	if(Dlg.DoModal()==IDOK)
	{
		m_pFileManagerWnd->FillFileList(GetPathDirectory(Dlg.GetPathName())+"\\*.*");
		OpenFile(Dlg.GetPathName());
	}
	
}


void CMainFrame::OnFullScreen()
{
	// TODO: 在此添加命令处理程序代码
	m_IsFullScreen=!m_IsFullScreen;

	if(m_IsFullScreen)
	{
		GetWindowRect(&m_SavedSize);
		m_SavedMenu=GetMenu();
		SetMenu(NULL);
		ModifyStyle(WS_OVERLAPPEDWINDOW,WS_POPUP);
		ModifyStyleEx(0,WS_EX_TOPMOST);
		CRect Rect(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
		MoveWindow(&Rect,true);		
		
	}
	else
	{		
		SetMenu(m_SavedMenu);
		m_SavedMenu=NULL;
		ModifyStyle(WS_POPUP,WS_OVERLAPPEDWINDOW);
		ModifyStyleEx(WS_EX_TOPMOST,0);
		MoveWindow(&m_SavedSize,true);
	}
	m_IsSizing=true;

}


BOOL CMainFrame::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类	
	
	return CFrameWnd::DestroyWindow();
}


void CMainFrame::Update()
{	
	

	if(m_IsSizing)
	{
		m_IsSizing=false;
		if(m_pD3DDevice)
		{
			m_pD3DDevice->GetFXManager()->Reset();
			m_pD3DDevice->GetFontManager()->Reset();
			m_pD3DDevice->GetTextureManager()->Reset();
			m_pUIRender->Reset();
			if(!m_pD3DDevice->Reset())
			{
				MessageBox("D3D设备Reset失败！");
				return;
			}
			m_pUIRender->Restore();
			m_pD3DDevice->GetFXManager()->Restore();
			m_pD3DDevice->GetFontManager()->Restore();
			m_pD3DDevice->GetTextureManager()->Restore();
		}
		CRect ClientRect;
		GetClientRect(&ClientRect);

		if(m_pVideoRect)
		{
			if(m_pVideoRect->GetVideoTexture())
			{
				CRect Rect=GetRatioRect(ClientRect);
				Rect.OffsetRect((ClientRect.Width()-Rect.Width())/2,
					(ClientRect.Height()-Rect.Height())/2);
				m_pVideoRect->SetRect(Rect);
			}
		}

		UpdateControlBarPostion();

		CRect Rect;


		Rect=ClientRect;
		Rect.top+=16;
		Rect.left+=16;
		Rect.bottom-=16;
		Rect.right-=16;
		m_pFileManagerWnd->SetRect(Rect);

		Rect=ClientRect;

		CRect FrameRateRect=m_pFrameRateWnd->GetRect();
		Rect.top+=32;
		Rect.left+=32;
		Rect.bottom=Rect.top+FrameRateRect.Height();
		Rect.right=Rect.left+FrameRateRect.Width();
		m_pFrameRateWnd->SetRect(Rect);

		Rect=ClientRect;

		CRect PromptRect=m_pActionPromptWnd->GetRect();
		Rect.top+=32;
		Rect.right-=32;
		Rect.bottom=Rect.top+PromptRect.Height();
		Rect.left=Rect.right-PromptRect.Width();
		m_pActionPromptWnd->SetRect(Rect);
		

	}	

	m_pControlBar->Update();
	m_pActionPromptWnd->Update();

	if(m_ShowCursor)
	{	
		if(m_CursorShowTimer.IsTimeOut(CURSOR_SHOW_TIME))
		{
			m_ShowCursor=false;
			SetCursor(NULL);			
		}
	}
	
	m_FrameCount++;
	if(m_FrameRateTestTimer.IsTimeOut(3000))
	{
		CString Rate;
		Rate.Format("%d",m_FrameCount/3);
		m_pFrameRateWnd->SetText(Rate);
		m_FrameRateTestTimer.SaveTime();
		m_FrameCount=0;
	}

	float time=(timeGetTime()-m_UpdateStartTime)/1000.0f;
	m_pD3DDevice->GetTextureManager()->Update(time);
	m_pGUI->Update();
	m_pUIRender->Update(time);	
	m_pD3DDevice->StartRender(D3DCOLOR_XRGB(0,0,0));	
	m_pUIRender->Render();
	m_pD3DDevice->EndRender();	

	Sleep(0);
}


void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
	CFrameWnd::OnSizing(fwSide, pRect);
	m_IsSizing=true;	

	// TODO: 在此处添加消息处理程序代码
}



BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(CFrameWnd::PreTranslateMessage(pMsg))
		return true;

	if(pMsg->message==WM_SYSCOMMAND&&pMsg->wParam==SC_SCREENSAVE)
		return TRUE;

	if(m_pGUI)
	{
		if(pMsg->message==WM_MOUSEWHEEL)
		{		
			POINT point;
			point.x=LOWORD(pMsg->lParam);
			point.y=HIWORD(pMsg->lParam);
			ScreenToClient(&point);
			pMsg->lParam=point.y<<16|point.x;
		}

		if(m_pGUI->HandleMessage(pMsg->message, pMsg->wParam, pMsg->lParam ))
			return true;
	}

	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
			case VK_UP:
				m_pControlBar->Seek(VST_OFFSET,5);				
				return true;
			case VK_DOWN:
				m_pControlBar->Seek(VST_OFFSET,-5);
				return true;
			}
		}
		break;
	case WM_KEYUP:
		{
			switch(pMsg->wParam)
			{
			case VK_RETURN:
				if(!m_pFileManagerWnd->IsVisible())
				{
					m_pFileManagerWnd->SetVisible(true);
					return true;
				}
			case VK_ESCAPE:
				{
					if(m_IsFullScreen)
						OnFullScreen();
					else
						PostMessage(WM_QUIT);
				}
				return true;
				
			}
		}
		break;
	case WM_MOUSEWHEEL:
		if(m_pVideoRect->GetVideoTexture()&&(!m_IsDisableWheel))
		{
			if(GET_WHEEL_DELTA_WPARAM(pMsg->wParam)>0)
				m_pControlBar->Seek(VST_OFFSET,5);
			else
				m_pControlBar->Seek(VST_OFFSET,-5);
		}
		return true;
	}

	return false;
	
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RECT Rect;
	GetClientRect(&Rect);
	RECT VideoRect={0,0,0,0};
	if(m_pVideoRect)
	{
		VideoRect=m_pVideoRect->GetRect();
	}

	if(Rect.right>=VideoRect.right&&Rect.bottom>=VideoRect.bottom)
	{
		pDC->FillSolidRect(&Rect,0);
	}
	return TRUE;
}


RECT CMainFrame::GetRatioRect(CRect Rect)
{
	RECT RatioRect={0,0,0,0};

	
	if(m_pVideoRect)
	{
		int Width,Height;
		float VideoRatio;
		m_pVideoRect->GetVideoTexture()->GetVideoSize(Width,Height);
		VideoRatio=(float)Height/Width;
		float RectVideo=(float)Rect.Height()/Rect.Width();
		if(VideoRatio<RectVideo)
		{
			RatioRect.right=Rect.Width();
			RatioRect.bottom=(int)(Rect.Width()*VideoRatio);
		}
		else
		{
			RatioRect.bottom=Rect.Height();
			RatioRect.right=(int)(Rect.Height()/VideoRatio);
		}
	}
	return RatioRect;
}

bool CMainFrame::OpenFile(LPCTSTR FileName)
{
	if(m_pVideoRect->Create(FileName,m_ForceLoadDirectVobSub))
	{
		m_pVideoRect->GetVideoTexture()->SetTimeUnit(VTU_SECOND);
		m_pControlBar->OnLoadVideo();
		m_IsSizing=true;
		m_pFileManagerWnd->AddHistory(MakeFullPath(FileName));
		theApp.WriteProfileString("History","RecentPlayFile",FileName);
		return true;
	}
	return false;
}

void CMainFrame::OnShowFrameRate()
{
	// TODO: 在此添加命令处理程序代码
	m_pFrameRateWnd->SetVisible(!m_pFrameRateWnd->IsVisible());
}

void CMainFrame::OnUpdateShowFrameRate(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_pFrameRateWnd->IsVisible());
}

void CMainFrame::OnShowControlBar()
{
	// TODO: 在此添加命令处理程序代码
	m_pControlBar->SetVisible(!m_pControlBar->IsVisible());

}

void CMainFrame::OnUpdateShowControlBar(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_pControlBar->IsVisible());
}

void CMainFrame::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFrameWnd::OnLButtonDblClk(nFlags, point);
}

void CMainFrame::OnPlayPause()
{
	// TODO: 在此添加命令处理程序代码
	if(m_pControlBar->PlayPause())
		m_pActionPromptWnd->ShowPrompt("Play");
	else
		m_pActionPromptWnd->ShowPrompt("Pause");
}

void CMainFrame::OnStop()
{
	// TODO: 在此添加命令处理程序代码
	m_pControlBar->Stop();
	m_pActionPromptWnd->ShowPrompt("Stop");
}

BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_ShowCursor)
		SetCursor(m_hCursor);
	else
		SetCursor(NULL);

	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static CPoint OldPoint;

	if(OldPoint!=point)
	{
		m_ShowCursor=true;
		m_CursorShowTimer.SaveTime();
		OldPoint=point;
	}
	
	CFrameWnd::OnMouseMove(nFlags, point);
}



void CMainFrame::OnHideControlBar()
{
	// TODO: 在此添加命令处理程序代码
	m_pControlBar->ForceHide(!m_pControlBar->IsForceHide());
	
}

void CMainFrame::OnTopControlBar()
{
	// TODO: 在此添加命令处理程序代码
	m_pControlBar->EnableTopShow(!m_pControlBar->IsTopShow());
	UpdateControlBarPostion();
}

void CMainFrame::OnUpdateHideControlBar(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_pControlBar->IsForceHide());	
}

void CMainFrame::OnUpdateTopControlBar(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_pControlBar->IsTopShow());	
}

void CMainFrame::UpdateControlBarPostion()
{
	CRect ClientRect;
	GetClientRect(&ClientRect);

	CRect SeekerRect=m_pControlBar->GetRect();
	CRect Rect=ClientRect;

	Rect.left+=ClientRect.Width()*0.05f;
	Rect.right-=ClientRect.Width()*0.05f;
	if(m_pControlBar->IsTopShow())
	{
		Rect.top+=ClientRect.Height()*0.1f;
		Rect.bottom=Rect.top+SeekerRect.Height();
	}
	else
	{
		Rect.bottom-=ClientRect.Height()*0.1f;
		Rect.top=Rect.bottom-SeekerRect.Height();
	}	
	m_pControlBar->SetRect(Rect);
}

void CMainFrame::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//if (point.x == -1 && point.y == -1)
	//{		
	//	CRect rect;
	//	GetClientRect(rect);
	//	ClientToScreen(rect);

	//	point = rect.TopLeft();
	//	point.Offset(5, 5);
	//}
	//else ClientToScreen(&point);

	//CMenu menu;
	//VERIFY(menu.LoadMenu(IDR_POPUP));

	//CMenu* pPopup = menu.GetSubMenu(0);
	//ASSERT(pPopup != NULL);
	//CWnd* pWndPopupOwner = this;

	//while (pWndPopupOwner->GetStyle() & WS_CHILD)
	//	pWndPopupOwner = pWndPopupOwner->GetParent();

	//
	//pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
	//	pWndPopupOwner);
	
	CFrameWnd::OnRButtonUp(nFlags, point);
}

void CMainFrame::OnDisableWhell()
{
	// TODO: 在此添加命令处理程序代码
	m_IsDisableWheel=!m_IsDisableWheel;
}

void CMainFrame::OnUpdateDisableWhell(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_IsDisableWheel);	
}

void CMainFrame::OnForceLoadDirectVobSub()
{
	// TODO: 在此添加命令处理程序代码
	m_ForceLoadDirectVobSub=!m_ForceLoadDirectVobSub;
}

void CMainFrame::OnUpdateForceLoadDirectVobSub(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_ForceLoadDirectVobSub);
}
