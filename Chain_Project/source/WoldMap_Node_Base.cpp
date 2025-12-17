/* ワールドマップノードのベースクラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "WoldMap_Node_Base.h"

// コンストラクタ
WoldMap_Node_Base::WoldMap_Node_Base()
{
	/* 初期化 */
	this->NodeType		= "";			// ノードの種類
	this->Position_Map	= {0, 0};		// マップ上での座標
	this->Image			= -1;			// 画像
}
