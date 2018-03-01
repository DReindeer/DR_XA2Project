// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : BOXのシーン(使わないかも)
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _SCENE3DBOX_H_
#define _SCENE3DBOX_H_

#include "main.h"
#include <string>
#include "scene3D.h"

#define NUM_BOX_VERTEX (18)

// -------- -------- -------- -------- -------- -------- -------- --------
// クラス
// -------- -------- -------- -------- -------- -------- -------- --------
class CSceneBox : public CScene3D
{
public:

	CSceneBox(int priority = 4) :CScene3D(priority), m_pVertexBuffer(nullptr) { m_classType = CScene::CLASSTYPE::SCENE_3DBOX; }
	~CSceneBox() {}

	void Init(void);				// 初期化
	void CreateVtxBuffer(void);
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画
	static CSceneBox *Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &size, const std::string &strFilePass, int priority = 4);

	XMFLOAT3 GetMove() { return m_move; }
	void SetMove(XMFLOAT3 move) { m_move = move; }

private:
	ID3D11Buffer *m_pVertexBuffer;			// 頂点バッファ
	ID3D11Buffer *m_pIndexBuffer;			// インデックスバッファ
	VERTEX_3D m_pVtxData[NUM_BOX_VERTEX];	// 頂点データ

	XMFLOAT3 m_move;
};

#endif
