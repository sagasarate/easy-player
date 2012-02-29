/****************************************************************************/
/*                                                                          */
/*      文件名:    VideoControlBar.cpp                                      */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include "VideoControlBar.h"

#define PROMPT_MSG_SHOW_TIME	(5*1000)

IMPLEMENT_CLASS_INFO(CVideoControlBar,CD3DWnd);

CVideoControlBar::CVideoControlBar(void):CD3DWnd()
{
	m_pVideo=NULL;
	m_pProgressBar=NULL;
	m_pTimeLengthWnd=NULL;
	m_ForceHide=false;
	m_IsTopShow=false;
}

CVideoControlBar::CVideoControlBar(CD3DGUI * pGUI):CD3DWnd(pGUI)
{
	m_pVideo=NULL;
	m_pProgressBar=NULL;
	m_pTimeLengthWnd=NULL;
	m_ForceHide=false;
	m_IsTopShow=false;
}

CVideoControlBar::~CVideoControlBar(void)
{
}


BOOL CVideoControlBar::OnMessage(CD3DWnd * pWnd,UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(CD3DWnd::OnMessage(pWnd,msg,wParam,lParam))
		return true;
	switch(msg)
	{
	case WM_LBUTTONDOWN:
		if(m_pVideo&&m_pProgressBar)
		{
			if(m_pVideo->GetVideoTexture())
			{
				POINT Point;
				Point.x=GET_X_LPARAM(lParam);
				Point.y=GET_Y_LPARAM(lParam);
				ScreenToClient(&Point);
				LONGLONG Pos=m_pProgressBar->GetPosByXY(Point.x,Point.y);
				m_pVideo->GetVideoTexture()->SetCurPos(Pos);
			}
		}
		return true;
	case WM_D3DGUI_CHILD_LOADED:
		if((CD3DWnd*)lParam==this)
		{
			m_pProgressBar=(CD3DProgressBar *)GetChildWndByName("Seeker");
			m_pProgressBar->SetPos(0);
			m_pProgressBar->SetMaxPos(0);
			m_pTimeLengthWnd=(CD3DWnd *)m_pProgressBar->GetChildWndByName("TimeLength");
		}
		return true;	
	}
	return false;
}

void CVideoControlBar::UpdateRects()
{
	CD3DWnd::UpdateRects();
	CRect Rect=GetClientRect();
	if(m_pProgressBar)
	{		
		CRect ProgressBarRect=m_pProgressBar->GetRect();
		ProgressBarRect.right=ProgressBarRect.left+Rect.Width()-100;
		m_pProgressBar->SetRect(ProgressBarRect);
		if(m_pTimeLengthWnd)
		{
			ProgressBarRect=m_pProgressBar->GetClientRect();
			m_pTimeLengthWnd->SetRect(ProgressBarRect);
		}
	}
	
}

void CVideoControlBar::SetVisible(bool IsVisible)
{
	if(m_ForceHide)
	{
		CD3DWnd::SetVisible(false);
	}
	else	
	{
		CD3DWnd::SetVisible(IsVisible);
	}
	
	m_ShowTimer.SaveTime();
}

void CVideoControlBar::Seek(int SeekType,LONGLONG Time)
{
	SetVisible(true);
	if(m_pVideo->GetVideoTexture())
	{
		switch(SeekType)
		{
		case VST_ORG:
			m_pVideo->GetVideoTexture()->SetCurPos(Time);
			break;
		case VST_OFFSET:
			{
				LONGLONG CurTime=m_pVideo->GetVideoTexture()->GetCurPos();
				CurTime+=Time;
				m_pVideo->GetVideoTexture()->SetCurPos(CurTime);
			}
			break;
		}
		
	}
}

void CVideoControlBar::Pause()
{
	CD3DVideoTexture * pVideo=m_pVideo->GetVideoTexture();
	if(pVideo)
	{
		pVideo->Pause();
	}
}

void CVideoControlBar::Play()
{
	CD3DVideoTexture * pVideo=m_pVideo->GetVideoTexture();
	if(pVideo)
	{
		pVideo->Run();
	}
}

void CVideoControlBar::Stop()
{
	CD3DVideoTexture * pVideo=m_pVideo->GetVideoTexture();
	if(pVideo)
	{			
		pVideo->Stop();		
	}
}

int CVideoControlBar::GetPlayStatus()
{
	CD3DVideoTexture * pVideo=m_pVideo->GetVideoTexture();
	if(pVideo)
	{
		return pVideo->GetPlayStatus();
	}
	return VPS_NONE;
}

bool CVideoControlBar::PlayPause()
{
	if(GetPlayStatus()==VPS_PLAYING)
	{
		Pause();
		return false;
	}
	else
	{
		Play();
		return true;
	}
}

void CVideoControlBar::OnLoadVideo()
{
	if(m_pProgressBar&&m_pVideo->GetVideoTexture())
	{
		LONGLONG Time=m_pVideo->GetVideoTexture()->GetMaxPos();
		m_pProgressBar->SetMaxPos(Time);		
		UpdateTimeLength();
		Play();
	} 
}

void CVideoControlBar::SynchProgress()
{
	if(m_pProgressBar&&m_pVideo->GetVideoTexture())
	{
		LONGLONG Time=m_pVideo->GetVideoTexture()->GetCurPos();
		m_pProgressBar->SetPos(Time);		
		UpdateTimeLength();
	}
}

void CVideoControlBar::Update()
{
	if(IsVisible())
	{
		if(m_ShowTimer.IsTimeOut(PROMPT_MSG_SHOW_TIME))
		{
			SetVisible(false);
		}
	}
	SynchProgress();
}

void CVideoControlBar::UpdateTimeLength()
{
	if(m_pProgressBar&&m_pTimeLengthWnd)
	{
		char buff[100];
		int h1,m1,s1;
		int h2,m2,s2;
		int CurTime=(int)m_pProgressBar->GetPos();
		int TimeLength=(int)m_pProgressBar->GetMaxPos();
		h1=CurTime/3600;
		m1=(CurTime%3600)/60;
		s1=CurTime%60;
		h2=TimeLength/3600;
		m2=(TimeLength%3600)/60;
		s2=TimeLength%60;

		if(h2)
			sprintf_s(buff,100,"%d:%02d:%02d/%d:%02d:%02d",h1,m1,s1,h2,m2,s2);
		else
			sprintf_s(buff,100,"%02d:%02d/%02d:%02d",m1,s1,m2,s2);
		m_pTimeLengthWnd->SetText(buff);
	}
}