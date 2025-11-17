/* カードのベースクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Base.h"

// コンストラクタ
Card_Base::Card_Base()
{
	/* 初期化 */
	this->iRarity	= 0;		// レアリティ
	this->iCardType	= 0;		// カードの種類
	this->Name		= "";		// カード名
	this->Pos_Draw	= { 0, 0 };	// 描画位置
}

// 描画
void Card_Base::Draw()
{
	/* 背景描写 */
	DrawBox(0, 0, 120, 170, GetColor(0, 0, 0), TRUE);
}
