// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : Xモデルのシーン
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
// 初期化
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModelListener::Init(void)
{
	// 親の初期化
	CSceneXModel::Init();
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 終了
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModelListener::Uninit(void)
{
	// 親の終了
	CSceneXModel::Uninit();
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 更新
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModelListener::Update(void)
{

}

// -------- -------- -------- -------- -------- -------- -------- --------
// 描画
// -------- -------- -------- -------- -------- -------- -------- --------
void CSceneXModelListener::Draw(void)
{
	// 親の描画
	CSceneXModel::Draw();
}

// -------- -------- -------- -------- -------- -------- -------- --------
// 作成処理
// -------- -------- -------- -------- -------- -------- -------- --------
CSceneXModelListener *CSceneXModelListener::Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &scl, const std::string &strFilePass, int priority)
{
	// CSceneXModelListenerの生成
	CSceneXModelListener *pXModel;
	pXModel = new CSceneXModelListener(priority);
	pXModel->Init();

	pXModel->SetPos(pos);
	pXModel->SetRot(rot);
	pXModel->SetScl(scl);
	pXModel->m_strFilePass = strFilePass;

	// モデルの作成
	CManager::GetXModelManager()->SetModel(strFilePass);

	// 頂点, シェーダーの作成
	pXModel->CreateVtxBuffer();

	return pXModel;
}
