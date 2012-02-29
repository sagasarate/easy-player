/****************************************************************************/
/*                                                                          */
/*      文件名:    MainFrm.h                                                */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// MainFrm.h : CMainFrame 类的接口
//


#pragma once

//#include "ChildView.h"
class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	//CChildView    m_wndView;

// 生成的消息映射函数
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


