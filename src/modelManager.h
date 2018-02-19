//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include "main.h"
#include <unordered_map>

typedef struct
{
	LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration;	// ���_�f�N�����[�V����
	LPDIRECT3DVERTEXSHADER9 pVertexShader;				// ���_�V�F�[�_�[�i�[�ꏊ
	LPD3DXCONSTANTTABLE pVConstantTable;				// ���_�V�F�[�_�[�p�萔�e�[�u��
	LPDIRECT3DPIXELSHADER9 pPixelShader;				// �s�N�Z���V�F�[�_�[�i�[�ꏊ
	LPD3DXCONSTANTTABLE pPConstantTable;				// �s�N�Z���V�F�[�_�[�p�萔�e�[�u��
	LPDIRECT3DPIXELSHADER9 pPixelShaderNoTex;			// �e�N�X�`�������s�N�Z���V�F�[�_�[�i�[�ꏊ
	LPD3DXCONSTANTTABLE pPConstantTableNoTex;			// �e�N�X�`�������s�N�Z���V�F�[�_�[�p�萔�e�[�u��
}MODEL_SHADER_SET;

//================================================================================
// �N���X
//--------------------------------------------------------------------------------
class CModelManager
{
public:
	typedef struct
	{
		LPD3DXMESH pMesh;					// ���b�V���C���^�[�t�F�C�X�|�C���^
		LPD3DXMESH pMeshClone;				// �N���[�����b�V���C���^�[�t�F�C�X�|�C���^
		LPD3DXBUFFER pBuffMat;				// �}�e���A���o�b�t�@
		DWORD numMat;						// �}�e���A����
		D3DXMATERIAL* pMat;					// �}�e���A�����
	}MODEL;

	CModelManager(){}
	~CModelManager(){}

	void Init();
	void Uninit();
	
	bool SetModel(const std::string &strFilePass);
	MODEL *GetModel(const std::string &strFilePass);

	LPD3DXMESH GetModelMesh(const std::string &strFilePass);
	LPD3DXBUFFER GetModelBuffMat(const std::string &strFilePass);
	DWORD GetModelNumMat(const std::string &strFilePass);
	D3DXMATERIAL *GetModelMat(const std::string &strFilePass);
	
	bool Release(const std::string &strFilePass);			// ���
	void ReleaseAll();										// �S�J��

private:

	std::unordered_map<std::string, MODEL*> m_umapModel;	// �t�@�C���p�X
};

#endif