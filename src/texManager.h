//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifndef _TEXMANAGER_H_
#define _TEXMANAGER_H_

#include "main.h"
#include <algorithm>
#include <unordered_map>

//================================================================================
// �N���X
//--------------------------------------------------------------------------------
class CTexManager
{
public:
	CTexManager(){};		// �R���X�g���N�^
	~CTexManager(){};		// �f�X�g���N�^
	
	void Init();		// ������
	void Uninit();		// �I��

	bool SetTex(const std::string &strFilePass);						// �e�N�X�`���̐ݒ�
	bool SetTex(const std::string &strFilePass, LPDIRECT3DTEXTURE9 texInterface);
	LPDIRECT3DTEXTURE9 GetTexInterface(const std::string &strFilePass);	// �e�N�X�`���o�b�t�@�̎擾
	bool CheckTex(const std::string &strFilePass);						// �e�N�X�`�����̗L�����m�F
	
	bool Release(const std::string &strFilePass);						// ���
	void ReleaseAll();													// �S�J��
	
private:
	std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_umapTexInterface;		// �C���^�[�t�F�C�X
};

#endif