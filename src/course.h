//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2017/12/17
//
//--------------------------------------------------------------------------------
#ifndef _COURSE_H_
#define _COURSE_H_

#include "main.h"
#include "sceneFieldX.h"
#include <assert.h>

//================================================================================
// �N���X��`
//--------------------------------------------------------------------------------
// �O���錾
class CSceneFieldX;
class SceneCheckPoint;
class CMiniMap;
class CScene2D;
class CCourse
{
public:
	CCourse() : m_currentCheckPoint(0), m_goal(false), m_pMiniMap(nullptr) {}
	~CCourse() {}

	void Init();	// ������
	void Uninit();	// �I��
	void Update();	// �X�V
	void Draw();	// �`��
	static CCourse *Create(int pry = 4);	// ����

	const D3DXVECTOR3 &GetCheckPoint(const int &index);
	const D3DXVECTOR3 &GetCourseSize() { return m_pCourse->GetSize(); }
	const D3DXVECTOR3 &GetCourseCenterPos() { return m_pCourse->GetCenterPos(); }
	const D3DXVECTOR3 &GetStartPos() { return m_startPos; }
	const D3DXVECTOR2 &GetMapWorldSize() { return m_mapWorldSize; }
	const int &GetCurrentCheckPoint() { return m_currentCheckPoint; }

	const bool &GetGoal()const { return m_goal; }

	int GetGoalCount() { return m_goalCount; }
	int GetReadyCount() { return m_readyCount; }

protected:
	CSceneFieldX *m_pCourse;			// �R�[�X
	SceneCheckPoint *m_checkPoint[5];	// �`�F�b�N�|�C���g
	CMiniMap *m_pMiniMap;				// �~�j�}�b�v

	D3DXVECTOR3 m_startPos;				// �X�^�[�g�ʒu
	D3DXVECTOR2 m_mapWorldSize;			// �R�[�X�̋��E���̃T�C�Y - �~�j�}�b�v�Ŏg�p

	int m_currentCheckPoint;			// ���݌o�߂����`�F�b�N�|�C���g��
	bool m_goal;						// �S�[������

	CScene2D *m_countDown;				// �J�E���g�_�E��
	CScene2D *m_sign;					// �X�^�[�g,�S�[��
	CScene2D *m_lapSign;				// ���b�v��m�点��

	int m_goalCount;
	int m_startCount;
	int m_readyCount;
	int m_signDrawCount;
	bool m_signDrawFlag;
};

#endif
