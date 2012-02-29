/****************************************************************************/
/*                                                                          */
/*      文件名:    VideoControlBar.h                                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

enum VIDEO_SEEK_TYPE
{
	VST_ORG,
	VST_OFFSET,
};

class CVideoControlBar :
	public CD3DWnd
{
protected:
	CVideoRect *		m_pVideo;
	CD3DProgressBar *	m_pProgressBar;
	CD3DWnd *			m_pTimeLengthWnd;
	CEasyTimer			m_ShowTimer;
	bool				m_ForceHide;
	bool				m_IsTopShow;

	DECLARE_CLASS_INFO(CVideoControlBar)
public:
	CVideoControlBar(void);
	CVideoControlBar(CD3DGUI * pGUI);	
	virtual ~CVideoControlBar(void);

	virtual BOOL OnMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void UpdateRects();
	virtual void SetVisible(bool IsVisible);

	void SetVideo(CVideoRect * pVideo)
	{
		m_pVideo=pVideo;
	}

	CD3DProgressBar * GetProgressBar()
	{
		return m_pProgressBar;
	}
	CD3DWnd * GetTimeLengthWnd()
	{
		return m_pTimeLengthWnd;
	}

	void ForceHide(bool IsForceHide)
	{
		m_ForceHide=IsForceHide;
	}
	bool IsForceHide()
	{
		return m_ForceHide;
	}
	void EnableTopShow(bool Enable)
	{
		m_IsTopShow=Enable;
	}
	bool IsTopShow()
	{
		return m_IsTopShow;
	}
	void Seek(int SeekType,LONGLONG Time);
	void Pause();
	void Play();
	void Stop();
	int GetPlayStatus();
	bool PlayPause();

	void OnLoadVideo();
	void SynchProgress();
	void Update();
protected:
	void UpdateTimeLength();
};
