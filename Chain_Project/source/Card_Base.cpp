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
	this->Now_Position		= {0, 0};	// 現在座標
	this->Setting_Position	= {0, 0};	// 設定座標(ホールドが解除された際に自動で補正される座標)
}

// 描画
void Card_Base::Draw()
{
	/* 背景描写 */
	DrawBox(Now_Position.iX - (CARD_WIDTH / 2), Now_Position.iY - (CARD_HEIGHT / 2), Now_Position.iX + (CARD_WIDTH / 2), Now_Position.iY + (CARD_HEIGHT / 2), GetColor(0, 0, 0), TRUE);
}
