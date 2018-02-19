//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/05/11
//
//--------------------------------------------------------------------------------
#include "mode.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"

//--------------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
std::list<CScene*> CScene::m_lisScene[MAX_SCENE_PRIORITY];
CScene::CLASSTYPE CScene::m_drawTypeOnly = CLASSTYPE_NONE;	// �B��`�悵�������̂�����`��

//--------------------------------------------------------------------------------
// �v���C�I���e�B�w��R���X�g���N�^
//--------------------------------------------------------------------------------
CScene::CScene(int priority = 4) : m_pVtxBuffer(nullptr), m_r(255), m_g(255), m_b(255), m_a(255),
									m_priority(priority), m_lightOff(false), m_alpha(false), m_drawFlag(true), m_overall(false)
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	// �쐬���̃��[�h
	CMode* pMode = CManager::GetMode();
	CMode* pLoadingMode = CManager::GetLoadingMode();
	if (pMode == nullptr && pLoadingMode == nullptr) { m_createModeType = CMode::MODETYPE::NONE; }
	else if (pMode == pLoadingMode || pLoadingMode == nullptr) { m_createModeType = pMode->GetModeType(); }
	else if (pMode != pLoadingMode) { m_createModeType = pLoadingMode->GetModeType(); }

	m_objType = OBJTYPE::OBJTYPE_NONE;		// �^�C�v
	m_pos = D3DXVECTOR3(0.f, 0.f, 0.f);		// ���W
	m_size = D3DXVECTOR3(0.f, 0.f, 0.f);	// �T�C�Y
	m_rot = D3DXVECTOR3(0.f, 0.f, 0.f);		// ��]�p
	m_scl = D3DXVECTOR3(1.f, 1.f, 1.f);		// �g�嗦
	m_delete = false;						// ����t���O

	//---- �R���e�i�ɂ߂�
	m_lisScene[m_priority].push_back(this);
}

//--------------------------------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------------------------------
CScene::~CScene()
{

}

//================================================================================
// CScene::�S�X�V
//--------------------------------------------------------------------------------
void CScene::UpdateAll()
{
	// ���݃��[�h�̎擾
	CMode::MODETYPE mode = CManager::GetMode()->GetModeType();

	//-------- �S�X�V
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// �X�V
		for (CScene* pScene : m_lisScene[cntPriority])
		{
			// ���݃��[�h���u���[�h�v�̏ꍇ�A���[�h���̃��[�h�̏����͍s��Ȃ�
			if (mode == CMode::LOADING && pScene->m_createModeType != CMode::LOADING) { continue; }

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

//================================================================================
// CScene::�v���C�I���e�B�w��X�V
//--------------------------------------------------------------------------------
void CScene::UpdateAll(int priority)
{
	// ���݃��[�h�̎擾
	CMode::MODETYPE mode = CManager::GetMode()->GetModeType();

	// �X�V
	for (CScene* pScene : m_lisScene[priority])
	{
		// ���݃��[�h���u���[�h�v�̏ꍇ�A���[�h���̃��[�h�̏����͍s��Ȃ�
		if (mode == CMode::LOADING && pScene->m_createModeType != CMode::LOADING) { continue; }

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

//================================================================================
// CScene::�t���O�t����scene��S����
//--------------------------------------------------------------------------------
void CScene::RemoveAll()
{
	//-------- �S�����[�X
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
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

//================================================================================
// CScene::�S�`��
//--------------------------------------------------------------------------------
void CScene::DrawAll()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXVECTOR3 camPos = CManager::GetCamera()->GetPos();

	// ���݃��[�h�̎擾
	CMode::MODETYPE mode = CManager::GetMode()->GetModeType();

	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// �J��������̋����ɂ���ă\�[�g
		if (mode != CMode::LOADING)
		{
			m_lisScene[cntPriority].sort([camPos](CScene* pSceneA, CScene* pSceneB)
			{
				D3DXVECTOR3 vecA = camPos - pSceneA->GetPos();
				D3DXVECTOR3 vecB = camPos - pSceneB->GetPos();
				return D3DXVec3LengthSq(&vecA) > D3DXVec3LengthSq(&vecB);
			});
		}

		for (CScene* pScene : m_lisScene[cntPriority])
		{
			// ���݃��[�h���u���[�h�v�̏ꍇ�A���[�h���̃��[�h�̏����͍s��Ȃ�
			if (mode == CMode::LOADING && pScene->m_createModeType != CMode::LOADING) { continue; }

			// �B��`�攻��
			if (m_drawTypeOnly != CLASSTYPE_NONE && pScene->m_classType != m_drawTypeOnly) { continue; }

			if (!pScene->m_drawFlag) { continue; }		// �`�悷�邩

			if (pScene->m_lightOff) pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ���C�g�\���I�t
			if (pScene->m_alpha) pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	// DEST��RGB�̌W�����P�ɂ���

			// �`�揈��
			pScene->Draw();

			if (pScene->m_alpha)
			{
				//�����_�[�X�e�[�g�̐ݒ��߂�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
			if (pScene->m_lightOff)pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	// ���C�g�̕\����L���ɂ���
		}
	}
}

//================================================================================
// CScene::�f�t�@�[�h�����_�����O�p�S�`��
//--------------------------------------------------------------------------------
void CScene::DrawDefferdRenderingAll()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXVECTOR3 camPos = CManager::GetCamera()->GetPos();

	// ���݃��[�h�̎擾
	CMode::MODETYPE mode = CManager::GetMode()->GetModeType();

	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// �J��������̋����ɂ���ă\�[�g
		if (mode != CMode::LOADING)
		{
			m_lisScene[cntPriority].sort([camPos](CScene* pSceneA, CScene* pSceneB)
			{
				D3DXVECTOR3 vecA = camPos - pSceneA->GetPos();
				D3DXVECTOR3 vecB = camPos - pSceneB->GetPos();
				return D3DXVec3LengthSq(&vecA) > D3DXVec3LengthSq(&vecB);
			});
		}

		for (CScene* pScene : m_lisScene[cntPriority])
		{
			// ���݃��[�h���u���[�h�v�̏ꍇ�A���[�h���̃��[�h�̏����͍s��Ȃ�
			if (mode == CMode::LOADING && pScene->m_createModeType != CMode::LOADING) { continue; }

			// �B��`�攻��
			if (m_drawTypeOnly != CLASSTYPE_NONE && pScene->m_classType != m_drawTypeOnly) { continue; }

			if (!pScene->m_drawFlag) { continue; }		// �`�悷�邩

			if (pScene->m_lightOff) pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ���C�g�\���I�t
			if (pScene->m_alpha) pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	// DEST��RGB�̌W�����P�ɂ���

			// �`�揈��
			pScene->DrawDefferdRendering();

			if (pScene->m_alpha)
			{
				//�����_�[�X�e�[�g�̐ݒ��߂�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
			if (pScene->m_lightOff)pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	// ���C�g�̕\����L���ɂ���
		}
	}
}

//================================================================================
// CScene::�v���C�I���e�B�w��`��
//--------------------------------------------------------------------------------
void CScene::DrawAll(int priority)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXVECTOR3 camPos = CManager::GetCamera()->GetPos();

	// ���݃��[�h�̎擾
	CMode::MODETYPE mode = CManager::GetMode()->GetModeType();

	// �J��������̋����ɂ���ă\�[�g
	if (mode != CMode::LOADING)
	{
		m_lisScene[priority].sort([camPos](CScene* pSceneA, CScene* pSceneB)
		{
			D3DXVECTOR3 vecA = camPos - pSceneA->GetPos();
			D3DXVECTOR3 vecB = camPos - pSceneB->GetPos();
			return D3DXVec3LengthSq(&vecA) > D3DXVec3LengthSq(&vecB);
		});
	}

	for (CScene* pScene : m_lisScene[priority])
	{
		// ���݃��[�h���u���[�h�v�̏ꍇ�A���[�h���̃��[�h�̏����͍s��Ȃ�
		if (mode == CMode::LOADING && pScene->m_createModeType != CMode::LOADING) { continue; }

		// �B��`�攻��
		if (m_drawTypeOnly != CLASSTYPE_NONE && pScene->m_classType != m_drawTypeOnly) { continue; }

		if (!pScene->m_drawFlag) { continue; }		// �`�悷�邩

		if (pScene->m_lightOff)pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ���C�g�\���I�t
		if (pScene->m_alpha) pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	// DEST��RGB�̌W�����P�ɂ���

		// �`�揈��
		pScene->Draw();

		if (pScene->m_alpha)
		{
			//�����_�[�X�e�[�g�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
		if (pScene->m_lightOff)pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	// ���C�g�̕\����L���ɂ���
	}
}

//================================================================================
// CScene::�S�����[�X
//--------------------------------------------------------------------------------
void CScene::ReleaseAll()
{
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// �Sscene�̍폜
		for (auto pScene : m_lisScene[cntPriority])
		{
			//---- ���
			pScene->Uninit();
			delete pScene;
			pScene = nullptr;
		}
		m_lisScene[cntPriority].clear();
	}
}

//================================================================================
// CScene::�t���O��false�̃V�[����S�����[�X
//--------------------------------------------------------------------------------
void CScene::FlagReleaseAll()
{
	for (int cntPriority = 0; cntPriority < MAX_SCENE_PRIORITY; cntPriority++)
	{
		// �폜�t���O�����Ă�
		for (auto pScene : m_lisScene[cntPriority])
		{
			//---- true�Ȃ̂Ŗ���
			if (pScene->m_overall == true) {
				continue;
			}

			//---- ���
			pScene->Release();
		}
	}
	// �폜�t���O�������Ă���scene�̏���
	RemoveAll();
}

//================================================================================
// CScene::�w��^�C�v�S�����[�X
//--------------------------------------------------------------------------------
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

//================================================================================
// �w��^�C�v�擾
//--------------------------------------------------------------------------------
CScene *CScene::GetSceneClassType(const CLASSTYPE &selectObjType)
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

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

//--------------------------------------------------------------------------------
// Device���L���b�N���ăo�b�t�@��������
//--------------------------------------------------------------------------------
void CScene::WriteBuffer(LPDIRECT3DVERTEXBUFFER9 pVtxBuffer, VERTEX_3D* pInVtx, int numVtx
						,LPDIRECT3DINDEXBUFFER9 pIdxBuffer, WORD *pInIdx, int numIdx)
{
	// �X���b�h�Z�[�t(���b�N)
#ifdef THREAD_ON
	std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif

	if (pVtxBuffer)
	{
		VERTEX_3D* pVtx;	// �o�b�t�@�����b�N����
		pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		for (int cntVtx = 0; cntVtx < numVtx; cntVtx++)
		{
			pVtx[cntVtx].pos = pInVtx[cntVtx].pos;
			pVtx[cntVtx].nom = pInVtx[cntVtx].nom;
			pVtx[cntVtx].col = pInVtx[cntVtx].col;
			pVtx[cntVtx].tex = pInVtx[cntVtx].tex;
		}

		//���b�N�̉���
		pVtxBuffer->Unlock();
	}

	if (pIdxBuffer)
	{
		WORD* pIdx;	// �o�b�t�@�����b�N����
		pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

		for (int cntIdx = 0; cntIdx < numIdx; cntIdx++)
		{
			pIdx[cntIdx] = pInIdx[cntIdx];
		}

		//���b�N�̉���
		pIdxBuffer->Unlock();
	}
}