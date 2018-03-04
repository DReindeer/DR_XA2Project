// ======== ======== ======== ======== ======== ======== ======== ========
// 
// 
//	title : X���f���̃V�[��
//	Auter : KENSUKE WATANABE
// 
// 
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _SCENEXMODELLISTENER_H_
#define _SCENEXMODELLISTENER_H_

#include "sceneXModel.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// �N���X
// -------- -------- -------- -------- -------- -------- -------- --------
class XA2Listener;
class CSceneXModelListener : public CSceneXModel
{
public:
	CSceneXModelListener(int priority = 4) :CSceneXModel(priority) { 
		m_classType = CScene::CLASSTYPE::SCENE_MODEL;
		m_objType = CScene::OBJTYPE::LISTENER;
	}
	~CSceneXModelListener() {}

	void Init(void);				// ������
	void Uninit(void);				// �I��
	void Update(void);				// �X�V
	void Draw(void);				// �`��
	static CSceneXModelListener *Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &scl, const std::string &strFilePass, int priority = 4);

	XA2Listener *GetListener() { return m_pListener; }

private:
	// ���X�i�[
	XA2Listener *m_pListener = nullptr;
};

#endif
