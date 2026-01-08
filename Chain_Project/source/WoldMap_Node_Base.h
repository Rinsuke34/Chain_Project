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

		/* 関数 */
		virtual void Update() {};	// 更新処理
		virtual void Draw();		// 描画処理
		virtual void AddMoveNode(std::shared_ptr<WoldMap_Node_Base> Node)	{ this->Move_Node_List.push_back(Node); };	// 移動可能ノードを追加

		/* セッター */
		void SetNodeType(int NodeType)															{ this->NodeType			= NodeType; }		// ノードタイプ
		void SetPosition_Map(Struct_2D::POSITION Position)										{ this->Position_Map		= Position; }		// マップ上での座標
		void SetPosition_Now(Struct_2D::POSITION Position)										{ this->Position_Now		= Position; }		// 画面上での座標
		void SetMoveNodeList(std::vector<std::shared_ptr<WoldMap_Node_Base>> PositionList)		{ this->Move_Node_List		= PositionList; }	// 移動可能ノードリスト
		void SetMoveNodePosList(std::vector<Struct_2D::POSITION> PositionList)					{ this->Move_Node_Pos_List	= PositionList; }	// 移動可能ノード座標リスト
		void SetImageIconSize(int Size)															{ this->Image_Icon_Size		= Size; }			// アイコン画像サイズ
		void SetNodeState(int State)															{ this->NodeState			= State; }			// ノード状態
		void SetNodeLevel(int NodeLevel)														{ this->NodeLevel			= NodeLevel; }		// ノードのレベル(内容に影響)

		/* ゲッター */
		int													GetNodeType()			{ return this->NodeType; }				// ノードタイプ
		Struct_2D::POSITION									GetPosition_Map()		{ return this->Position_Map; }			// マップ上での座標
		Struct_2D::POSITION									GetPosition_Now()		{ return this->Position_Now; }			// 画面上での座標
		std::vector<std::shared_ptr<WoldMap_Node_Base>>		GetMoveNodeList()		{ return this->Move_Node_List; }		// 移動可能ノードリスト
		std::vector<Struct_2D::POSITION>					GetMoveNodePosList()	{ return this->Move_Node_Pos_List; }	// 移動可能ノード座標リスト
		int													GetImageIconSize()		{ return this->Image_Icon_Size; }		// アイコン画像サイズ
		int 												GetNodeState()			{ return this->NodeState; }				// ノード状態
		int													GetNodeLevel()			{ return this->NodeLevel; }				// ノードのレベル(内容に影響)

		/* 定数 */
		// ノードの種類
		static const int	NODE_TYPE_ENEMY			= 0;	// 敵
		static const int	NODE_TYPE_SHOP			= 1;	// ショップ
		static const int	NODE_TYPE_BOSS			= 2;	// ボス
		// ノード状態
		static const int	NODE_STATE_NORMAL		= 0;	// 通常
		static const int	NODE_STATE_CLEARED		= 1;	// クリア済み
		static const int	NODE_STATE_PLAYER_POS	= 2;	// プレイヤー現在位置
		// アイコン画像インデックス
		static const int	ICON_INDEX_PLAYER_POS	= 0;	// プレイヤー現在位置
		static const int	ICON_INDEX_NODE_ICON	= 1;	// 固有のアイコン
		// アイコン画像サイズ
		static const int	WOLDMAP_ICON_SIZE		= 128;	// アイコン画像サイズ(ワールドマップ上)
		// フレーム関係
		static const int	FRAME_THICKNESS			= 16;	// 枠の太さ

	protected:
		/* 変数 */
		int													NodeType;			// ノードの種類
		Struct_2D::POSITION									Position_Map;		// マップ上での座標
		Struct_2D::POSITION									Position_Now;		// 画面上での中心座標
		std::vector<std::shared_ptr<WoldMap_Node_Base>>		Move_Node_List;		// 移動可能ノードリスト
		std::vector<Struct_2D::POSITION>					Move_Node_Pos_List;	// 移動可能ノード座標リスト
		int													Image_Icon_Size;	// アイコン画像サイズ
		int													NodeState;			// ノード状態
		int													NodeLevel;			// ノードのレベル(内容に影響)
		// 画像
		std::shared_ptr<int>	Image_Icon[2];		// アイコン画像[0:プレイヤー現在位置, 1:固有のアイコン]
		std::shared_ptr<int>	Image_Frame_Corner;	// 角
		std::shared_ptr<int>	Image_Frame_Line;	// 線
		std::shared_ptr<int>	Image_Frame_Inside;	// 内側
};
