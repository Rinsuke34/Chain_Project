/* "道具"カードのベースクラスの定義 */

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Item_Base.h"

// コンストラクタ
Card_Item_Base::Card_Item_Base() : Card_Base()
{
	/* カード情報の設定 */
	SetCardType(Card_Base::TYPE_ITEM);	// カード種類:道具
}

// デストラクタ
Card_Item_Base::~Card_Item_Base()
{

}

// 描画
void Card_Item_Base::Draw()
{
	/* 背景描写 */
	DrawBox(Now_Position.iX - (CARD_WIDTH / 2), Now_Position.iY - (CARD_HEIGHT / 2), Now_Position.iX + (CARD_WIDTH / 2), Now_Position.iY + (CARD_HEIGHT / 2), GetColor(0, 0, 255), TRUE);
}
