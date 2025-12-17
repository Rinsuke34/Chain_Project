/* ワールドマップノード"ショップ"クラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 親クラス
#include "WoldMap_Node_Base.h"

// ワールドマップノード"ショップ"クラス
class WoldMap_Node_Shop : public WoldMap_Node_Base
{
	public:
		WoldMap_Node_Shop();				// コンストラクタ
		virtual ~WoldMap_Node_Shop() {};	// デストラクタ
};
