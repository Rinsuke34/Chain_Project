/* "旅人の弓"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "旅人の弓"カードクラス
class Card_Arms_TravelerBow : public Card_Arms_Base
{
	public:
		Card_Arms_TravelerBow();				// コンストラクタ
		virtual ~Card_Arms_TravelerBow() {};	// デストラクタ

		/* 関数 */
		virtual void Effect_Action_Before(std::shared_ptr<Character_Base> Target) override;		// 効果発動:行動直前

	private:
		/* 変数 */
		int PermanenceBuff_Atk;		// 攻撃力の永続バフ
};
