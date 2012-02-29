/****************************************************************************/
/*                                                                          */
/*      文件名:    EasyPlayer.cpp                                           */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// EasyPlayer.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "EasyPlayer.h"
#include "MainFrm.h"
#include ".\easyplayer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEasyPlayerApp

BEGIN_MESSAGE_MAP(CEasyPlayerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()


// CEasyPlayerApp 构造

CEasyPlayerApp::CEasyPlayerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CEasyPlayerApp 对象

CEasyPlayerApp theApp;

// CEasyPlayerApp 初始化

BOOL CEasyPlayerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。

	CExceptionParser::GetInstance()->Init(EXCEPTION_SET_TRANSLATOR);

	InitCommonControls();

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象



	DbgSetModuleLevel(LOG_TRACE,3);



	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建并加载带有其资源的框架
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);


	EnableShellOpen();
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	
	if(cmdInfo.m_nShellCommand==CCommandLineInfo::FileOpen)
	{
		pFrame->OpenFile(cmdInfo.m_strFileName);
	}
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	
	// 仅当存在后缀时才调用 DragAcceptFiles，
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生
	return TRUE;
}


// CEasyPlayerApp 消息处理程序



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CEasyPlayerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CEasyPlayerApp 消息处理程序
int CEasyPlayerApp::Run()
{
	ASSERT_VALID(this);

	if (m_pMainWnd == NULL && AfxOleGetUserCtrl())
	{
		// Not launched /Embedding or /Automation, but has no main window!
		TRACE(traceAppMsg, 0, "Warning: m_pMainWnd is NULL in CWinApp::Run - quitting application.\n");
		AfxPostQuitMessage(0);
	}


	_AFX_THREAD_STATE* pState = AfxGetThreadState();

	// for tracking the idle time state
	

	// acquire and dispatch messages until a WM_QUIT message is received.
	for (;;)
	{
		if(m_pMainWnd)
			((CMainFrame *)m_pMainWnd)->Update();		
		if(::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE))
		{
			// pump message, but quit on WM_QUIT
			if (!PumpMessage())
				return ExitInstance();

			// reset "no idle" state after pumping "normal" message
			//if (IsIdleMessage(&m_msgCur))
		}
	}
}



int CEasyPlayerApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	SAFE_DELETE(m_pMainWnd);
	CLogManager::ReleaseInstance();
	CPerformanceStatistician::ReleaseInstance();
	return CWinApp::ExitInstance();
}
