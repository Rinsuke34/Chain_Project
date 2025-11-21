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

