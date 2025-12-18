/* ワールドマップノード"ボス"クラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "WoldMap_Node_Base.h"

// ワールドマップノード"ボス"クラス
class WoldMap_Node_Boss : public WoldMap_Node_Base
{
	public:
		WoldMap_Node_Boss();				// コンストラクタ
		virtual ~WoldMap_Node_Boss() {};	// デストラクタ
};
