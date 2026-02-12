/* キャラクター(スライム(巨大/緑))の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Character_Base.h"

// 前方宣言
class DataList_Battle;

// キャラクター(スライム(巨大/緑))のクラス
class Character_BigSlime_Green : public Character_Base
{
	public:
		Character_BigSlime_Green();				// コンストラクタ
		virtual ~Character_BigSlime_Green() {};	// デストラクタ

		/* 関数 */
		virtual void Action()		override;	// 行動
		virtual void Action_Extra()	override;	// 特殊行動(エネミーの特殊行動はこの関数を継承して行う)

	private:
		int ActionCount;	// 行動カウント(行動内容の決定に使用)
};
