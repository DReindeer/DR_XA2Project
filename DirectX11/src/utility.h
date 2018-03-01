// ======== ======== ======== ======== ======== ======== ======== ========
//
//
//	title : 便利関数群
//	Auter : KENSUKE WATANABE
//
//
// ======== ======== ======== ======== ======== ======== ======== ========
#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "main.h"
#include <string>

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }
// 
namespace Utility
{
	class Cast
	{
	public:
		static wchar_t *CastWChae_t(const char *orig);
	};

	class Character
	{
	public:
		// 指定文字列まで読み飛ばす
		// -------- -------- -------- -------- -------- -------- -------- --------
		static bool CutStrToStr(std::string *strData, int *current, const std::string &strString);

		// 指定トークンまでの文字列の内、切り取りトークンを無視した文字列を取得する
		// -------- -------- -------- -------- -------- -------- -------- --------
		static bool GetStrToToken(std::string *strData, int *current,
			const std::string &strToken, const std::string &strCutToken, std::string *strBuf);

		// 指定文字列までの文字列の内、切り取りトークンを無視した文字列を取得する
		// -------- -------- -------- -------- -------- -------- -------- --------
		static bool GetStrToStr(std::string *strData, int *current,
			const std::string &strString, const std::string &strCutToken, std::string *strBuf);
	};
}

#endif