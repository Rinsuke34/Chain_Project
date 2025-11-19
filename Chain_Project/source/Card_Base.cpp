/* カードのベースクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Base.h"

// コンストラクタ
Card_Base::Card_Base()
{
	/* 初期化 */
	// カード情報
	this->iRarity	= 0;		// レアリティ
	this->iCardType	= 0;		// カードの種類
	this->Name		= "";		// カード名
	// その他
	this->Now_Position		= { SCREEN_SIZE_WIDE, SCREEN_SIZE_HEIGHT / 2 };	// 現在座標(ドローしてる感を出すため山札の位置を初期値に設定)
	this->Setting_Position	= { 0, 0 };	// 設定座標(ホールドが解除された際に自動で補正される座標)
}

// 位置座標補間処理
void Card_Base::Position_Interpolation()
{
	/* X座標補間 */
	if (std::abs(this->Setting_Position.iX - this->Now_Position.iX) < INTERPOLATION_SPEED)
	{
		this->Now_Position.iX = this->Setting_Position.iX;
	}
	else
	{
		this->Now_Position.iX += (this->Setting_Position.iX - this->Now_Position.iX) / INTERPOLATION_SPEED;
	}

	/* Y座標補間 */
	if (std::abs(this->Setting_Position.iY - this->Now_Position.iY) < INTERPOLATION_SPEED)
	{
		this->Now_Position.iY = this->Setting_Position.iY;
	}
	else
	{
		this->Now_Position.iY += (this->Setting_Position.iY - this->Now_Position.iY) / INTERPOLATION_SPEED;
	}
}
