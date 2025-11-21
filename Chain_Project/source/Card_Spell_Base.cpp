/* "魔法"カードのベースクラスの定義 */

#pragma once

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Spell_Base.h"

// コンストラクタ
Card_Spell_Base::Card_Spell_Base() : Card_Base()
{
	/* カード情報の設定 */
	SetCardType(Card_Base::TYPE_SPELL);	// カード種類:魔法
}

// デストラクタ
Card_Spell_Base::~Card_Spell_Base()
{

}
