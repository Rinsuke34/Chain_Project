/* データリスト"ゲームリソース管理"クラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

/* 前方宣言 */
class WoldMap_Node_Base;
class Card_Base;
class Character_Player;

// データリスト"ゲームリソース管理"クラス
class DataList_GameResource : public DataList_Base
{
	public:
		DataList_GameResource();				// コンストラクタ
		virtual ~DataList_GameResource() {};	// デストラクタ

		/* 関数 */
		void AddCoin(int coin);								// コインを追加
		void AddDropCard(std::shared_ptr<Card_Base> card);	// ドロップカードを追加

		/* ゲッター */
		int										GetGameState()			{ return GameState; }			// ゲームの状態
		bool									GetStageEndFlg()		{ return StageEndFlg; }			// ステージ終了フラグ
		bool									GetWoldMapActiveFlg()	{ return WoldMapActiveFlg; }	// ワールドマップが有効であるかのフラグ
		std::shared_ptr<WoldMap_Node_Base>		GetNowMapNode()			{ return NowMapNode; }			// 現在地点のノード
		bool									GetNextStageSelectFlg() { return NextStageSelectFlg; }	// 次のステージの選択が完了したかのフラグ
		bool									GetGameEndFlg()			{ return GameEndFlg; }			// ゲーム終了フラグ
		int										GetHaveCoin()			{ return HaveCoin; }			// 所持コイン
		std::vector<std::shared_ptr<Card_Base>>	GetDeckCardList()		{ return DeckCardList; }		// カードリスト
		std::shared_ptr<Character_Player>		GetPlayerCharacter()	{ return PlayerCharacter; }		// プレイヤーキャラクター
		std::vector<std::shared_ptr<Card_Base>>	GetDropCardList()		{ return DropCardList; }		// ドロップカードリスト
		bool									GetDropItemCheckFlg()	{ return DropItemCheckFlg; };	// ドロップアイテム確認シーンを有効化

		/* セッター */
		void SetGameState(int state)											{ GameState				= state; }		// ゲームの状態
		void SetStageEndFlg(bool flg)											{ StageEndFlg			= flg; }		// ステージ終了フラグ
		void SetWoldMapActiveFlg(bool flg)										{ WoldMapActiveFlg		= flg; }		// ワールドマップが有効であるかのフラグ
		void SetNowMapNode(std::shared_ptr<WoldMap_Node_Base> node)				{ NowMapNode			= node; }		// 現在地点のノード
		void SetNextStageSelectedFlg(bool flg)									{ NextStageSelectFlg	= flg; }		// 次のステージの選択が完了したかのフラグ
		void SetGameEndFlg(bool flg)											{ GameEndFlg			= flg; }		// ゲーム終了フラグ
		void SetHaveCoin(int coin)												{ HaveCoin				= coin; }		// 所持コイン
		void SetDeckCardList(std::vector<std::shared_ptr<Card_Base>> cardList)	{ DeckCardList			= cardList; }	// カードリスト
		void SetPlayerCharacter(std::shared_ptr<Character_Player> character)	{ PlayerCharacter		= character; }	// プレイヤーキャラクター
		void SetDropCardList(std::vector<std::shared_ptr<Card_Base>> cardList)	{ DropCardList			= cardList; }	// ドロップカードリスト
		void SetDropItemCheckFlg(bool flg)										{ DropItemCheckFlg		= flg; };		// ドロップアイテム確認シーンを有効化

		/* 定数 */
		// ゲームの状態
		static const int STATE_STAGE	= 0;	// ステージ処理
		static const int STATE_WORLDMAP	= 1;	// ワールドマップ

	private:
		/* 変数 */
		int										GameState;			// ゲームの状態
		bool									StageEndFlg;		// ステージ終了フラグ
		bool									WoldMapActiveFlg;	// ワールドマップが有効であるかのフラグ
		std::shared_ptr<WoldMap_Node_Base>		NowMapNode;			// 現在地点のノード
		bool									NextStageSelectFlg;	// 次のステージの選択が完了したかのフラグ
		bool									GameEndFlg;			// ゲーム終了フラグ
		int										HaveCoin;			// 所持コイン
		std::vector<std::shared_ptr<Card_Base>>	DeckCardList;		// カードリスト
		std::shared_ptr<Character_Player>		PlayerCharacter;	// プレイヤーキャラクター
		std::vector<std::shared_ptr<Card_Base>> DropCardList;		// ドロップカードリスト
		bool									DropItemCheckFlg;	// ドロップアイテム確認シーンを有効化
};
