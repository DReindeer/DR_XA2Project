// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : �V�[���Ǘ�
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "utility.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// �ÓI�����o�ϐ�
// -------- -------- -------- -------- -------- -------- -------- --------
std::list<CScene*> CScene::m_lisScene[MAX_SCENE_PRIORITY];
int CScene::m_numScene[MAX_SCENE_PRIORITY] = { 0 };			// �V�[���̑���

// -------- -------- -------- -------- -------- -------- -------- --------
// �v���C�I���e�B�w��R���X�g���N�^
// -------- -------- -------- -------- -------- -------- -------- --------
CScene::CScene(int priority = 4) : m_r(255), m_g(255), m_b(255), m_a(255),
									m_priority(priority)
{
	//---- �R���e�i�ɂ߂�
	m_lisScene[m_priority].push_back(this);

	//-------- ������
	m_objType = OBJTYPE::OBJTYPE_NONE;
	m_pos = XMFLOAT3(0.f, 0.f, 0.f);		// ���W
	m_size = XMFLOAT3(0.f, 0.f, 0.f);		// �T�C�Y
	m_rot = XMFLOAT3(0.f, 0.f, 0.f);		// ��]�p
	m_scl = XMFLOAT3(1.f, 1.f, 1.f);		// �g�嗦
	m_delete = false;						// ����t���O
	m_numScene[m_priority]++;				// �����̃C���N�������g
}
// -------- -------- -------- -------- -------- -------- -------- --------
// �f�X�g���N�^
// -------- -------- -------- -------- -------- -------- -------- --------
CScene::~CScene()
{
	m_numScene[m_priority]--;		// �����̃f�N�������g
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CScene::�S�X�V
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene::UpdateAll(void)
{
	//-------- �S�X�V
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// �X�V
		for (CScene* pScene : m_lisScene[cntPriority])
		{
			pScene->Update();
		}

		// �폜�t���O�̌����Ă���Scene�̍폜
		m_lisScene[cntPriority].remove_if([](CScene* pScene)
		{
			bool bErase = false;
			if (pScene->m_delete)
			{
				//---- ���
				pScene->Uninit();
				delete pScene;
				pScene = nullptr; 
				bErase = true;
			}
			return bErase;
		});
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CScene::�v���C�I���e�B�w��X�V
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene::UpdateAll(int priority)
{
	// �X�V
	for (CScene* pScene : m_lisScene[priority])
	{
		pScene->Update();
	}

	// �폜�t���O�̌����Ă���Scene�̍폜
	m_lisScene[priority].remove_if([](CScene* pScene)
	{
		bool bErase = false;
		if (pScene->m_delete)
		{
			//---- ���
			pScene->Uninit();
			delete pScene;
			pScene = nullptr;
			bErase = true;
		}
		return bErase;
	});
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CScene::�S�`��
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene::DrawAll(void)
{
	// �f�o�C�X�̎擾
	ID3D11DeviceContext* pDevice = CRenderer::GetDeviceContext();
	XMFLOAT3 camPos = CManager::GetCamera()->GetPos();

	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// �J��������̋����ɂ���ă\�[�g
		m_lisScene[cntPriority].sort([camPos](CScene* pSceneA, CScene* pSceneB)
		{
			float vecA = XM::F3::Distance(pSceneA->GetPos(), camPos);
			float vecB = XM::F3::Distance(pSceneB->GetPos(), camPos);
			return vecA > vecB;
		});

		for (CScene* pScene : m_lisScene[cntPriority])
		{
			// �`�揈��
			pScene->Draw();
		}
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CScene::�v���C�I���e�B�w��`��
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene::DrawAll(int priority)
{
	// �f�o�C�X�̎擾
	ID3D11DeviceContext* pDevice = CRenderer::GetDeviceContext();
	XMFLOAT3 camPos = CManager::GetCamera()->GetPos();

	// �J��������̋����ɂ���ă\�[�g
	m_lisScene[priority].sort([camPos](CScene* pSceneA, CScene* pSceneB)
	{
		float vecA = XM::F3::Distance(pSceneA->GetPos(), camPos);
		float vecB = XM::F3::Distance(pSceneB->GetPos(), camPos);
		return vecA > vecB;
	});

	for (CScene* pScene : m_lisScene[priority])
	{
		pScene->Draw();
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CScene::�S�����[�X
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene::ReleaseAll(void)
{
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// Scene�̍폜
		for(CScene* it : m_lisScene[cntPriority])
		{
			//---- ���
			it->Uninit();
			it = nullptr;
		};
		m_lisScene[cntPriority].clear();
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// CScene::�w��^�C�v�S�����[�X
// -------- -------- -------- -------- -------- -------- -------- --------
void CScene::TypeRelease(const OBJTYPE &selectObjType)
{
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// �w��^�C�v��Scene�̍폜
		for(CScene* pScene : m_lisScene[cntPriority])
		{
			if (pScene->GetObjType() == selectObjType)
			{
				pScene->Release();
			}
		}
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �w��^�C�v�擾
// -------- -------- -------- -------- -------- -------- -------- --------
CScene *CScene::GetSceneClassType(const CLASSTYPE &selectObjType)
{
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// �w��^�C�v��Scene�̎擾
		for (CScene* pScene : m_lisScene[cntPriority])
		{
			if (pScene->GetClassType() == selectObjType)
			{
				return pScene;
			}
		}
	}
	return nullptr;
}