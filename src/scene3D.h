//================================================================================
//
//	title : �|���S���\���v���O����[ CScene3D.h ]	Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
//--------------------------------------------------------------------------------
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include <string>
#include "scene.h"

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class CScene3D : public CScene
{
public:
	typedef struct
	{
		LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration;	// ���_�f�N�����[�V����
		LPDIRECT3DVERTEXSHADER9 pVertexShader;				// ���_�V�F�[�_�[�i�[�ꏊ
		LPD3DXCONSTANTTABLE pVConstantTable;				// ���_�V�F�[�_�[�p�萔�e�[�u��
		LPDIRECT3DPIXELSHADER9 pPixelShader;				// �s�N�Z���V�F�[�_�[�i�[�ꏊ
		LPD3DXCONSTANTTABLE pPConstantTable;				// �s�N�Z���V�F�[�_�[�p�萔�e�[�u��
		LPDIRECT3DPIXELSHADER9 pPixelShaderNoTex;			// �e�N�X�`�������s�N�Z���V�F�[�_�[�i�[�ꏊ
		LPD3DXCONSTANTTABLE pPConstantTableNoTex;			// �e�N�X�`�������s�N�Z���V�F�[�_�[�p�萔�e�[�u��
	}SHADER_SET;

	CScene3D(int priority = 4);
	~CScene3D();

	void Init() {}
	void Uninit() {}
	void Update() {}
	void Draw() {}

	const D3DXMATRIX &GetMtxWorld()const { return m_mtxWorld; }
	const float &GetR()const { return m_r; }
	const float &GetG()const { return m_g; }
	const float &GetB()const { return m_b; }
	const float &GetA()const { return m_a; }
	void SetCol(const float &r, const float &g, const float &b, const float &a) { m_r = r; m_g = g; m_b = b; m_a = a; }
	void AddCol(const float &r, const float &g, const float &b, const float &a) { m_r += r; m_g += g; m_b += b; m_a += a; }

	void SetNumVtx(const int &numVtx) { m_numVtx = numVtx; }
	void SetNumIndex(const int &numIndex) { m_numIndex = numIndex; }
	const int &GetNumVtx() { return m_numVtx; }
	const int &GetNumIndex() { return m_numIndex; }

	std::string GetFilePath()const { return m_strFilePass; }
	void SetFilePass(std::string strFilePass) { m_strFilePass = strFilePass; }

protected:
	std::string m_strFilePass;		// �e�N�X�`��FilePass
	D3DXMATRIX m_mtxWorld;			// ���[���h�}�g���N�X
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffer;
	D3DMATERIAL9 *m_pMat;
	static SHADER_SET *m_pShaderSet;	// �V�F�[�_�[

private:
	void CreateShader();

	int m_numVtx;
	int m_numIndex;
	float m_r, m_g, m_b, m_a;
};

#endif
