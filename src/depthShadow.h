//================================================================================
//
//	title : �|���S���\���v���O����[ CScene2D.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _SCENE2DEPTHSHADOW_H_
#define _SCENE2DEPTHSHADOW_H_

#include <string>
#include "scene2D.h"

//--------------------------------------------------------------------------------
// �O���錾
//--------------------------------------------------------------------------------
class CTexManager;

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class DepthShadow
{
public:
	typedef struct
	{
		LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration;	// ���_�f�N�����[�V����
		LPDIRECT3DVERTEXSHADER9 pVertexShader;				// ���_�V�F�[�_�[�i�[�ꏊ
		LPD3DXCONSTANTTABLE pVConstantTable;				// ���_�V�F�[�_�[�p�萔�e�[�u��
		LPDIRECT3DPIXELSHADER9 pPixelShader;				// �s�N�Z���V�F�[�_�[�i�[�ꏊ
		LPD3DXCONSTANTTABLE pPConstantTable;				// �s�N�Z���V�F�[�_�[�p�萔�e�[�u��
	}DEPTHSHADOW_SHADER_SET;

	DepthShadow() {}
	~DepthShadow();

	void Init();				// ������
	void Uninit();				// �I��
	void Update();				// �X�V
	void DrawBigin(D3DXMATRIX mtxWorld);	// �`��
	void DrawEnd();				// �`��
	void DrawDeviceBegin();
	void DrawDeviceEnd();
	void ClearBackBuffer();
	static DepthShadow *Create();

	const LPDIRECT3DTEXTURE9 &GetTexture() { return m_depthShadowTexture; }

protected:
	D3DXVECTOR2 m_posUV;		// �e�N�X�`�����W
	float m_widthUV;			// ��
	float m_heightUV;			// ����
	std::string m_strFilePass;	// �e�N�X�`��FilePass

private:
	void CreateShader();

	void SwapDrawFlag() {
		m_drawFlag ^= 1;
		m_pDepthShadowPolygon->SetDrawFlag((bool)m_drawFlag);
	}

	CTexManager *m_pTexManager;
	DEPTHSHADOW_SHADER_SET *m_pDepthShadow;
	LPDIRECT3DTEXTURE9 m_depthShadowTexture;	// �e�N�X�`��
	LPDIRECT3DSURFACE9 m_depthShadowSurface;	// �T�[�t�F�X
	LPDIRECT3DSURFACE9 m_renderTargetZBuffer;	// �����_�[�^�[�Q�b�g�ɂ����e�N�X�`����Z�o�b�t�@�i�[�p�T�[�t�F�X
	D3DVIEWPORT9 m_viewOld;

	// �������|���S��
	int m_drawFlag = 1;
	CScene2D *m_pDepthShadowPolygon = nullptr;
};

#endif
