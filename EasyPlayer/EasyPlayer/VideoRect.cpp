/****************************************************************************/
/*                                                                          */
/*      文件名:    VideoRect.cpp                                            */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\videorect.h"


LPCTSTR DEFAULT_VIDEO_FX=
"texture TexLay0 < string name = \"test.jpg\"; >;"
"texture TexLay1 < string name = \"test1.jpg\"; >;"
"technique tec0"
"{"
"    pass p0"
"    {"
"		MultiSampleAntialias = FALSE;"
"		Lighting=false;"
"		zenable = false;"
"		zwriteenable = false;"
"		CullMode = none;"
"		fogenable = false;"
"		Texture[0] = <TexLay0>;"
"		AlphaTestEnable = false;"
"		AlphaBlendEnable = false;"
"		SrcBlend = SrcAlpha;"
"		DestBlend = InvSrcAlpha;"
"		Texture[0] = <TexLay0>;"
"     	ColorOp[0] = SelectArg1;"
"       ColorArg1[0] = Texture;"
"       ColorArg2[0] = Diffuse;"      	
"       AlphaOp[0] = SelectArg1;"
"       AlphaArg1[0] = diffuse;"
"		ColorOp[1] = disable;"
"		AlphaOp[1] = disable;"
"		AddressU[0] = clamp;"
"		AddressV[0] = clamp;"      
"		MinFilter[0] = linear;"
"       MagFilter[0] = linear;"
"       MipFilter[0] = linear;"
"    }"
"}";

CVideoRect::CVideoRect(void):
	m_Rect(0,0,100,100)
{
	m_pVideoTexture=NULL;
	
	m_SubMesh.GetVertexFormat().FVF=D3DFVF_RECTVERTEX;
	m_SubMesh.GetVertexFormat().VertexSize=sizeof(RECTVERTEX);
	m_SubMesh.GetVertexFormat().IndexSize=sizeof(WORD);
	m_SubMesh.SetVertices((BYTE *)m_Vertexs);
	m_SubMesh.SetVertexCount(4);
	m_SubMesh.SetPrimitiveType(D3DPT_TRIANGLESTRIP);
	m_SubMesh.SetPrimitiveCount(2);
	m_SubMesh.SetRenderBufferUsed(CD3DSubMesh::BUFFER_USE_CUSTOM);
}

CVideoRect::~CVideoRect(void)
{
	Destory();
}

void CVideoRect::Destory()
{
	SAFE_RELEASE(m_pVideoTexture);	
	SetVisible(false);
}

BOOL CVideoRect::Create(LPCTSTR FileName,bool ForceLoadDirectVobSub)
{
	if(m_pRender==NULL)
		return FALSE;
	Destory();

	m_SubMesh.GetMaterial().ClearAllTexture();

	m_pVideoTexture=new CD3DVideoTexture();
	m_pVideoTexture->SetManager(m_pRender->GetDevice()->GetTextureManager());
	m_pVideoTexture->EnableForceLoadVobSub(ForceLoadDirectVobSub);

	m_SubMesh.GetMaterial().AddTexture(m_pVideoTexture,0,"","");
	m_pVideoTexture->AddUseRef();

	CD3DFX * pFX=m_pRender->GetDevice()->GetFXManager()->
		LoadFXFromMemory("DefaultVideoFX",DEFAULT_VIDEO_FX,(int)strlen(DEFAULT_VIDEO_FX));
	if(pFX)
	{
		m_SubMesh.GetMaterial().SetFX(pFX);		
	}
	
	
	

	if(!m_pVideoTexture->Create(FileName))
	{
		m_pVideoTexture->Destory();
		return FALSE;
	}
		
	m_pRender->GetDevice()->GetTextureManager()->AddTexture(m_pVideoTexture,m_pVideoTexture->GetName());

	//int Width,Height;
	//m_pVideoTexture->GetVideoSize(Width,Height);
	//m_Rect.left=0;
	//m_Rect.top=0;
	//m_Rect.right=Width;
	//m_Rect.bottom=Height;

	CreateVertex();
	SetVisible(true);
	return TRUE;
}

void CVideoRect::OnPrepareRender(CD3DBaseRender * pRender,CD3DFX * pFX,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera)
{	
}

void CVideoRect::OnPrepareRenderSubMesh(CD3DBaseRender * pRender,CD3DFX * pFX,CD3DSubMesh * pSubMesh,CD3DSubMeshMaterial * pMaterial,CEasyArray<CD3DLight *>& LightList,CD3DCamera * pCamera)
{
	//设置材质

	//如果是被选中的使用特殊材质
	if(pSubMesh->IsSelected())
		pRender->GetDevice()->GetD3DDevice()->SetMaterial(&SELECTED_SUBMESH_MATERIAL);
	else
		pRender->GetDevice()->GetD3DDevice()->SetMaterial(&(pMaterial->GetMaterial()));

	//设置纹理
	if(pMaterial->GetFX())
	{			
		pMaterial->GetFX()->SetTexture("TexLay0",pMaterial->GetTexture(0));
	}	
}

void CVideoRect::CreateVertex()
{
	FLOAT x1,y1,tx1,ty1;
	FLOAT x2,y2,tx2,ty2;
	

	x1=(FLOAT)m_Rect.left;
	y1=(FLOAT)m_Rect.top;	
	x2=(FLOAT)m_Rect.right;
	y2=(FLOAT)m_Rect.bottom;
	if(m_pVideoTexture)
	{
		tx1=0;
		ty1=0;
		tx2=m_pVideoTexture->GetUS();
		ty2=m_pVideoTexture->GetVS();
	}
	else
	{
		tx1=0;tx2=0;
		ty1=0;ty2=0;
	}
	m_Vertexs[0] = InitRECTVertex( D3DXVECTOR4(x1,y1,0.9f,1.0f), 0xffffffff, tx1, ty1 );
	m_Vertexs[1] = InitRECTVertex( D3DXVECTOR4(x2,y1,0.9f,1.0f), 0xffffffff, tx2, ty1 );
	m_Vertexs[2] = InitRECTVertex( D3DXVECTOR4(x1,y2,0.9f,1.0f), 0xffffffff, tx1, ty2 );
	m_Vertexs[3] = InitRECTVertex( D3DXVECTOR4(x2,y2,0.9f,1.0f), 0xffffffff, tx2, ty2 );
}