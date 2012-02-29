/****************************************************************************/
/*                                                                          */
/*      �ļ���:    MainFrm.h                                                */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once

//#include "ChildView.h"
class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	//CChildView    m_wndView;

// ���ɵ���Ϣӳ�亯��
protected:
	CD3DDevice *		m_pD3DDevice;
	CD3DUIRender *		m_pUIRender;	
	CD3DGUIObjectCreator *	m_pObjectCreator;
	CD3DGUI *			m_pGUI;	

	bool			m_IsSizing;	
	bool			m_IsFullScreen;

	RECT			m_SavedSize;
	CMenu*			m_SavedMenu;
	
	DWORD			m_UpdateStartTime;
	int				m_FrameCount;
	CEasyTimer		m_FrameRateTestTimer;

	CVideoRect*		m_pVideoRect;

	CD3DWnd*			m_pFrameRateWnd;
	CVideoControlBar *	m_pControlBar;
	CFileManagerWnd*	m_pFileManagerWnd;
	CActionPromptWnd*	m_pActionPromptWnd;

	HCURSOR				m_hCursor;
	bool				m_ShowCursor;
	CEasyTimer			m_CursorShowTimer;

	bool				m_IsDisableWheel;

	bool				m_ForceLoadDirectVobSub;

	

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
protected:
	BOOL InitDirectShow();	
	void ReleaseDirectShow();
	BOOL InitD3D();
	void ReleaseD3D();
	void ResizeWindowForVideo();
	BOOL PlayFile(LPCTSTR FileName);	
public:
	void Update();

protected:
	RECT GetRatioRect(CRect Rect);	
	void UpdateControlBarPostion();
public:
	bool OpenFile(LPCTSTR FileName);


	afx_msg void OnFileOpen();
	afx_msg void OnFullScreen();
	virtual BOOL DestroyWindow();

	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowFrameRate();
	afx_msg void OnUpdateShowFrameRate(CCmdUI *pCmdUI);
	afx_msg void OnShowControlBar();
	afx_msg void OnUpdateShowControlBar(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPlayPause();
	afx_msg void OnStop();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	afx_msg void OnHideControlBar();
	afx_msg void OnTopControlBar();
	afx_msg void OnUpdateHideControlBar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTopControlBar(CCmdUI *pCmdUI);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDisableWhell();
	afx_msg void OnUpdateDisableWhell(CCmdUI *pCmdUI);
	afx_msg void OnForceLoadDirectVobSub();
	afx_msg void OnUpdateForceLoadDirectVobSub(CCmdUI *pCmdUI);
};


