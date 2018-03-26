//================================================================================
//
// Auter : KENSUKE WATANABE
//
//================================================================================
#ifndef _XA2OGGDECODER_H_
#define _XA2OGGDECODER_H_

#include "xa2Manager.h"

#include "vorbis/vorbisfile.h"

#pragma comment ( lib, "libogg_static.lib" )
#pragma comment ( lib, "libvorbis_static.lib" )
#pragma comment ( lib, "libvorbisfile_static.lib" )


// oggデコーダー
//--------------------------------------------------------------------------------
class XA2OggDecoder
{
public:
	XA2OggDecoder() {}
	~XA2OggDecoder() {}

	bool Open();

private:
	OggVorbis_File m_ovf;

};

#endif
