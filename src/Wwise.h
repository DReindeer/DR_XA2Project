// ======== ======== ======== ======== ======== ======== ======== ========
//
//			[Wwise.h] 							Auter : KENSUKE WATANABE
//												Data  : 2017/09/06
//
// -------- -------- -------- -------- -------- -------- -------- --------
//��Update : 2018/02/06
// 
// �� CheckEndEvent() - �C�x���g���I�����Ă��邩�ǂ������� : 2018/02/06
// 
// ======== ======== ======== ======== ======== ======== ======== ========
//
// �g�����I
//
// �Z Init(), Uninit(), Update() ��Manager.cpp�ɏ����ĉ�����
// �Z ���X�i�[(m_pMainListener)�́A�v���C���[���̈ʒu������ɍ��킹��
//    ���t���[���ݒ肵�����ĉ������B(Wwise�N���X���Ƀ��C�����X�i�[�p�̊֐�����)
//
// �@ CreateNoListenerGameObj() - �Q�[���I�u�W�F�N�g���쐬�I
// �A SetEvent() - �C�x���g���Đ��I
// �B Stop() - �v���O�������ŃC�x���g�̒�~�I(����̎g�p�̓~���[�W�b�N�̐l�Ɨv���k)
// 
// -------- -------- -------- -------- -------- -------- -------- --------
#ifndef _WWISE_H_
#define _WWISE_H_

#include <windows.h>
#include <unordered_map>
#include <assert.h>

// Release���ɒ�`������
#ifndef _DEBUG
#define AK_OPTIMIZED
#endif

// Wwise(�T�E���h�G���W��)���g�p���邽�߂ɕK�v
#include <AK/SoundEngine/Common/AkSoundEngine.h>		// �T�E���h�G���W��
#include <AK/IBytes.h>

#include <AK/SoundEngine/Platforms/Windows/AkTypes.h>	// �^�C�v

#include "Wwise_IDs.h"									// �C�x���g��ID

using namespace AK;
using namespace AK::SoundEngine;

// ======== ======== ======== ======== ======== ======== ======== ========
// �\����
// -------- -------- -------- -------- -------- -------- -------- --------
// �Q�[���I�u�W�F�N�g
struct WWISE_GAMEOBJ
{
	D3DXVECTOR3 pos, rot;		// ���W, ��]�p			- ���w�I��0�x����̉�]�p
	D3DXVECTOR3 vel;			// ���x					- �h�b�v���[���ʂŕK�v�H
};

// �I�������C�x���g�̏����i�[ - �R�[���o�b�N�֐��Ŏg�p
struct WWISE_ENDEVENT_GAMEOBJ
{
	AkGameObjectID gameObjID;	// �Q�[���I�u�W�F�N�gID
	AkUniqueID eventID;			// �C�x���gID
};

// �O���錾
class CAkFilePackageLowLevelIOBlocking;

// ======== ======== ======== ======== ======== ======== ======== ========
// �N���X
// -------- -------- -------- -------- -------- -------- -------- --------
class Wwise
{
public:
	Wwise(){}
	~Wwise(){}

	void Init(HWND hWnd);	// ������
	void Uninit();			// �I��
	void Update();			// �X�V

	// �Q�[���I�u�W�F�N�g�̐���
	// -------- -------- -------- -------- -------- -------- -------- --------
	WWISE_GAMEOBJ *CreateNoListenerGameObj(D3DXVECTOR3 pos, D3DXVECTOR3 rot, void *thisPtr, char *gameObjName = nullptr);	// Listener�����I�u�W�F�N�g
	WWISE_GAMEOBJ *CreateMainListenerGameObj(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *gameObjName = "Listener (Default)");	// ���C�����X�i�[
	void DeleteGameObj(void *thisPtr);


	// �C�x���g���M
	// -------- -------- -------- -------- -------- -------- -------- --------
	void SetEvent(int eventID, void *thisPtr);		// �w��Q�[���I�u�W�F�N�g��ID�ŃC�x���g�𑗐M

	// �C�x���g�����ɏI�����Ă��邩���ׂ�
	// -------- -------- -------- -------- -------- -------- -------- --------
	bool CheckEndEvent(int eventID, void *thisPtr = nullptr);

	// �Q�[���I�u�W�F�N�g�̍Đ����~
	// -------- -------- -------- -------- -------- -------- -------- --------
	void Stop(void *thisPtr);


	// �C�x���g�̏�Ԃ�ύX
	// -------- -------- -------- -------- -------- -------- -------- --------
	void SetState(int stateGroup, int stateID);			// �X�e�[�g�̐ݒ�
	void SetGameParam(int rtpcID, float value);			// �Q�[���p�����[�^�[(RTPC)�̐ݒ�


	// �}�e���A��
	// -------- -------- -------- -------- -------- -------- -------- --------
	int GetMaterialId(char *name);						// ���O����}�e���A��ID�̎擾
//	void SetMaterial(void *gameObject, int materialid);	// �Q�[���I�u�W�F�N�g�Ƀ}�e���A��ID��ݒ�


	// ���C�����X�i�[�n
	// -------- -------- -------- -------- -------- -------- -------- --------
	void SetMainListenerEvent(int eventID);		// ���C�����X�i�[(�v���C���[)�ɃC�x���g�𑗐M
	void StopMainListener();
	void SetPos(const D3DXVECTOR3 &pos) { m_pMainListener->pos = pos; }		// ���W
	void SetRot(const D3DXVECTOR3 &rot) { m_pMainListener->rot = rot; }		// ��]
	void SetVel(const D3DXVECTOR3 &vel) { m_pMainListener->vel = vel; }		// ���x


	// �e�Q�[���I�u�W�F�N�g�̈ʒu�A���W�̐ݒ�
	// -------- -------- -------- -------- -------- -------- -------- --------
	void SetPos(void *thisPtr, const D3DXVECTOR3 &pos) {			// ���W
		if (m_umapGameObj.count(thisPtr) < 1) { assert(!"�����̖��O���s��"); }
		m_umapGameObj[thisPtr]->pos = pos;
	}
	void SetRot(void *thisPtr, const D3DXVECTOR3 &rot) {			// ��]
		if (m_umapGameObj.count(thisPtr) < 1) { assert(!"�����̖��O���s��"); }
		m_umapGameObj[thisPtr]->rot = rot;
	}
	void SetVel(void *thisPtr, const D3DXVECTOR3 &vel) {			// ���x
		if (m_umapGameObj.count(thisPtr) < 1) { assert(!"�����̖��O���s��"); }
		m_umapGameObj[thisPtr]->vel = vel;
	}

//	void SetMultiplePositions(void *thisPtr, WWISE_GAMEOBJ *pArrayGameObj) {
//		AkChannelEmitter *AkArrayPos;
//		// ������x�N�g��
//		AkVector top;
//		float fRadPitch = pArrayGameObj[0].rot.z;
//		float fCosPitch = cos(fRadPitch);
//		float fSinPitch = sin(fRadPitch);
//		float fRadYaw = pArrayGameObj[0].rot.y - D3DX_PI * 0.5f;
//		float fCosYaw = cos(fRadYaw);
//		float fSinYaw = sin(fRadYaw);
//		top.X = -fCosYaw * fSinPitch;
//		top.Y = fCosPitch;
//		top.Z = -fSinYaw * fSinPitch;
//		AkArrayPos->position.Set(VecToAkVector(pArrayGameObj[0].pos), VecToAkVector(pArrayGameObj[0].rot), top);
//		AK::SoundEngine::SetMultiplePositions((AkGameObjectID)thisPtr, AkArrayPos, 
//	}

	// �o�b�N�O���E���h/�t�H�A�O���E���h���̏���
	// false ���ׂẴI�[�f�B�I�����_�����O���~����
	// true  �ꕔ���~���� (�C�x���g�̏����͌p�����邪�A�I�[�f�B�I�������_�����O�����Ȃ�)
	// -------- -------- -------- -------- -------- -------- -------- --------
	void Suspend(bool suspend) { SoundEngine::Suspend(suspend); }	// �o�b�N�O���E���h/�t�H�A�O���E���h���̏���
	void WakeupFromSuspend() { SoundEngine::WakeupFromSuspend(); }	// Suspend�Œ�~���������̍ĊJ
	

	// D3DXVECTOR3��AkVector�̕ϊ�
	// -------- -------- -------- -------- -------- -------- -------- --------
	static void VecToAkVector(const D3DXVECTOR3 &vec, AkVector &vector);
	static AkVector VecToAkVector(const D3DXVECTOR3 &vec);


	// �L���v�`�����X�g�փG���[���b�Z�[�W�𑗂�
	// -------- -------- -------- -------- -------- -------- -------- --------
	void PostMessage(char *szMessage);

private:
	// �Q�[���I�u�W�F�N�g�̓o�^(���ڂ͌Ăяo���Ȃ�)
	// -------- -------- -------- -------- -------- -------- -------- --------
	void registPtr(void *thisPtr, char *pName = nullptr);
	void registPtr(const AkGameObjectID &ID, char *pName = nullptr);
	void unregistPtr(void *thisPtr);

	// �჌�x��I/O
	// -------- -------- -------- -------- -------- -------- -------- --------
	static CAkFilePackageLowLevelIOBlocking m_lowLevelIO;			

	// �Q�[���I�u�W�F�N�g
	// -------- -------- -------- -------- -------- -------- -------- --------
	std::unordered_map<void*, WWISE_GAMEOBJ*> m_umapGameObj;		// �Q�[���I�u�W�F�N�g
	WWISE_GAMEOBJ *m_pMainListener;									// ���C�����X�i�[�ւ̃|�C���^
	AkGameObjectID m_mainListenerID;								// ���C�����X�i�[��ID

	// �_�~�[�R�[���o�b�N
	// -------- -------- -------- -------- -------- -------- -------- --------
	static void dummyCallback(AkCallbackType type, AkCallbackInfo* pCallbackInfo);
	static std::list<int> dummies;
	static CAkLock dummiesLock;

	// �Q�[���I�u�W�F�N�g�̃C�x���g�̏I�����m�F���鏈��
	// -------- -------- -------- -------- -------- -------- -------- --------
	static void EndEventCallback(AkCallbackType type, AkCallbackInfo* pCallbackInfo);
	static std::list<WWISE_ENDEVENT_GAMEOBJ *> m_pEndEvents;
};

#endif

/*
// ���O��(bit���)�R�[���o�b�N�t���O�𗧂Ă鎞�Ɏg�p�ł���t���O�ꗗ - AkCallback.h��27�s�Œ�`����Ă��܂��B
AK_EndOfEvent						�R�[���o�b�N�́A�C�x���g�̏I�����Ƀg���K����܂��B
AK_EndOfDynamicSequenceItem			���I�V�[�P���X�A�C�e���̍Ō�ɓ��B�����Ƃ��ɌĂяo�����R�[���o�b�N�B
AK_Marker							�Đ����Ƀ}�[�J�[�ɑ��������Ƃ��ɌĂяo�����R�[���o�b�N�B
AK_Duration							�����̎������Ԃ��T�E���h�G���W���ɂ���ĔF�����ꂽ�Ƃ��ɃR�[���o�b�N����܂��B
AK_SpeakerVolumeMatrix				�e�t���[���Ńg���K���ꂽ�R�[���o�b�N�B�N���C�A���g���X�s�[�J�̃{�����[���}�g���b�N�X��ύX�ł���悤�ɂ��܂��B
AK_Starvation 						�Đ����X�g���[���̌͊��̂��߂Ƀt���[�����X�L�b�v����Ƃ��ɔ�������R�[���o�b�N�B
AK_MusicPlaylistSelect				���y�v���C���X�g�R���e�i���Đ����鎟�̃A�C�e����I�����Ȃ���΂Ȃ�Ȃ��Ƃ��ɃR�[���o�b�N����܂��B
AK_MusicPlayStarted					�u�Đ��v�܂��́u�V�[�N�v�R�}���h�����s���ꂽ�Ƃ��ɌĂяo�����R�[���o�b�N�i�u�V�[�N�v�R�}���h�́AAK :: SoundEngine :: SeekOnEvent�i�j���甭�s����܂��j�B�C���^���N�e�B�u�~���[�W�b�N�K�w�̃I�u�W�F�N�g�ɂ̂ݓK�p����܂��B
AK_MusicSyncBeat					�~���[�W�b�N�r�[�g�Œʒm��L���ɂ���B
AK_MusicSyncBar						�~���[�W�b�N�o�[�Œʒm��L���ɂ���B
AK_MusicSyncEntry					Music Entry Cue�Œʒm��L���ɂ���B
AK_MusicSyncExit					�~���[�W�b�NExit Cue�Œʒm��L���ɂ���B
AK_MusicSyncGrid					�~���[�W�b�N�O���b�h�Œʒm��L���ɂ���B
AK_MusicSyncUserCue					�~���[�W�b�N�J�X�^���L���[�̒ʒm��L���ɂ���B
AK_MusicSyncPoint					�~���[�W�b�N�X�C�b�`�̈ڍs�����|�C���g�Œʒm��L���ɂ���B
AK_MusicSyncAll						AK_MusicSync�o�^�Ɋւ��邷�ׂĂ̒ʒm����M����ꍇ�́A���̃t���O���g�p���܂��B
AK_MIDIEvent						MIDI�C�x���g�̒ʒm��L���ɂ���B
AK_CallbackBits						���ׂẴR�[���o�b�N�^�̃r�b�g�}�X�N�B
AK_EnableGetSourcePlayPosition		AK :: SoundEngine :: GetSourcePlayPosition�i�j�Ŏg�p����Đ��ʒu����L���ɂ��܂��B
AK_EnableGetMusicPlayPosition		AK :: MusicEngine :: GetPlayingSegmentInfo�i�j����ďƉ�ꂽ���y�I�u�W�F�N�g�̍Đ��ʒu����L���ɂ��܂��B
AK_EnableGetSourceStreamBuffering	AK :: SoundEngine :: GetSourceStreamBuffering�i�j�Ŏg�p���邽�߂̃X�g���[���o�b�t�@�����O����L���ɂ��܂��B

*/