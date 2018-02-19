//================================================================================
//
//	title : �|���S���\���v���O����[ CScene2D.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _DEFERRD_RENDERING_H_
#define _DEFERRD_RENDERING_H_

#include <string>
#include "scene2D.h"

static const int MAX_RENDERTARGET = 4;
static const int MAX_BLOOM = 2;

//--------------------------------------------------------------------------------
// �O���錾
//--------------------------------------------------------------------------------
class CTexManager;
class CScene2DDeferred;
class CScene2DPx;

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class DefferdRendering
{
public:
	typedef struct
	{
		LPDIRECT3DVERTEXSHADER9 pVertexShader;				// ���_�V�F�[�_�[�i�[�ꏊ
		LPD3DXCONSTANTTABLE pVConstantTable;				// ���_�V�F�[�_�[�p�萔�e�[�u��
		LPDIRECT3DPIXELSHADER9 pPixelShader;				// �s�N�Z���V�F�[�_�[�i�[�ꏊ
		LPD3DXCONSTANTTABLE pPConstantTable;				// �s�N�Z���V�F�[�_�[�p�萔�e�[�u��
		LPDIRECT3DPIXELSHADER9 pPixelShaderNoTex;			// �e�N�X�`�������s�N�Z���V�F�[�_�[�i�[�ꏊ
		LPD3DXCONSTANTTABLE pPConstantTableNoTex;			// �e�N�X�`�������s�N�Z���V�F�[�_�[�p�萔�e�[�u��
	}SHADER_SET;

	DefferdRendering() : m_pCombinePolygon(nullptr){}
	~DefferdRendering();

	void Init();											// ������
	void Uninit();											// �I��
	void Update();											// �X�V

	// ���f���p
	void SetVXShader(D3DXMATRIX mtxWorld);					// VX�V�F�[�_�[�̐ݒ�
	void SetPXShader(D3DMATERIAL9* pMat);					// PX�V�F�[�_�[�̐ݒ�
	void SetNoTexturePXShader(D3DMATERIAL9* pMat);			// �e�N�X�`������PX�V�F�[�_�[�̐ݒ�

	void DrawDeviceBegin();									// �����_�[�^�[�Q�b�g�̐؂�ւ�
	void DrawDeviceEnd();									// �����_�[�^�[�Q�b�g��߂�
	void ClearBackBuffer();									// Z�o�b�t�@�N���A

	void DrawCombineTexture();
	void DrawCombineEnd();

	void DrawCombineBloom();
	void DrawCombineBloomEnd();

	static DefferdRendering *Create();						// ����

	const LPDIRECT3DTEXTURE9 &GetTexture(const int &i) { return m_renderTexture[i]; }
	const LPDIRECT3DSURFACE9 &GetSurface(const int &i) { return m_renderSurface[i]; }

protected:
	D3DXVECTOR2 m_posUV;									// �e�N�X�`�����W
	float m_widthUV;										// ��
	float m_heightUV;										// ����
	std::string m_strFilePass[MAX_RENDERTARGET];			// �e�N�X�`��FilePass

private:
	void CreateShader();

	void SwapDrawFlag() { 
		m_drawFlag ^= 1;
		m_pGPolygon->SetDrawFlag((bool)m_drawFlag);
		m_pBloom->SetDrawFlag((bool)m_drawFlag);
		for (int i = 0; i < MAX_RENDERTARGET; ++i) { m_pRenderPolygon[i]->SetDrawFlag((bool)m_drawFlag); }
	}

	LPDIRECT3DSURFACE9 m_renderTargetZBuffer;				// �e�N�X�`����Z�o�b�t�@�i�[�p�T�[�t�F�X
	LPDIRECT3DSURFACE9 m_renderTargetZBuffer4;				// �e�N�X�`����Z�o�b�t�@�i�[�p�T�[�t�F�X1/4
	LPDIRECT3DSURFACE9 m_renderTargetZBuffer16;				// �e�N�X�`����Z�o�b�t�@�i�[�p�T�[�t�F�X1/16

	CTexManager *m_pTexManager;
	SHADER_SET *m_pShaderSet;
	LPDIRECT3DTEXTURE9 m_renderTexture[MAX_RENDERTARGET];	// �e�N�X�`��
	LPDIRECT3DSURFACE9 m_renderSurface[MAX_RENDERTARGET];	// �T�[�t�F�X

	CScene2DDeferred *m_pCombinePolygon;					// ������|���S��
	LPDIRECT3DTEXTURE9 m_combineTexture;					// �e�N�X�`��
	LPDIRECT3DSURFACE9 m_combineSurface;					// �T�[�t�F�X
	LPD3DXCONSTANTTABLE m_pPConstantTableCombine;			// �s�N�Z���V�F�[�_�[�p�萔�e�[�u��
	LPDIRECT3DPIXELSHADER9 m_pPixelShaderCombine;			// �s�N�Z���V�F�[�_�[�i�[�ꏊ

	void UpdateBloom();										// �u���[���̍X�V
	CScene2DPx *m_pBloomPolygon[4];							// �u���[��������|���S��
	CScene2DPx *m_pCombineBuloom;							// ������|���S��
	LPDIRECT3DTEXTURE9 m_bloomTexture[4];					// �e�N�X�`��
	LPDIRECT3DSURFACE9 m_bloomSurface[4];					// �T�[�t�F�X

	LPD3DXCONSTANTTABLE m_pPCColorBrightPass;				// �P�x�s�N�Z���V�F�[�_�[�p�萔�e�[�u��
	LPDIRECT3DPIXELSHADER9 m_pPColorBrightPass;				// �P�x�s�N�Z���V�F�[�_�[�i�[�ꏊ

	LPD3DXCONSTANTTABLE m_pPCBloomH;						// �u���[��H�s�N�Z���V�F�[�_�[�p�萔�e�[�u��
	LPDIRECT3DPIXELSHADER9 m_pPBloomH;						// �u���[���s�N�Z���V�F�[�_�[�i�[�ꏊ
	LPD3DXCONSTANTTABLE m_pPCBloomV;						// �u���[��V�s�N�Z���V�F�[�_�[�p�萔�e�[�u��
	LPDIRECT3DPIXELSHADER9 m_pPBloomV;						// �u���[���s�N�Z���V�F�[�_�[�i�[�ꏊ

	LPD3DXCONSTANTTABLE m_pPCBloomCombine;					// �u���[�������s�N�Z���V�F�[�_�[�p�萔�e�[�u��
	LPDIRECT3DPIXELSHADER9 m_pPBloomCombine;				// �u���[�������s�N�Z���V�F�[�_�[�i�[�ꏊ

	// �������|���S��
	int m_drawFlag = 1;
	CScene2D *m_pRenderPolygon[MAX_RENDERTARGET] = { nullptr };
	CScene2D *m_pGPolygon = nullptr;			// G�o�b�t�@�`��p
	CScene2D *m_pBloom = nullptr;				// �u���[���`��p
	CScene2D *m_combineBloomPolygon = nullptr;	// ������(�ŏI)�`��p
};

#endif
