/* ワールドマップノード"エネミー"クラスの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "WoldMap_Node_Enemy.h"
// 関連クラス
#include "DataList_Image.h"

// コンストラクタ
WoldMap_Node_Enemy::WoldMap_Node_Enemy() : WoldMap_Node_Base()
{
	/* 初期化 */
	this->NodeType = NODE_TYPE_ENEMY;	// ノードの種類
}
