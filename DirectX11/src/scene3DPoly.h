// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 3Dポリゴンのシーン
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _SCENE3DPOLY_H_
#define _SCENE3DPOLY_H_

#include "main.h"
#include <string>
#include "scene3D.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// クラス
// -------- -------- -------- -------- -------- -------- -------- --------
class CScene3DPoly : public CScene3D
{
public:
	CScene3DPoly(int priority = 4) :CScene3D(priority), m_pVertexBuffer(nullptr) { m_classType = CScene::CLASSTYPE::SCENE_3DPOLY; }
	~CScene3DPoly() {}

	void Init(void);				// 初期化
	void CreateVtxBuffer(void);
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画
	static CScene3DPoly *Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &scl, const std::string &strFilePass, int priority = 4);

private:
	ID3D11Buffer *m_pVertexBuffer;			// 頂点バッファ
	ID3D11Buffer *m_pIndexBuffer;			// インデックスバッファ
	VERTEX_3D m_pVtxData[NUM_VERTEX];		// 頂点データ
};

#endif
