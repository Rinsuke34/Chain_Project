/* キャラクター(テストNPC)の宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "Character_Base.h"

// 前方宣言
class DataList_Battle;

// キャラクター(テストNPC)のクラス
class Character_Npc_Test : public Character_Base
{
	public:
		Character_Npc_Test();				// コンストラクタ
		virtual ~Character_Npc_Test() {};	// デストラクタ

		/* 関数 */
		virtual void Action() override;		// 行動

	protected:
		/* 変数 */
		// データリスト
		std::shared_ptr<DataList_Battle> pDataList_Battle;		// バトル用データリスト
};
