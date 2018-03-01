// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 2D�|���S���̃V�[��
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include <string>
#include "scene.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// �\���̐錾
// -------- -------- -------- -------- -------- -------- -------- --------
typedef struct
{
	XMFLOAT3 pos;		// ���_���W
	XMFLOAT4 col;		// ���_�F
	XMFLOAT2 tex;		// �e�N�X�`�����W
}VERTEX_2D;

// -------- -------- -------- -------- -------- -------- -------- --------
// �N���X
// -------- -------- -------- -------- -------- -------- -------- --------
class CScene2D : public CScene
{
public:
	CScene2D(int priority = 4) : CScene(priority), m_pVertexBuffer(nullptr) { m_classType = CScene::CLASSTYPE::SCENE_3DPOLY; }
	~CScene2D() {}

	void Init(void);				// ������
	void CreateVtxBuffer(void);
	void Uninit(void);				// �I��
	void Update(void);				// �X�V
	void Draw(void);				// �`��
	static CScene2D *Create(const XMFLOAT2 &pos, const float &rot, const XMFLOAT2 &scl, const std::string &strFilePass, int priority = 4);

private:
	ID3D11Buffer *m_pVertexBuffer;			// ���_�o�b�t�@
	ID3D11Buffer *m_pIndexBuffer;			// �C���f�b�N�X�o�b�t�@
	VERTEX_2D m_pVtxData[NUM_VERTEX];		// ���_�f�[�^

protected:
	XMFLOAT2 m_posUV;						// �e�N�X�`�����W
	float m_widthUV;						// ��
	float m_heightUV;						// ����
	std::string m_strFilePass;				// �e�N�X�`��FilePass

	// �萔�o�b�t�@
	ID3D11Buffer *m_pConstantBufferModel;
	ID3D11Buffer *m_pConstantBufferMat;		// �}�e���A��

	// �V�F�[�_�[
	ID3D11VertexShader *m_pVertexShader;	// ���_
	ID3D11PixelShader *m_pPixelShader;		// �s�N�Z��
	ID3D11PixelShader *m_pPixelTexShader;	// �s�N�Z��
	ID3D11InputLayout *m_pVertexLayout;		// ���C�A�E�g

	ID3D11SamplerState *m_pTexSampler;		// �T���v���[
	static ID3D11Buffer *m_pConstantBuffer2DCam;
};

#endif
