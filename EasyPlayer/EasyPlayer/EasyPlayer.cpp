/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EasyPlayer.cpp                                           */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// EasyPlayer.cpp : ����Ӧ�ó��������Ϊ��
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


// CEasyPlayerApp ����

CEasyPlayerApp::CEasyPlayerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CEasyPlayerApp ����

CEasyPlayerApp theApp;

// CEasyPlayerApp ��ʼ��

BOOL CEasyPlayerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�

	CExceptionParser::GetInstance()->Init(EXCEPTION_SET_TRANSLATOR);

	InitCommonControls();

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���



	DbgSetModuleLevel(LOG_TRACE,3);



	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ���������ش�������Դ�Ŀ��
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
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	
	// �������ں�׺ʱ�ŵ��� DragAcceptFiles��
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����
	return TRUE;
}


// CEasyPlayerApp ��Ϣ�������



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CEasyPlayerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CEasyPlayerApp ��Ϣ�������
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
	// TODO: �ڴ����ר�ô����/����û���
	SAFE_DELETE(m_pMainWnd);
	CLogManager::ReleaseInstance();
	CPerformanceStatistician::ReleaseInstance();
	return CWinApp::ExitInstance();
}
