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
		virtual void Update();		// 更新処理
		virtual void Draw();		// 描画処理
		virtual void AddMovePosition(Struct_2D::POSITION Position)	{ this->Move_Position_List.push_back(Position); };	// 移動可能座標を追加

		/* セッター */
		void SetNodeType(std::string NodeType)									{ this->NodeType			= NodeType; }		// ノードタイプ
		void SetPosition_Map(Struct_2D::POSITION Position)						{ this->Position_Map		= Position; }		// マップ上での座標
		void SetPosition_Now(Struct_2D::POSITION Position)						{ this->Position_Now		= Position; }		// 画面上での座標
		void SetMovePositionList(std::vector<Struct_2D::POSITION> PositionList)	{ this->Move_Position_List	= PositionList; }	// 移動可能座標リスト
		void SetImageIconSize(int Size)											{ this->Image_Icon_Size		= Size; }			// アイコン画像サイズ

		/* ゲッター */
		std::string	GetNodeType()								{ return this->NodeType; }				// ノードタイプ
		Struct_2D::POSITION GetPosition_Map()					{ return this->Position_Map; }			// マップ上での座標
		Struct_2D::POSITION GetPosition_Now()					{ return this->Position_Now; }			// 画面上での座標
		std::vector<Struct_2D::POSITION> GetMovePositionList()	{ return this->Move_Position_List; }	// 移動可能座標リスト

		/* 定数 */
		// ノードの種類
		inline static const	std::string	NODE_TYPE_ENEMY	= "Enemy";		// 敵
		inline static const	std::string	NODE_TYPE_SHOP	= "Shop";		// ショップ
		inline static const	std::string	NODE_TYPE_BOSS	= "Boss";		// ボス
		// アイコン画像インデックス
		static const int	ICON_INDEX_PLAYER_POS	= 0;	// プレイヤー現在位置
		static const int	ICON_INDEX_NODE_ICON	= 1;	// 固有のアイコン
		// アイコン画像サイズ
		static const int	WOLDMAP_ICON_SIZE		= 128;	// アイコン画像サイズ(ワールドマップ上)
		// フレーム関係
		static const int	FRAME_THICKNESS			= 16;	// 枠の太さ

	protected:
		/* 変数 */
		std::string							NodeType;			// ノードの種類
		Struct_2D::POSITION					Position_Map;		// マップ上での座標
		Struct_2D::POSITION					Position_Now;		// 画面上での中心座標
		std::vector<Struct_2D::POSITION>	Move_Position_List;	// 移動可能座標リスト
		int									Image_Icon_Size;	// アイコン画像サイズ
		// 画像
		std::shared_ptr<int>	Image_Icon[2];		// アイコン画像[0:プレイヤー現在位置, 1:固有のアイコン]
		std::shared_ptr<int>	Image_Frame_Corner;	// 角
		std::shared_ptr<int>	Image_Frame_Line;	// 線
		std::shared_ptr<int>	Image_Frame_Inside;	// 内側
};
