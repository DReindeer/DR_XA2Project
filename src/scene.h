//================================================================================
//
//	Auter : KENSUKE WATANABE
//	Data  : 2018/02/10
//
//--------------------------------------------------------------------------------
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
#include "mode.h"
#include "manager.h"
#include <list>
#include <mutex>

//--------------------------------------------------------------------------------
// �萔��`
//--------------------------------------------------------------------------------
#define NUM_VERTEX		( 4 )			// ���_��
#define NUM_POLYGON		( 2 )			// �|���S����(�v���~�e�B�u��)
#define MAX_SCENE_PRIORITY	( 10 )		// �\���D�揇��

// ���_�t�H�[�}�b�g
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//���_�t�H�[�}�b�g	DIFFUSE:�F TEX1:�e�N�X�`�����W(UV��ST)
// ���_�t�H�[�}�b�g
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//--------------------------------------------------------------------------------
// �\���̐錾
//--------------------------------------------------------------------------------
typedef struct
{// ���_�\����
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nom;	// �@�����W
	D3DXVECTOR4 col;	// �F
	D3DXVECTOR2 tex;	// UV
}VERTEX_3D;

typedef struct
{
	D3DXVECTOR3 pos;	// 
	float rhw;			// ���W�ϊ��ςݒ��_�t���O(�K��1.f������)
	D3DCOLOR col;		// ���_�̐F
	D3DXVECTOR2 tex;	// �e�N�X�`��
}VERTEX_2D;

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------
class CScene
{
public:
	// �I�u�W�F�N�g�̎��
	typedef enum
	{
		OBJTYPE_NONE = 0,		// ����
		MODEL,
		PLAYER,					// �v���C���[
		ENEMY,					// �G
		GRAVITY_OBJ,			// 
		BULLET,					// �e
		EXPLOSION,				// ����
		EFFECT,					// �G�t�F�N�g
		PARTICLE,				// �p�[�e�B�N��
		FIELD,					// 
		FIELDX,
		FONT,					// 
		OBJTYPE_MAX				// �ő�l
	}OBJTYPE;

	// �I�u�W�F�N�g�̃N���X�̎��
	typedef enum
	{
		CLASSTYPE_NONE = 0,		// ����
		SCENE_2D,				// 

		SCENE_3DPOLY,			// 
		SCENE_MODEL,			// ���f��
		SCENE_CYLINDER,			// �V�����_�[
		SCENE_FIELD,			// �t�B�[���h
		SCENE_SKYDOME,			// �X�J�C�h�[��
		CLASSTYPE_MAX			// �ő�l
	}CLASSTYPE;

	// �|���S���̎��
	typedef enum
	{
		POLYGON_TYPE_NONE = -1,
		POLYGON_TYPE_2D = 0,	// 2D
		POLYGON_TYPE_3D,		// 3D
	}POLYGON_TYPE;

	CScene(int priority);		// 
	~CScene();					// �f�X�g���N�^

	virtual void Init() = 0;	// ������
	virtual void Uninit() = 0;	// �I��
	virtual void Update() = 0;	// �X�V
	virtual void Draw() {};		// �`��
	virtual void DrawDefferdRendering(){}	// �f�t�@�[�h�����_�����O�p

	static void UpdateAll(int priority);	// �w��priority�S�X�V
	static void UpdateAll();				// �S�X�V
	static void DrawAll(int priority);		// �w��priority�S�`��
	static void DrawAll();					// �S�`��
	static void DrawDefferdRenderingAll();	// �f�t�@�[�h�����_�����O�p�S�`��
	void Release() { m_delete = true; }		// �����[�X
	static void RemoveAll();
	static void ReleaseAll();				// �S�����[�X
	static void FlagReleaseAll();
	static void TypeRelease(const OBJTYPE &selectObjType);

	// Device���L���b�N���ăo�b�t�@��������
	static void WriteBuffer(LPDIRECT3DVERTEXBUFFER9 pVtxBuffer, VERTEX_3D* pInVtx, int numVtx
							,LPDIRECT3DINDEXBUFFER9 pIdxBuffer, WORD *pInIdx, int numIdx);

	// �擾
	LPDIRECT3DVERTEXBUFFER9 *GetVtxBuffer() { return &m_pVtxBuffer; }
	const D3DXVECTOR3 &GetPos() { return m_pos; }		// ���W
	const D3DXVECTOR3 &GetScl() { return m_scl; }		// �g�嗦
	const D3DXVECTOR3 &GetRot() { return m_rot; }		// �p�x
	const D3DXVECTOR3 &GetSize() { return m_size; }		// �T�C�Y
	const int &GetR() { return m_r; }
	const int &GetG() { return m_g; }
	const int &GetB() { return m_b; }
	const int &GetA() { return m_a; }

	// �ݒ�
	void SetPos(const D3DXVECTOR3 &pos) { m_pos = pos; }		// ���W
	void SetScl(const D3DXVECTOR3 &scl) { m_scl = scl * CManager::GetMasterScale();	}		// �g�嗦
	void SetScl(const float &scl) { m_scl = D3DXVECTOR3(scl, scl, scl); }		// �g�嗦
	void SetRot(const D3DXVECTOR3 &rot) { m_rot = rot; }		// �p�x
	void SetSize(const D3DXVECTOR3 &size) { m_size = size; }	// �T�C�Y
	void SetCol(int r, int g, int b, int a) { m_r = r; m_g = g; m_b = b; m_a = a; }

	// ���Z
	void AddPos(const D3DXVECTOR3 &pos) { m_pos += pos; }		// ���W
	void AddScl(const D3DXVECTOR3 &scl) { m_scl += scl * CManager::GetMasterScale(); }		// �g�嗦
	void AddScl(const float &scl) { m_scl += D3DXVECTOR3(scl, scl, scl) * CManager::GetMasterScale();}		// �g�嗦
	void AddRot(const D3DXVECTOR3 &rot) { m_rot += rot; }		// �p�x
	void AddSize(const D3DXVECTOR3 &size) { m_size += size; }	// �T�C�Y
	void AddCol(const int &r, const int &g, const int &b, const int &a) { m_r += r; m_g += g; m_b += b; m_a += a; }

	// �^�C�v
	void SetObjType(OBJTYPE type) { m_objType = type; }			// �������ɃI�u�W�F�N�g�̃^�C�v��ݒ�
	OBJTYPE GetObjType() { return m_objType; }					// �I�u�W�F�N�g�^�C�v�̎擾
	void SetClassType(CLASSTYPE type) { m_classType = type; }	// �������ɃN���X�̃^�C�v��ݒ�
	CLASSTYPE GetClassType() { return m_classType; }			// �N���X�^�C�v�̎擾

// �`��D��x
	const int &GetPriority() { return m_priority; }				// priority�̎擾

// �B��`��
	static void DrawTypeOnly(CLASSTYPE classType) {
#ifdef THREAD_ON
		std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
		m_drawTypeOnly = classType;
	}
	static CScene *GetSceneClassType(const CLASSTYPE &selectObjType);

	// scene
	static std::list<CScene*> *GetScene(int priority) {
#ifdef THREAD_ON
		std::unique_lock<std::recursive_mutex> locker = CManager::Locker();
#endif
		if (priority < 0 || priority >= MAX_SCENE_PRIORITY)MessageBox(nullptr, "PRIORITY�̎w�肪�Ԉ���Ă��܂�", "�x��", MB_OK);
		return &m_lisScene[priority];			// �V�[���̐擪
	}

	// ���C�g
	bool GetLightOff() { return m_lightOff; }
	void SetLightOff(bool lightOff = true) { m_lightOff = lightOff; }
	// �A���t�@
	bool GetAlpha() { return m_alpha; }
	void SetAlpha(bool alpha = true) { m_alpha = alpha; }
	// �`�悷�邩
	bool GetDraw() { return m_drawFlag; }
	void SetDrawFlag(bool draw = true) { m_drawFlag = draw; }
	// �V�[���ׂ��ŏ����Ȃ��łق����t���O
	bool GetOverall() { return m_overall; }
	void SetOverall(bool overall = true) { m_overall = overall; }

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// Vtx�o�b�t�@

	D3DXVECTOR3 m_pos;			// ���W
	D3DXVECTOR3 m_rot;			// ��]�p
	D3DXVECTOR3 m_size;			// �T�C�Y
	int m_r, m_g, m_b, m_a;

	OBJTYPE m_objType;			// �I�u�W�F�N�g�̃^�C�v
	CLASSTYPE m_classType;		// �N���X�̃^�C�v
	int m_priority;				// �`��D��x
	static CLASSTYPE m_drawTypeOnly;	// �B��`�悵�������̂�����`��

	bool m_lightOff;
	bool m_alpha;
	bool m_drawFlag;

private:
	D3DXVECTOR3 m_scl;				// �g�嗦
	static std::list<CScene*> m_lisScene[MAX_SCENE_PRIORITY];
	bool m_delete;					// ����t���O
	bool m_overall;					// �J�ڂŏ�����Ȃ��l�ɂ���t���O
	CMode::MODETYPE m_createModeType;
};

#endif
