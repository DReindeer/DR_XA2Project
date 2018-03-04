// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : X���f���̃V�[��
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
//#include <tchar.h>
#include "manager.h"
#include "texManager.h"
#include "modelManager.h"
#include "XModelLoader.h"
#include "sceneXModelListener.h"
#include "renderer.h"
#include "camera.h"
#include "utility.h"

#include "xa2Listener.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// ������
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModelListener::Init(void)
{
	// �e�̏�����
	CSceneXModel::Init();
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �I��
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModelListener::Uninit(void)
{
	// �e�̏I��
	CSceneXModel::Uninit();
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �X�V
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModelListener::Update(void)
{

}

// -------- -------- -------- -------- -------- -------- -------- --------
// �`��
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModelListener::Draw(void)
{
	// �e�̕`��
	CSceneXModel::Draw();
}

// -------- -------- -------- -------- -------- -------- -------- --------
// �쐬����
// -------- -------- -------- -------- -------- -------- -------- --------
CSceneXModelListener *CSceneXModelListener::Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &scl, const std::string &strFilePass, int priority)
{
	// CSceneXModelListener�̐���
	CSceneXModelListener *pXModel;
	pXModel = new CSceneXModelListener(priority);
	pXModel->Init();

	pXModel->SetPos(pos);
	pXModel->SetRot(rot);
	pXModel->SetScl(scl);
	pXModel->m_strFilePass = strFilePass;

	// ���f���̍쐬
	CManager::GetXModelManager()->SetModel(strFilePass);

	// ���_, �V�F�[�_�[�̍쐬
	pXModel->CreateVtxBuffer();

	return pXModel;
}
