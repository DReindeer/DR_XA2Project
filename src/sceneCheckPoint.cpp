//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/01/15
//
//--------------------------------------------------------------------------------
#include "renderer.h"
#include "manager.h"
#include "modeGame.h"
#include "modeTitle.h"
#include "modelManager.h"
#include "texManager.h"
#include "sceneCheckPoint.h"
#include "light.h"
#include "debugStr.h"

//--------------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------------
void SceneCheckPoint::Init()
{
	CSceneModel::Init();
}

//--------------------------------------------------------------------------------
// �I��
//--------------------------------------------------------------------------------
void SceneCheckPoint::Uninit()
{
	CSceneModel::Uninit();
}

//--------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------
void SceneCheckPoint::Update()
{
	CSceneModel::Update();
}

//--------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------
void SceneCheckPoint::Draw()
{

}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
SceneCheckPoint *SceneCheckPoint::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &scl, const std::string &strFilePass, int pry)
{
	// ����
	SceneCheckPoint *pCheckPoint;
	pCheckPoint = new SceneCheckPoint(pry);
	pCheckPoint->Init();

	pCheckPoint->SetPos(pos);
	pCheckPoint->SetRot(rot);
	pCheckPoint->SetBasePos(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCheckPoint->SetBaseRot(D3DXVECTOR3(0.f, 0.f, 0.f));
	pCheckPoint->SetScl(scl);
	pCheckPoint->m_strFilePass = strFilePass;
	pCheckPoint->m_strPartsName = strFilePass;

	// ���f���̐���
	pCheckPoint->CreateModel(pCheckPoint->m_strFilePass);

	D3DXVECTOR3 vec01 = pCheckPoint->m_point[1] - pCheckPoint->m_point[0];
	pCheckPoint->m_startRot = atan2(vec01.z, vec01.x) * D3DX_PI * 0.5f;

	return pCheckPoint;
}

//================================================================================
// ���f������
//--------------------------------------------------------------------------------
void SceneCheckPoint::CreateModel(std::string strFilePass)
{
	if (m_pUseModelManager == nullptr)
	{
		m_pUseModelManager = CManager::GetModelManager();
	}

	// ���f���}�l�[�W���ɐ���
	if (m_pUseModelManager->SetModel(strFilePass))
	{
		// �e�N�X�`���̃Z�b�g
		D3DXMATERIAL* pMat = CManager::GetModelManager()->GetModelMat(strFilePass);
		for (int nCntMat = 0; nCntMat < (int)CManager::GetModelManager()->GetModelNumMat(strFilePass); nCntMat++)
		{
			// �g�p���Ă���e�N�X�`��������Γǂݍ���
			if (pMat[nCntMat].pTextureFilename != nullptr && lstrlen(pMat[nCntMat].pTextureFilename) > 0)
			{
				CManager::GetTexManager()->SetTex(pMat[nCntMat].pTextureFilename);
			}
			else { pMat[nCntMat].pTextureFilename = nullptr; }
		}
		//---------------------------------------------
		// ���f���̒��_�̍ő�l�A�ŏ��l�̎擾
		//---------------------------------------------
		LPD3DXMESH pMesh = nullptr;
		pMesh = m_pUseModelManager->GetModelMesh(strFilePass);
		D3DXVECTOR3 Vertex;
		D3DXVECTOR3 min = D3DXVECTOR3(0.f, 0.f, 0.f);
		D3DXVECTOR3 max = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_centerPos = D3DXVECTOR3(0.f, 0.f, 0.f);

		//�o�b�t�@�����b�N����
		BYTE *pVtx = nullptr;							// ���_�o�b�t�@
		pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);
		DWORD FVF = pMesh->GetFVF();					// ���_�t�H�[�}�b�g���擾����B
		DWORD VertexSize = D3DXGetFVFVertexSize(FVF);	// ���_�̃T�C�Y���擾����B

		// ���_�̎擾
		for (int nCntMesh = 0; nCntMesh < 2; nCntMesh++)
		{
			Vertex = *(D3DXVECTOR3*)pVtx * CManager::GetMasterScale();				// ���_���W���擾����B
			m_point[nCntMesh] = Vertex;					// ���_����ꍞ��
			m_centerPos += Vertex;						// �S���_�̍��W�𑫂�����
			min.x = RETURN_MIN(min.x, Vertex.x);		// X�ŏ��l
			max.x = RETURN_MAX(max.x, Vertex.x);		// X�ő�l
			min.y = RETURN_MIN(min.y, Vertex.y);		// Y�ŏ��l
			max.y = RETURN_MAX(max.y, Vertex.y);		// Y�ő�l
			min.z = RETURN_MIN(min.z, Vertex.z);		// Z�ŏ��l
			max.z = RETURN_MAX(max.z, Vertex.z);		// Z�ő�l
			pVtx += VertexSize;							// �i�߂�
		}
		// �o�b�t�@�̃A�����b�N
		pMesh->UnlockVertexBuffer();

		// ��,����,���s�̎擾
		SetSize(D3DXVECTOR3(max.x - min.x, max.y - min.y, max.z - min.z));

		// 2���_�̒��S���W
		m_centerPos /= 2;
	}
}

