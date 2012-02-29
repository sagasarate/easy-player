/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ActionPromptWnd.h                                        */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
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
