/* "打刀"カードの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Card_Arms_Base.h"

// "打刀"カードクラス
class Card_Arms_Katana : public Card_Arms_Base
{
	public:
		Card_Arms_Katana();				// コンストラクタ
		virtual ~Card_Arms_Katana() {};	// デストラクタ

		virtual void	Effect_BattleStart()	override;	// 効果発動:戦闘開始時
		virtual void	Effect_StartTurn()		override;	// 効果発動:ターン開始時

	private:
		bool LiveFlg[3];	// 生存フラグ
		int Kill_Count;		// 累計キル数
};
