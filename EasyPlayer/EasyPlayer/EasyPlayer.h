/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EasyPlayer.h                                             */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// EasyPlayer.h : EasyPlayer Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CEasyPlayerApp:
// �йش����ʵ�֣������ EasyPlayer.cpp
//

class CEasyPlayerApp : public CWinApp
{
public:
	CEasyPlayerApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	virtual int Run();	
	virtual int ExitInstance();
};

extern CEasyPlayerApp theApp;
