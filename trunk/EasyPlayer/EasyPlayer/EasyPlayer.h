/****************************************************************************/
/*                                                                          */
/*      文件名:    EasyPlayer.h                                             */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// EasyPlayer.h : EasyPlayer 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h” 
#endif

#include "resource.h"       // 主符号


// CEasyPlayerApp:
// 有关此类的实现，请参阅 EasyPlayer.cpp
//

class CEasyPlayerApp : public CWinApp
{
public:
	CEasyPlayerApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	virtual int Run();	
	virtual int ExitInstance();
};

extern CEasyPlayerApp theApp;
