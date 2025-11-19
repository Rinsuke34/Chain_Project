/* "武具"カードのベースクラスの定義 */

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Arms_Base.h"

// コンストラクタ
Card_Arms_Base::Card_Arms_Base() : Card_Base()
{
	/* カード情報の設定 */
	SetCardType(Card_Base::TYPE_ARMS);	// カード種類:武具
}

// デストラクタ
Card_Arms_Base::~Card_Arms_Base()
{

}

// 描画
void Card_Arms_Base::Draw()
{
	/* 背景描写 */
	DrawBox(Now_Position.iX - (CARD_WIDTH / 2), Now_Position.iY - (CARD_HEIGHT / 2), Now_Position.iX + (CARD_WIDTH / 2), Now_Position.iY + (CARD_HEIGHT / 2), GetColor(255, 0, 0), TRUE);
}
