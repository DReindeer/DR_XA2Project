// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 3D�̃V�[���̐e�N���X
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include <string>
#include "scene.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// �萔��`
// -------- -------- -------- -------- -------- -------- -------- --------
// ���_�t�H�[�}�b�g
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//���_�t�H�[�}�b�g	DIFFUSE:�F TEX1:�e�N�X�`�����W(UV��ST)

// -------- -------- -------- -------- -------- -------- -------- --------
// �\���̐錾
// -------- -------- -------- -------- -------- -------- -------- --------
typedef struct
{
	XMFLOAT3 pos;		// ���_���W
	XMFLOAT3 nom;		// �@�����W
	XMFLOAT4 col;		// ���_�F
	XMFLOAT2 tex;		// �e�N�X�`�����W
}VERTEX_3D;

// -------- -------- -------- -------- -------- -------- -------- --------
// �N���X
// -------- -------- -------- -------- -------- -------- -------- --------
class CScene3D : public CScene
{
public:
	CScene3D(int priority = 4) : CScene(priority) { m_classType = CLASSTYPE::SCENE_3D; }
	~CScene3D() {}

	void Init(void);				// ������
	void Uninit(void);				// �I��
	void Update(void);				// �X�V
	void Draw(void);				// �`��

	void SetStrFilePass(std::string strFilePass) { m_strFilePass = strFilePass; }
	const std::string &GetStrFilePass(void) { return m_strFilePass; }
	void SetTexturePos(XMFLOAT2 uv, XMFLOAT2 length);

protected:
	XMFLOAT2 m_posUV;				// �e�N�X�`�����W
	float m_widthUV;				// ��
	float m_heightUV;				// ����
	std::string m_strFilePass;		// �e�N�X�`��FilePass

	// �萔�o�b�t�@
	ID3D11Buffer *m_pConstantBufferModel;
	ID3D11Buffer *m_pConstantBufferMat;		// �}�e���A��

	// �V�F�[�_�[
	ID3D11VertexShader *m_pVertexShader;	// ���_
	ID3D11PixelShader *m_pPixelShader;		// �s�N�Z��
	ID3D11PixelShader *m_pPixelTexShader;	// �s�N�Z��
	ID3D11InputLayout *m_pVertexLayout;		// ���C�A�E�g

	ID3D11SamplerState *m_pTexSampler;		// �T���v���[
};

#endif
