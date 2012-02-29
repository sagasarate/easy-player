/****************************************************************************/
/*                                                                          */
/*      文件名:    VideoRect.h                                              */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
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
