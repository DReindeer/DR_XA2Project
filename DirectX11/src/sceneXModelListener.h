// ======== ======== ======== ======== ======== ======== ======== ========
// 
// 
//	title : Xモデルのシーン
//	Auter : KENSUKE WATANABE
// 
// 
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _SCENEXMODELLISTENER_H_
#define _SCENEXMODELLISTENER_H_

#include "sceneXModel.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// クラス
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

	void Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画
	static CSceneXModelListener *Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &scl, const std::string &strFilePass, int priority = 4);

	XA2Listener *GetListener() { return m_pListener; }

private:
	// リスナー
	XA2Listener *m_pListener = nullptr;
};

#endif
