// ======== ======== ======== ======== ======== ======== ======== ========
//
//	title : �|���S���\���v���O����[ scene.h ]		Auter : KENSUKE WATANABE
//													Data  : 2016/05/30
//
// -------- -------- -------- -------- -------- -------- -------- --------
//��Update : 2016/05/30
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _SCENE_H_
#define _SCENE_H_

#include <list>
#include <assert.h>

#include <algorithm>
#include <memory>
#include <fstream>

#include "main.h"
#include "Wmath.h"

// -------- -------- -------- -------- -------- -------- -------- --------
// �萔��`
// -------- -------- -------- -------- -------- -------- -------- --------
#define MAX_SCENE_PRIORITY	( 8 )		// �\���D�揇��
#define NUM_VERTEX		( 4 )			// ���_��
#define NUM_POLYGON		( 2 )			// �|���S����(�v���~�e�B�u��)

struct BinFile
{
	BinFile(const wchar_t* fpath)
	{
		std::ifstream binfile(fpath, std::ios::in | std::ios::binary);

		if (binfile.is_open()) {
			int fsize = static_cast<int>(binfile.seekg(0, std::ios::end).tellg());
			binfile.seekg(0, std::ios::beg);
			std::unique_ptr<char> code(new char[fsize]);
			binfile.read(code.get(), fsize);
			nSize = fsize;
			Bin = std::move(code);
		}

	}
	const void* get() const { return Bin.get(); }
	int size() const { return nSize; }
private:
	int nSize = 0;
	std::unique_ptr<char> Bin;
};

// -------- -------- -------- -------- -------- -------- -------- --------
// �N���X
// -------- -------- -------- -------- -------- -------- -------- --------
class CScene
{
public:
	typedef enum
	{// �I�u�W�F�N�g�̎��
		OBJTYPE_NONE = 0,		// ����
		LISTENER,
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

	typedef enum
	{// �I�u�W�F�N�g�̃N���X�̎��
		CLASSTYPE_NONE = 0,		// ����
		SCENE_2D,				// 
		SCENE_3D,				// 
		SCENE_3DPOLY,			// 
		SCENE_3DBOX,			// ��
		SCENE_MODEL,			// ���f��
		SCENE_CYLINDER,			// �V�����_�[
		SCENE_FIELD,			// �t�B�[���h
		SCENE_SKYDOME,			// �X�J�C�h�[��
		CLASSTYPE_MAX			// �ő�l
	}CLASSTYPE;
	
	CScene(int priority);	// 
	~CScene();				// �f�X�g���N�^

	virtual void Init(void) = 0;	// ������
	virtual void Uninit(void) = 0;	// �I��
	virtual void Update(void) = 0;	// �X�V
	virtual void Draw(void) = 0;	// �`��

	static void UpdateAll(int priority);		// �w��priority�S�X�V
	static void UpdateAll(void);				// �S�X�V
	static void DrawAll(int priority);			// �w��priority�S�`��
	static void DrawAll(void);					// �S�`��
	static void ReleaseAll(void);				// �S�����[�X
	static void TypeRelease(const OBJTYPE &selectObjType);

	// �擾
	XMFLOAT3 GetPos(void)const{return m_pos;}		// ���W
	XMFLOAT3 GetScl(void)const{return m_scl;}		// �g�嗦
	XMFLOAT3 GetRot(void)const{return m_rot;}		// �p�x
	XMFLOAT3 GetSize(void)const{return m_size;}		// �T�C�Y
	const int &GetR()const { return m_r; }
	const int &GetG()const { return m_g; }
	const int &GetB()const { return m_b; }
	const int &GetA()const { return m_a; }

	// �ݒ�
	void SetPos(const XMFLOAT3 &pos){m_pos = pos;}		// ���W
	void SetScl(const XMFLOAT3 &scl){m_scl = scl;}		// �g�嗦
	void SetRot(const XMFLOAT3 &rot){m_rot = rot;}		// �p�x
	void SetSize(const XMFLOAT3 &size){m_size = size;}	// �T�C�Y
	void SetCol(int r, int g, int b, int a) { m_r = r; m_g = g; m_b = b; m_a = a; }

	// ���Z
	void AddPos(XMFLOAT3 pos) { m_pos = XM::F3::Add(m_pos, pos); }		// ���W
	void AddScl(XMFLOAT3 scl){m_scl = XM::F3::Add(m_pos, scl);}			// �g�嗦
	void AddRot(XMFLOAT3 rot){m_rot = XM::F3::Add(m_pos, rot);}			// �p�x
	void AddSize(XMFLOAT3 size){m_size = XM::F3::Add(m_pos, size);}		// �T�C�Y
	void AddCol(const int &r, const int &g, const int &b, const int &a) { m_r += r; m_g += g; m_b += b; m_a += a; }

	// �^�C�v
	void SetObjType(OBJTYPE type) { m_objType = type; }			// �������ɃI�u�W�F�N�g�̃^�C�v��ݒ�
	OBJTYPE GetObjType(void)const { return m_objType; }			// �I�u�W�F�N�g�^�C�v�̎擾
	void SetClassType(CLASSTYPE type) { m_classType = type; }	// �������ɃN���X�̃^�C�v��ݒ�
	CLASSTYPE GetClassType(void)const { return m_classType; }	// �N���X�^�C�v�̎擾

	// �`��D��x
	const int &GetPriority(void)const { return m_priority; }	// priority�̎擾

	CScene *CScene::GetSceneClassType(const CLASSTYPE &selectObjType);

	// scene
	static std::list<CScene*> *GetScene(int priority) {
		assert((priority > 0 || priority < MAX_SCENE_PRIORITY) && "PRIORITY�̎w�肪�Ԉ���Ă��܂�");
		return &m_lisScene[priority];			// �V�[���̐擪
	}
	
protected:
	void Release(void) { m_delete = true; }	// �����[�X

	XMFLOAT3 m_pos;			// ���W
	XMFLOAT3 m_scl;			// �g�嗦
	XMFLOAT3 m_rot;			// ��]�p
	XMFLOAT3 m_size;		// �T�C�Y
	int m_r, m_g, m_b, m_a;

	OBJTYPE m_objType;			// �I�u�W�F�N�g�̃^�C�v
	CLASSTYPE m_classType;		// �N���X�̃^�C�v
	int m_priority;				// �`��D��x

private:
	
	static std::list<CScene*> m_lisScene[MAX_SCENE_PRIORITY];
	static int m_numScene[MAX_SCENE_PRIORITY];		// �V�[���̑���
	bool m_delete;									// ����t���O
};

#endif

