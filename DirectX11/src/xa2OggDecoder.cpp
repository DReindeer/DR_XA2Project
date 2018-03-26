//================================================================================
//
// Auter : KENSUKE WATANABE
//
//================================================================================
#include "xa2OggDecoder.h"

//--------------------------------------------------------------------------------
// oggファイルオープン
//--------------------------------------------------------------------------------
bool XA2OggDecoder::Open()
{
	// ファイルオープン
	//--------------------------------------------------------------------------------
	int error = ov_fopen("Test.ogg", &m_ovf);

	if (error != 0) {
		switch (error) {
		case OV_EREAD:
		{
			MessageBox(nullptr, "XA2OggDecoder::Open() err::OV_EREAD\r\nメディアからの読み込みがエラーを返しました。", "err", MB_OK);
			break;
		}
		case OV_ENOTVORBIS:
		{
			MessageBox(nullptr, "XA2OggDecoder::Open() err::OV_ENOTVORBIS\r\nビットストリームにVorbisデータが含まれていません。", "err", MB_OK);
			break;
		}
		case OV_EVERSION:
		{
			MessageBox(nullptr, "XA2OggDecoder::Open() err::OV_EVERSION\r\nVorbisのバージョンが一致しません。", "err", MB_OK);
			break;
		}
		case OV_EBADHEADER:
		{
			MessageBox(nullptr, "XA2OggDecoder::Open() err::OV_EBADHEADER\r\nVorbisビットストリームヘッダーが無効です。", "err", MB_OK);
			break;
		}
		case OV_EFAULT:
		{
			MessageBox(nullptr, "XA2OggDecoder::Open() err::OV_EFAULT\r\n内部ロジックフォルト。バグまたはヒープ/スタックの破損を示します。", "err", MB_OK);
			break;
		}
		default:
			break;
		}
		return false; // エラー
	}
	return true;
}