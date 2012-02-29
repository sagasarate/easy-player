/****************************************************************************/
/*                                                                          */
/*      �ļ���:    VideoRect.h                                              */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CVideoRect :
	public CD3DObject
{
protected:
	CD3DVideoTexture *		m_pVideoTexture;
	CD3DSubMesh				m_SubMesh;	
	FLOAT_RECT				m_Rect;	
	RECTVERTEX 				m_Vertexs[6];
public:
	CVideoRect(void);
	virtual ~CVideoRect(void);

	virtual void Destory();

	BOOL Create(LPCTSTR FileName,bool ForceLoadDirectVobSub);

	void SetRect(FLOAT_RECT Rect);
	FLOAT_RECT GetRect();

	CD3DVideoTexture * GetVideoTexture();

	virtual int GetSubMeshCount()
	{
		return 1;
	}
	virtual CD3DSubMesh * GetSubMesh(UINT index)
	{
		return &m_SubMesh;
	}	

	virtual void OnPrepareRender(CD3DBaseRender * pRender,CD3DFX * pFX,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera);
	virtual void OnPrepareRenderSubMesh(CD3DBaseRender * pRender,CD3DFX * pFX,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera);

protected:
	void CreateVertex();
};

inline void CVideoRect::SetRect(FLOAT_RECT Rect)
{
	m_Rect=Rect;
	CreateVertex();
}
inline FLOAT_RECT CVideoRect::GetRect()
{
	return m_Rect;
}
inline CD3DVideoTexture * CVideoRect::GetVideoTexture()
{
	return m_pVideoTexture;
}
