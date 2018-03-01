// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 3D�|���S���̃V�[��
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
// �N���X
// -------- -------- -------- -------- -------- -------- -------- --------
class CScene3DPoly : public CScene3D
{
public:
	CScene3DPoly(int priority = 4) :CScene3D(priority), m_pVertexBuffer(nullptr) { m_classType = CScene::CLASSTYPE::SCENE_3DPOLY; }
	~CScene3DPoly() {}

	void Init(void);				// ������
	void CreateVtxBuffer(void);
	void Uninit(void);				// �I��
	void Update(void);				// �X�V
	void Draw(void);				// �`��
	static CScene3DPoly *Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &scl, const std::string &strFilePass, int priority = 4);

private:
	ID3D11Buffer *m_pVertexBuffer;			// ���_�o�b�t�@
	ID3D11Buffer *m_pIndexBuffer;			// �C���f�b�N�X�o�b�t�@
	VERTEX_3D m_pVtxData[NUM_VERTEX];		// ���_�f�[�^
};

#endif
