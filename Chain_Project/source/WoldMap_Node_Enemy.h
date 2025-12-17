/* ワールドマップノード"エネミー"クラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "WoldMap_Node_Base.h"

// ワールドマップノード"エネミー"クラス
class WoldMap_Node_Enemy : public WoldMap_Node_Base
{
	public:
		WoldMap_Node_Enemy();				// コンストラクタ
		virtual ~WoldMap_Node_Enemy() {};	// デストラクタ
};
