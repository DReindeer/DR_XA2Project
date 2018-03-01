// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : Xモデルのシーン
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _SCENEXMODEL_H_
#define _SCENEXMODEL_H_

#include "main.h"
#include <string>
#include <vector>
#include "scene3D.h"

// Xモデルのマテリアル分割後のメッシュ用のデータ構造
// -------- -------- -------- -------- -------- -------- -------- --------
struct ModelBuffer
{
	std::vector<VERTEX_3D> vecVtxData;	// 頂点データ
	ID3D11Buffer *pVertexBuffer;		// Vtxバッファ
	ID3D11Buffer *pIndexBuffer;			// インデックスバッファ
	ID3D11SamplerState *pTexSampler;	// テクスチャサンプラー
	int numIdx;							// インデックス数
};

// -------- -------- -------- -------- -------- -------- -------- --------
// クラス
// -------- -------- -------- -------- -------- -------- -------- --------
class CSceneXModel : public CScene3D
{
public:
	CSceneXModel(int priority = 4) :CScene3D(priority) { m_classType = CScene::CLASSTYPE::SCENE_MODEL; }
	~CSceneXModel() {}

	void Init(void);				// 初期化
	void CreateVtxBuffer(void);
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画
	static CSceneXModel *Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &scl, const std::string &strFilePass, int priority = 4);

	XMFLOAT3 GetMove() { return m_move; }
	void SetMove(XMFLOAT3 move) { m_move = move; }

private:
	std::vector<ModelBuffer> m_vecModelBuffer;
	XMFLOAT3 m_move;
};

#endif
