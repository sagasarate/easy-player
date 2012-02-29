/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ActionPromptWnd.cpp                                      */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\actionpromptwnd.h"
#define PROMPT_MSG_SHOW_TIME		(3*1000)

IMPLEMENT_CLASS_INFO(CActionPromptWnd,CD3DWnd);

CActionPromptWnd::CActionPromptWnd(void)
{
}

CActionPromptWnd::CActionPromptWnd(CD3DGUI * pGUI):CD3DWnd(pGUI)
{
}

CActionPromptWnd::~CActionPromptWnd(void)
{
}


void CActionPromptWnd::Update()
{
	if(IsVisible())
	{
		if(m_ShowTimer.IsTimeOut(PROMPT_MSG_SHOW_TIME))
		{
			SetVisible(false);
		}
	}
}

void CActionPromptWnd::ShowPrompt(LPCTSTR Msg)
{
	SetText(Msg);
	m_ShowTimer.SaveTime();
	SetVisible(true);
}
