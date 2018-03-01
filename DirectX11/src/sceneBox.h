// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : BOX�̃V�[��(�g��Ȃ�����)
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
// �N���X
// -------- -------- -------- -------- -------- -------- -------- --------
class CSceneBox : public CScene3D
{
public:

	CSceneBox(int priority = 4) :CScene3D(priority), m_pVertexBuffer(nullptr) { m_classType = CScene::CLASSTYPE::SCENE_3DBOX; }
	~CSceneBox() {}

	void Init(void);				// ������
	void CreateVtxBuffer(void);
	void Uninit(void);				// �I��
	void Update(void);				// �X�V
	void Draw(void);				// �`��
	static CSceneBox *Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &size, const std::string &strFilePass, int priority = 4);

	XMFLOAT3 GetMove() { return m_move; }
	void SetMove(XMFLOAT3 move) { m_move = move; }

private:
	ID3D11Buffer *m_pVertexBuffer;			// ���_�o�b�t�@
	ID3D11Buffer *m_pIndexBuffer;			// �C���f�b�N�X�o�b�t�@
	VERTEX_3D m_pVtxData[NUM_BOX_VERTEX];	// ���_�f�[�^

	XMFLOAT3 m_move;
};

#endif
