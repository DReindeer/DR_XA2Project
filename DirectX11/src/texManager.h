// ======== ======== ======== ======== ======== ======== ======== ========
//
//	title : classMake[ texManager.h ]		Auter : KENSUKE WATANABE
//													Data  : 2017/04/27
//
// -------- -------- -------- -------- -------- -------- -------- --------
//��Update : 2016/11/17
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _TEXMANAGER_H_
#define _TEXMANAGER_H_

#include "main.h"
#include <unordered_map>

#include "DirectXTex.h"
#if _DEBUG
#pragma comment(lib, "DirectXTex-d.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#endif


// ======== ======== ======== ======== ======== ======== ======== ========
// �N���X
// -------- -------- -------- -------- -------- -------- -------- --------
class CTexManager
{
public:

	CTexManager(){};		// �R���X�g���N�^
	~CTexManager(){};		// �f�X�g���N�^
	
    void Init(void);		// ������
    void Uninit(void);		// �I��

	bool SetTex(const std::string &strFilePass);						// �e�N�X�`���̐ݒ�
	ID3D11ShaderResourceView *GetTexInterface(const std::string &strFilePass);	// �e�N�X�`���o�b�t�@�̎擾
	bool CheckTex(const std::string &strFilePass);						// �e�N�X�`�����̗L�����m�F
	
	bool Release(const std::string &strFilePass);						// ���
	void ReleaseAll(void);												// �S�J��
	
private:
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_unmapTexInterface;		// �C���^�[�t�F�C�X
};

#endif