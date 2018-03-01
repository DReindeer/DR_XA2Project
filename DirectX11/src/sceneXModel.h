// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : X���f���̃V�[��
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

// X���f���̃}�e���A��������̃��b�V���p�̃f�[�^�\��
// -------- -------- -------- -------- -------- -------- -------- --------
struct ModelBuffer
{
	std::vector<VERTEX_3D> vecVtxData;	// ���_�f�[�^
	ID3D11Buffer *pVertexBuffer;		// Vtx�o�b�t�@
	ID3D11Buffer *pIndexBuffer;			// �C���f�b�N�X�o�b�t�@
	ID3D11SamplerState *pTexSampler;	// �e�N�X�`���T���v���[
	int numIdx;							// �C���f�b�N�X��
};

// -------- -------- -------- -------- -------- -------- -------- --------
// �N���X
// -------- -------- -------- -------- -------- -------- -------- --------
class CSceneXModel : public CScene3D
{
public:
	CSceneXModel(int priority = 4) :CScene3D(priority) { m_classType = CScene::CLASSTYPE::SCENE_MODEL; }
	~CSceneXModel() {}

	void Init(void);				// ������
	void CreateVtxBuffer(void);
	void Uninit(void);				// �I��
	void Update(void);				// �X�V
	void Draw(void);				// �`��
	static CSceneXModel *Create(const XMFLOAT3 &pos, const XMFLOAT3 &rot, const XMFLOAT3 &scl, const std::string &strFilePass, int priority = 4);

	XMFLOAT3 GetMove() { return m_move; }
	void SetMove(XMFLOAT3 move) { m_move = move; }

private:
	std::vector<ModelBuffer> m_vecModelBuffer;
	XMFLOAT3 m_move;
};

#endif
