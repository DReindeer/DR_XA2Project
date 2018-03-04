//================================================================================
//
// Auter : KENSUKE WATANABE
// Data  : 2018/01/02
//
//================================================================================
#ifndef _XA2SOURCEVOICEMANAGER_H_
#define _XA2SOURCEVOICEMANAGER_H_

#include <unordered_map>
#include <thread>
#include <mutex>

class XA2SourceVoiceInterface;

// �\�[�X�{�C�X�}�l�[�W��
//--------------------------------------------------------------------------------
class XA2SourceVoiceManager
{
public:
	XA2SourceVoiceManager();
	~XA2SourceVoiceManager();

	void Update();			// �X�V
	void Uninit();			// �I��
	void StopAndUninit();	// ��~���ďI��

	// �ʃX���b�h�|�[�����O����
	static void Polling();	

	// �R���e�i
	//--------------------------------------------------------------------------------
	// �擾
	XA2SourceVoiceInterface *GetXA2SourceVoice(void *thisPointer);

	// �o�^
	void SetXA2SoundObject(XA2SourceVoiceInterface* pXA2SourceVoice, void* thisPointer) {
		if (!pXA2SourceVoice || !thisPointer) return;
		if (m_sourceVoices.count(thisPointer) != 0) return;
		m_sourceVoices[thisPointer] = pXA2SourceVoice; 
	}

private:
	// �\�[�X�{�C�X�C���^�[�t�F�C�X�̃R���e�i
	static std::unordered_map<void*, XA2SourceVoiceInterface*> m_sourceVoices;

	// Streaming�Đ��p�̃X���b�h
	static std::thread m_pollingThread;	// �X���b�h
	static bool m_endPollingThread;		// �X���b�h�̏I������
};

#endif
