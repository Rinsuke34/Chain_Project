/* ワールドマップノードのベースクラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// ワールドマップノードのベースクラス
class WoldMap_Node_Base : public std::enable_shared_from_this<WoldMap_Node_Base>
{
	public:
		WoldMap_Node_Base();				// コンストラクタ
		virtual ~WoldMap_Node_Base() {};	// デストラクタ

		/* 定数 */
		inline static const	std::string	NODE_TYPE_ENEMY	= "Enemy";		// 敵
		inline static const	std::string	NODE_TYPE_SHOP	= "Shop";		// ショップ
		inline static const	std::string	NODE_TYPE_BOSS	= "Boss";		// ボス

	protected:
		/* 変数 */
		std::string							NodeType;			// ノードの種類
		Struct_2D::POSITION					Position_Map;		// マップ上での座標
		std::vector<Struct_2D::POSITION>	Move_Position_List;	// 移動可能座標リスト
		int									Image;				// 画像
};
