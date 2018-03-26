//================================================================================
//
// Auter : KENSUKE WATANABE
//
//================================================================================
#include "xa2OggDecoder.h"

//--------------------------------------------------------------------------------
// ogg�t�@�C���I�[�v��
//--------------------------------------------------------------------------------
bool XA2OggDecoder::Open()
{
	// �t�@�C���I�[�v��
	//--------------------------------------------------------------------------------
	int error = ov_fopen("Test.ogg", &m_ovf);

	if (error != 0) {
		switch (error) {
		case OV_EREAD:
		{
			MessageBox(nullptr, "XA2OggDecoder::Open() err::OV_EREAD\r\n���f�B�A����̓ǂݍ��݂��G���[��Ԃ��܂����B", "err", MB_OK);
			break;
		}
		case OV_ENOTVORBIS:
		{
			MessageBox(nullptr, "XA2OggDecoder::Open() err::OV_ENOTVORBIS\r\n�r�b�g�X�g���[����Vorbis�f�[�^���܂܂�Ă��܂���B", "err", MB_OK);
			break;
		}
		case OV_EVERSION:
		{
			MessageBox(nullptr, "XA2OggDecoder::Open() err::OV_EVERSION\r\nVorbis�̃o�[�W��������v���܂���B", "err", MB_OK);
			break;
		}
		case OV_EBADHEADER:
		{
			MessageBox(nullptr, "XA2OggDecoder::Open() err::OV_EBADHEADER\r\nVorbis�r�b�g�X�g���[���w�b�_�[�������ł��B", "err", MB_OK);
			break;
		}
		case OV_EFAULT:
		{
			MessageBox(nullptr, "XA2OggDecoder::Open() err::OV_EFAULT\r\n�������W�b�N�t�H���g�B�o�O�܂��̓q�[�v/�X�^�b�N�̔j���������܂��B", "err", MB_OK);
			break;
		}
		default:
			break;
		}
		return false; // �G���[
	}
	return true;
}