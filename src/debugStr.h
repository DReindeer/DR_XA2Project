//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/04/27
//
//--------------------------------------------------------------------------------
#ifdef _DEBUG		// Debug���̂ݎ��s
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

//--------------------------------------------------------------------------------
// �萔��`
//--------------------------------------------------------------------------------
#define DEBUG_DISPLAY_MAX			(100)	//�f�o�b�O�p�\���o�^�\�ő吔
#define DEBUG_DISPLAY_CHAR_MAX		(20)	//�f�o�b�O�p�\���ϐ����ő啶����
#define DEBUG_DISPLAY_LINE			(20)	//�f�o�b�O�p�\����
#define DEBUG_DISPLAY_VARIABLE_MAX	(3)		//�f�o�b�O�p�\���ő�ϐ���
#define DEBUG_MAX					(512)	//�f�o�b�O�\��������

#define DEBUG_DISPLAY_POS_Y			(20)	//�f�o�b�O�\���ʒu(Y���W)

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class CDebugStr
{
public:
	CDebugStr();
	~CDebugStr();

	void Init(int line, std::string name, std::vector<float> vecVal);
	void Update();
	void Draw();
	static void UpdateAll();
	static void DrawAll();
	static void ReleaseAll();

	static void Create(int line, std::string name, std::vector<float> vecVal);
	static void Set(std::string name, std::vector<float> vecVal);

private:
	int m_nLine;					// �\���s
	int m_nNumValue;				// �\���ϐ���
	std::vector<float> m_vecValue;	// �\���l
	std::string m_aName;			// �\����

	LPD3DXFONT m_pDebugFont;		// �t�H���g�ւ̃C���^�[�t�F�[�X�|�C���^
	std::string m_strDebug;			// �f�o�b�O�\��������

	static std::unordered_map<std::string, CDebugStr*> m_umapDebugStr;
};

#endif
#endif