//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#include "gravityObj.h"
#include "input.h"
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "debugStr.h"
#include "modelManager.h"
#include "sceneModel.h"
#include "texManager.h"
#include "sceneField.h"

//================================================================================
// CGravityObj::����������
//================================================================================
void CGravityObj::Init()
{
	// �e�N���X�̏�����
	CSceneModel::Init();

	m_move = D3DXVECTOR3(0.f, 0.f, 0.f);		// �ړ���
	m_posOldY = 0.f;
	m_jump = true;
}

//================================================================================
// CGravityObj::�I������
//================================================================================
void CGravityObj::Uninit()
{
	// �e�N���X�̏I��
	CSceneModel::Uninit();
}

//================================================================================
// CGravityObj::�X�V����
//================================================================================
void CGravityObj::Update()
{
	// �e�N���X�̍X�V
	CSceneModel::Update();

	// �n�ʂƂ̍������m�肵���ꍇ�͗�����������Ȃ�
	float y = GetPos().y; 
	if (m_posOldY != y)
	{
		m_jump = true;
	}

	if (m_jump == true)
	{
		m_move.y -= 0.1f;		// �����I�I�I
		AddPos(m_move);			// �ړ��ʂ̉��Z

		// �Փ˔���
		for (int n = 0; n < MAX_SCENE_PRIORITY; n++)
		{
			std::list<CScene*> *lisScene = CScene::GetScene(n);
			for (CScene* pScene : *lisScene)
			{
				if (pScene == nullptr)continue;
				// �^�C�v�̎擾
				CScene::OBJTYPE objType = pScene->GetObjType();

				// �t�B�[���h�̏ꍇ
				if (objType == CScene::OBJTYPE::FIELD)
				{
					// �_�E���L���X�g
					CSceneField *pField = ((CSceneField*)pScene);
					D3DXVECTOR3 gObjPos = GetPos();
					bool hit;
					float fieldY = pField->GetHeight(gObjPos, &hit);
					if (!hit)
					{
						//					this->Release();
						//					return;
					}
					if (gObjPos.y < fieldY)
					{
						SetPos(D3DXVECTOR3(gObjPos.x, fieldY, gObjPos.z));
						m_move.y = 0.f;
						m_jump = false;
						m_posOldY = fieldY;
						break;
					}
				}
			}
		}
	}
}

//================================================================================
// CGravityObj::�`�揈��
//================================================================================
void CGravityObj::Draw()
{
	// �e�N���X�̕`��
	CSceneModel::Draw();
}

//--------------------------------------------------------------------------------
// �쐬����
//--------------------------------------------------------------------------------
CGravityObj *CGravityObj::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rotY, const D3DXVECTOR3 &scl, const std::string &strFilePass)
{
	// CScene3D�̐���
	CGravityObj *pObj = new CGravityObj();
	pObj->Init();

	pObj->SetPos(pos);
	pObj->SetRot(rotY);
	pObj->SetScl(scl);
	pObj->SetFilePass(strFilePass);

	//---------------------
	// ���f���̃Z�b�g
	//---------------------
	CManager::GetModelManager()->SetModel(strFilePass);

	//---------------------
	// �e�N�X�`���̃Z�b�g
	//---------------------
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

	return pObj;
}