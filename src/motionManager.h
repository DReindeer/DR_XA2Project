#ifndef _MOTIONMANAGER_H_
#define _MOTIONMANAGER_H_

#include "main.h"
#include <string>
#include <unordered_map>

// --------���[�V�����ǂݍ��݃N���X
class CSceneModel;
class CMotionManager
{
public:
	typedef struct
	{
		int motionType;				// ���݃��[�V�����̎��
		int cntFlame;				// ���݃t���[����
		int numKey;					// ���݃L�[��

		int motionTypeNext;			// ���̃��[�V�����̎��
		int cntBlendFlame;			// ���̃t���[����

		int motionBlendFrame;		// ���[�V�����ԕ�Ԏ��̃t���[����

		std::string motionFilePass;
		std::vector<CSceneModel*> *pVecModel;	// �p�[�c���
	}MOTION_DATA;

	typedef struct
	{
		float move;					// �ړ���
		float jump;					// �W�����v��
		float rad;					// ���a
		int numParts;				// �p�[�c�� (PARTS�N���X�̑���)
	}CHARA;

	// �p�[�c���ꎞ�ǂݎ��p�\����
	typedef struct
	{
		int index, parent;
		D3DXVECTOR3 pos, rot, basePos, baseRot;
		std::string filePass;
	}PARTS;

	// �p�[�c���W���
	typedef struct
	{
		D3DXVECTOR3 pos;			// ���W
		D3DXVECTOR3 rot;			// ��]��
		int index;					// �p�[�c�C���f�b�N�X

		D3DXVECTOR3 posParFlame;	// 1�t���[���Ɉړ�������W
		D3DXVECTOR3 rotParFlame;	// 1�t���[���ɉ�]�����
	}MOVEPARTS;

	// �L�[���
	typedef struct
	{
		int	flame;					// ���s�t���[����
		int index;					// �L�[�C���f�b�N�X

		std::list<MOVEPARTS*> lisMoveParts;	// �ړ��p�[�c
	}KEY;

	// ���[�V�������
	typedef struct
	{
		bool loop;					// �J��Ԃ��̗L��
		int numKey;					// �L�[�� (MOTION����KEY�\���̂̑���)
		int index;					// ���[�V�����C���f�b�N�X

		std::list<KEY*> lisKey;		// �L�[
	}MOTION;

	// �R���X�g���N�^, �f�X�g���N�^
	CMotionManager() {}
	~CMotionManager() {}

	// -------- -------- -------- --------
	// �F���g�����[�V�����Ɋւ���ꏊ
	static void GetCharaPartsList(MOTION_DATA *pMotionData, D3DXVECTOR3 scl = D3DXVECTOR3(1.f, 1.f, 1.f));	// �p�[�c�̃��X�g���擾
	void UpdateMotion(MOTION_DATA *pMotionData);
	void SetMotionType(MOTION_DATA *pMotionData, int motionTypeNext);

	// -------- -------- -------- --------
	// �}�l�[�W���ŋL�q
	// ����
	static void LoadMotion(const std::string &strFilePass);	// ���[�V�����}�l�[�W���ɒǉ�
	// �S����
	static void ReleaseAll();							// ���f���S����

	// �擾
	CHARA GetChara() { return m_chara; }
	CMotionManager *GetMotionManager(std::string strName);			// ���[�V�����}�l�[�W���̎擾
	MOTION *GetMotion(int index);									// ���[�V�����̎擾
	KEY *GetKey(MOTION *pMotion, int index);						// Key�̎擾
	MOVEPARTS *GetMoveParts(KEY *pKey, int index);					// MoveParts�̎擾

private:
	// �����o�֐�

	static bool seekSrcData(char *texName);	// �w�蕶����܂œǂݐi�߂�
	static int getNumInt();				// int�̒l��Ԃ�
	static float getNumFloat();			// float�̒l��Ԃ�
	static char *getChar();				// char�̃|�C���^��Ԃ�
	
	// �����o�ϐ�
	static int m_fileSize;					// �t�@�C��������
	static int m_currentSrc;				// ���݈ʒu
	static char *m_pSrcData;				// ������i�[�p�|�C���^
	static char m_buf[MAX_PATH];			// �o�b�t�@(�ꎞ�����i�[�p)

	int m_numModel;							// ���f����
	CHARA m_chara;							// �L�����N�^�[
	std::vector<PARTS*> m_vecParts;			// �p�[�c
	std::list<MOTION*> m_lisMotion;			// ���[�V����
	static std::unordered_map<std::string, CMotionManager*> m_umapMotionManager;	// ���[�V�����}�l�[�W��
};

#endif