//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "manager.h"
#include "texManager.h"
#include "modelManager.h"
#include "scene3D.h"
#include "sceneCircleShadow.h"
#include "input.h"
#include "renderer.h"

#include <assert.h>

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void CSceneCircleShadow::Init()
{
	CSceneModel::Init();
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void CSceneCircleShadow::Uninit()
{
	CSceneModel::Uninit();
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void CSceneCircleShadow::Update()
{
	CSceneModel::Update();
}

//--------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------
void CSceneCircleShadow::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXMatrixIdentity(&m_mtxWorld);// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )
	D3DXMatrixIdentity(&m_mtxScl);	// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )
	D3DXMatrixIdentity(&m_mtxRot);	// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )
	D3DXMatrixIdentity(&m_mtxPos);	// �s���P�ʍs��ɂ���( �g�嗦1,��0�ŏ����� )

	D3DXVECTOR3 scl = GetScl();

	// ���[���h�ϊ��s���ݒ肵�ĕϊ�
	D3DXMatrixScaling(&m_mtxScl, scl.x, scl.y, scl.z);	// �g��s��
	m_mtxRot = GetMatrixRot(this);
	m_mtxPos = GetMatrixPos(this);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxPos);

	// �f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���݃f�o�C�X�ɐݒ肳��Ă���}�e���A�����̎擾
	D3DXMATERIAL* pMat = CManager::GetModelManager()->GetModelMat(m_strFilePass);
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// �\��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

	// ���f���̃}�e���A�����Ƃɐݒ�
	for (int nCntMat = 0; nCntMat < (int)CManager::GetModelManager()->GetModelNumMat(m_strFilePass); nCntMat++)
	{
		// �f�o�C�X�Ƀ}�e���A����ݒ�
		pDevice->SetMaterial(&pMat->MatD3D);

		// �e�N�X�`�����̐ݒ�
		if (pMat[nCntMat].pTextureFilename != nullptr){
			pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(pMat[nCntMat].pTextureFilename));
		}
		else{// �e�N�X�`�����Ȃ����null
			pDevice->SetTexture(0, nullptr);
		}

		// �`��
		CManager::GetModelManager()->GetModelMesh(m_strFilePass)->DrawSubset(nCntMat);

	//	pDevice->SetMaterial(&matDef);	// ���ɖ߂�
	}

	// ����
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

//	pDevice->GetMaterial(&matDef);

	// ���f���̃}�e���A�����Ƃɐݒ�
	for (int nCntMat = 0; nCntMat < (int)CManager::GetModelManager()->GetModelNumMat(m_strFilePass); nCntMat++)
	{
		// �f�o�C�X�Ƀ}�e���A����ݒ�
		pDevice->SetMaterial(&pMat->MatD3D);

		// �e�N�X�`�����̐ݒ�
		if (pMat[nCntMat].pTextureFilename != nullptr) {
			pDevice->SetTexture(0, CManager::GetTexManager()->GetTexInterface(pMat[nCntMat].pTextureFilename));
		}
		else {// �e�N�X�`�����Ȃ����null
			pDevice->SetTexture(0, nullptr);
		}

		// �`��
		CManager::GetModelManager()->GetModelMesh(m_strFilePass)->DrawSubset(nCntMat);
	}

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	pDevice->SetMaterial(&matDef);	// ���ɖ߂�
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CSceneCircleShadow *CSceneCircleShadow::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, int pry)
{
	// CScene3D�̐���
	CSceneCircleShadow *pCircleShadow;
	pCircleShadow = new CSceneCircleShadow(pry);
	pCircleShadow->Init();

	pCircleShadow->SetPos(pos);
	pCircleShadow->SetRot(rot);
	pCircleShadow->SetBasePos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCircleShadow->SetBaseRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCircleShadow->SetScl(scl);
	pCircleShadow->m_strFilePass = strFilePass;
	pCircleShadow->m_strPartsName = strFilePass;

	// ���f���̐���
	pCircleShadow->CreateModel(pCircleShadow->m_strFilePass);

	return pCircleShadow;
}
