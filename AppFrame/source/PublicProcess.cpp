/* 汎用的に使用できる計算式の定義 */

/* 使用する要素のインクルード */
// 標準ライブラリ
#include <algorithm>
// 共通定義
#include "AppFunctionDefine.h"

// 値を範囲内に収める
float PUBLIC_PROCESS::fClamp(float fValue, float fMax, float fMin)
{
	// 引数
	// fValue	<- 範囲内に収める値
	// fMax		<- 最大値
	// fMin		<- 最小値
	// 戻り値
	// float	<- 範囲内に収めた値

	float fReturn = fValue;

	/* 最大値より大きいか確認 */
	if (fValue > fMax)
	{
		// 最大値より大きいなら
		/* 最大値に設定する */
		fReturn = fMax;
	}

	/* 最小値より小さいか確認 */
	if (fValue < fMin)
	{
		// 最小値より小さいなら
		/* 最小値に設定する */
		fReturn = fMin;
	}

	return fReturn;
}

// 閾値を超えているか確認
bool PUBLIC_PROCESS::bCheckAboveThreshold(unsigned char ucValue, unsigned char ucThreshold)
{
	// 引数
	// ucValue		<- 確認する値
	// ucThreshold	<- 閾値
	// 戻り値
	// bool			<- 閾値を超えているか(true:超えている、false:超えていない)

	bool bReturn = false;

	/* 閾値を超えているか確認 */
	if (ucValue > ucThreshold)
	{
		// 閾値を超えているなら
		/* trueを返す */
		bReturn = true;
	}

	return bReturn;
}

// アナログスティックの入力を正規化(±1)
float PUBLIC_PROCESS::fAnalogStickNorm(short sMouseMove)
{
	// 引数
	// sMouseMove	<- 正規化したいアナログスティックの入力
	// 戻り値
	// float		<- 正規化した値(±1の範囲)


	/* 入力値を±1の範囲へ正規化 */
	return (float)sMouseMove / 32767.f;
}

// 入力値がデッドゾーンの範囲内であるか確認
bool PUBLIC_PROCESS::bCheckInputDeadzone(float fInput, float fDeadzone)
{
	// 引数
	// fInput		<- 確認する値
	// fDeadzone	<- デッドゾーン
	// 戻り値
	// bool			<- デッドゾーンの範囲内であるか(true:範囲内である、false:範囲内ではない)

	bool bReturn = false;

	/* デッドゾーンの範囲内であるか確認 */
	if ((-fDeadzone < fInput) && (fInput < +fDeadzone))
	{
		// デッドゾーンの範囲内であるならtrue
		bReturn = true;
	}

	return bReturn;

}

// 2つの線分の交点を取得
VECTOR PUBLIC_PROCESS::vecGetLineCrossPoint(VECTOR vecLineAStart, VECTOR vecLineADirection, VECTOR vecLineBStart, VECTOR vecLineBDirection)
{
	// 引数
	// vecLineAStart		<- 線分Aの始点
	// vecLineADirection	<- 線分Aの方向
	// vecLineBStart		<- 線分Bの始点
	// vecLineBDirection	<- 線分Bの方向
	// 戻り値
	// VECTOR				<- 線分ABの交点(接触していない場合は線分Aの始点)

	VECTOR vecReturn;

	/* 直線が平行であるか確認 */
	VECTOR vecGetLineCrossPoint	= VCross(vecLineADirection, vecLineBDirection);		// 外積
	float fCrossSize = VSize(vecGetLineCrossPoint);									// 外積の大きさ

	/* 直線が平行でないか確認 */
	if (fCrossSize == 0.f)
	{
		// 平行である場合
		/* 交点を取得できないため、始点を返す */
		vecReturn = vecLineAStart;
		return vecReturn;
	}

	// パラメータ t1 を計算
	VECTOR diff = VSub(vecLineBStart, vecLineAStart);

	// パラメータ t1 の内積を計算
	float t1 = VDot(diff, VCross(vecLineBDirection, vecGetLineCrossPoint)) / (fCrossSize * fCrossSize);

	// 直線1上の交点を計算
	return VAdd(vecLineAStart, VScale(vecLineADirection, t1));
}

// 2つの色(COLOR_F型)を合成
COLOR_F	PUBLIC_PROCESS::stAddCollorF(COLOR_F vecColorF_A, COLOR_F vecColorF_B)
{
	// 引数
	// vecColorF_A		<- 1つ目の色
	// vecColorF_B		<- 2つ目の色
	// 戻り値
	// COLOR_F			<- 合成後の色

	COLOR_F stReturn;

	/* 各要素を加算 */
	stReturn.a	=	vecColorF_A.a + vecColorF_B.a;
	stReturn.b	=	vecColorF_A.b + vecColorF_B.b;
	stReturn.g	=	vecColorF_A.g + vecColorF_B.g;
	stReturn.r	=	vecColorF_A.r + vecColorF_B.r;

	/* 各要素が最大値(1.f)を超えないように調整 */
	if (stReturn.a > 1.f)	{ stReturn.a = 1.f; }
	if (stReturn.b > 1.f)	{ stReturn.b = 1.f; }
	if (stReturn.g > 1.f)	{ stReturn.g = 1.f; }
	if (stReturn.r > 1.f)	{ stReturn.r = 1.f; }

	return stReturn;
}

// UTF-8 → Shift-JIS 変換
std::string	PUBLIC_PROCESS::aUtf8ToShiftJIS(std::string Utf8)
{
	// 引数
	// Utf8			<- Shift-Jisに変換予定のUTF8文字列
	// 戻り値
	// std::string	<- Shift-Jis文字列

	/* UTF-8 → UTF-16(ワイド文字) */
	int	iWideSize = MultiByteToWideChar(CP_UTF8, 0, Utf8.c_str(), -1, NULL, 0);

	if (iWideSize == 0) return "";

	std::wstring wideStr(iWideSize, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, Utf8.c_str(), -1, &wideStr[0], iWideSize);

	/* UTF-16 → Shift-JIS */
	int iSjisSize = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
	if (iSjisSize == 0) return "";

	std::string aSjisStr(iSjisSize, '\0');
	WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &aSjisStr[0], iSjisSize, NULL, NULL);

	return aSjisStr;
}

// Shift-JIS → UTF-8 変換
std::string	PUBLIC_PROCESS::aShiftJisToUtf8(std::string ShiftJis)
{
	// 引数
	// ShiftJis		<-	UTF-8に変換予定のShift-Jis文字列
	// 戻り値
	// std::string	<-	UTF-8文字列

	// 入力が空または空白のみの場合、半角スペースを返す
	if (ShiftJis.empty()) {
		return "";
	}

	// Shift-JIS から UTF-16 に変換
	int wideSize = MultiByteToWideChar(CP_ACP, 0, ShiftJis.c_str(), -1, NULL, 0);
	if (wideSize == 0) return "";

	std::wstring wideStr(wideSize, 0);
	MultiByteToWideChar(CP_ACP, 0, ShiftJis.c_str(), -1, &wideStr[0], wideSize);

	// UTF-16 から UTF-8 に変換
	int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8Size == 0) return "";

	std::string utf8Str(utf8Size - 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Size, NULL, NULL);

	return utf8Str;
}

// Shift-JIS文字列を指定文字数でカット
std::string	PUBLIC_PROCESS::aCutShitfJisString(std::string ShiftJis, int iLength)
{
	// 引数
	// ShiftJis		<- カットするShift-JIS文字列
	// iLength		<- カットする文字数
	// 戻り値
	// std::string	<- カット後のShift-JIS文字列

	int iCount = 0;
	int iPossition = 0;

	while (iPossition < ShiftJis.size() && iCount < iLength)
	{
		/* Shift-JIS の先頭バイトを取得 */
		unsigned char ch = static_cast<unsigned char>(ShiftJis[iPossition]);

		/* 文字のバイト数を取得 */
		if (ch <= 0x7F || (ch >= 0xA1 && ch <= 0xDF))
		{
			// 半角文字(1バイト)である場合
			iPossition += 1;
		}
		else
		{
			// 全角文字(2バイト)である場合
			/* 文字が最後まで記述されているか確認 */
			if (iPossition + 1 < ShiftJis.size())
			{
				// 文字が最後まで記述されている場合
				iPossition += 2;
			}
			else
			{
				// 文字が途中で途切れている場合
				break;
			}
		}

		/* カウントを加算する */
		iCount++;
	}

	return ShiftJis.substr(0, iPossition);
}

// Shift-JIS文字列を指定文字数で改行
std::string PUBLIC_PROCESS::aInsertNewLine(std::string ShiftJis, int iLength)
{
	// 引数
	// ShiftJis		<- 改行するShift-JIS文字列
	// iLength		<- 改行する文字数
	// 戻り値
	// std::string	<- 改行後の文字列

	int iPossition = 0;  // 現在の位置
	int iCount = 0;      // 文字数カウント

	while (iPossition < ShiftJis.size())
	{
		/* 改行があったらカウントをリセット */
		if (ShiftJis[iPossition] == '\n')
		{
			iCount = 0;
			iPossition += 1;
			continue;
		}

		/* Shift-JIS の先頭バイトを取得 */
		unsigned char ch = static_cast<unsigned char>(ShiftJis[iPossition]);

		/* 文字のバイト数を取得 */
		if (ch <= 0x7F || (ch >= 0xA1 && ch <= 0xDF))
		{
			// 半角文字 (1バイト)
			iPossition += 1;
		}
		else
		{
			// 全角文字 (2バイト)
			if (iPossition + 1 < ShiftJis.size())
			{
				iPossition += 2;
			}
			else
			{
				break; // 文字が途中で途切れている場合
			}
		}

		iCount++;

		// 指定文字数に達したら改行を挿入
		if (iCount >= iLength)
		{
			ShiftJis.insert(iPossition, "\n");
			iPossition += 1; // 改行の分だけ位置をずらす
			iCount = 0;       // カウントをリセット
		}
	}

	return ShiftJis;
}

// 2つのfloat型の値の差が閾値以内であるか確認
bool PUBLIC_PROCESS::bIsFloatDiffWithinThreshold(float fA, float fB, float fThreshold)
{
	return fabsf(fA - fB) <= fThreshold;
}

// AABB同士の当たり判定
bool PUBLIC_PROCESS::bBoxHitCheck(Struct_Collision::COLLISION_BOX BoxA, Struct_Collision::COLLISION_BOX BoxB)
{
	// 引数
	// BoxA		<- 判定するボックスA
	// BoxB		<- 判定するボックスB
	// 戻り値
	// bool		<- 接触している(true) / 接触していない(false)

	// 自身のボックスの最小・最大座標を取得
	float afBox_Min_This[3] =
	{
		BoxA.vecBoxCenter.x - BoxA.vecBoxHalfSize.x,
		BoxA.vecBoxCenter.y - BoxA.vecBoxHalfSize.y,
		BoxA.vecBoxCenter.z - BoxA.vecBoxHalfSize.z
	};

	float afBox_Max_This[3] =
	{
		BoxA.vecBoxCenter.x + BoxA.vecBoxHalfSize.x,
		BoxA.vecBoxCenter.y + BoxA.vecBoxHalfSize.y,
		BoxA.vecBoxCenter.z + BoxA.vecBoxHalfSize.z
	};

	// 判定するボックスの最小・最大を取得
	float afBox_Min[3] =
	{
		BoxB.vecBoxCenter.x - BoxB.vecBoxHalfSize.x,
		BoxB.vecBoxCenter.y - BoxB.vecBoxHalfSize.y,
		BoxB.vecBoxCenter.z - BoxB.vecBoxHalfSize.z
	};

	float afBox_Max[3] =
	{
		BoxB.vecBoxCenter.x + BoxB.vecBoxHalfSize.x,
		BoxB.vecBoxCenter.y + BoxB.vecBoxHalfSize.y,
		BoxB.vecBoxCenter.z + BoxB.vecBoxHalfSize.z
	};

	// 各軸で重なっている個所がないならfalseを返す
	for (int i = 0; i < 3; ++i)
	{
		if (afBox_Max_This[i] < afBox_Min[i] || afBox_Min_This[i] > afBox_Max[i])
		{
			return false;
		}
	}

	// すべての軸で重なりがあればtrue
	return true;
}

// 指定の座標が指定範囲内に存在するか確認
bool PUBLIC_PROCESS::bPositionIn2DRangeCheck(Struct_2D::POSITION Position, Struct_2D::RANGE Range)
{
	// 引数
	// Position	<- 確認する座標(2D)
	// Range	<- 範囲(2D)
	// 戻り値
	// bool		<- 指定範囲内に存在するか(true:存在する、false:存在しない)

	// X座標が範囲内に存在するか確認
	if (Range.iTopX <= Position.iX && Position.iX <= Range.iBottomX)
	{
		// Y座標が範囲内に存在するか確認
		if (Range.iTopY <= Position.iY && Position.iY <= Range.iBottomY)
		{
			// 指定範囲内に存在するならtrueを返す
			return true;
		}
	}

	// 指定範囲内に存在しないならfalseを返す
	return false;
}
