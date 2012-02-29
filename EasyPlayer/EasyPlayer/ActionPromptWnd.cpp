/****************************************************************************/
/*                                                                          */
/*      文件名:    ActionPromptWnd.cpp                                      */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
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
