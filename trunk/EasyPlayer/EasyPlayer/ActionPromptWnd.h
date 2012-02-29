/****************************************************************************/
/*                                                                          */
/*      文件名:    ActionPromptWnd.h                                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CActionPromptWnd :
	public CD3DWnd
{
protected:
	CEasyTimer		m_ShowTimer;

	DECLARE_CLASS_INFO(CActionPromptWnd)
public:
	CActionPromptWnd(void);
	CActionPromptWnd(CD3DGUI * pGUI);
	~CActionPromptWnd(void);

	void Update();
	void ShowPrompt(LPCTSTR Msg);
};
