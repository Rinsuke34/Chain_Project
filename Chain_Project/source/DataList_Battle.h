/* データリスト"バトル用データ管理"クラスの宣言 */

#pragma once

/* 使用する要素のインクルード */
// 共通定義
#include "AppFrame.h"

// 前方宣言
class Card_Base;

// データリスト"バトル用データ管理"クラス
class DataList_Battle : public DataList_Base
{
	public:
		DataList_Battle();				// コンストラクタ
		virtual ~DataList_Battle()	{};	// デストラクタ

		/* 関数 */
		void AddDeckCard(const std::shared_ptr<Card_Base>& pCard);		// デッキにカードを追加
		void AddHandCard(const std::shared_ptr<Card_Base>& pCard);		// 手札にカードを追加
		void AddTrashCard(const std::shared_ptr<Card_Base>& pCard);		// 捨て札にカードを追加
		void RemoveDeckCard(const std::shared_ptr<Card_Base>& pCard);	// デッキからカードを削除
		void RemoveHandCard(const std::shared_ptr<Card_Base>& pCard);	// 手札からカードを削除
		void RemoveTrashCard(const std::shared_ptr<Card_Base>& pCard);	// 捨て札からカードを削除
		void RemoveBattleAreaCard(int AreaNo);							// バトルエリアからカードを削除
		
		/* ゲッター */
		// カード関連
		std::vector<std::shared_ptr<Card_Base>>	GetDeckCardList()					{ return DeckCardList; }				// デッキのカード一覧を取得
		std::vector<std::shared_ptr<Card_Base>>	GetHandCardList()					{ return HandCardList; }				// 手札のカード一覧を取得
		std::vector<std::shared_ptr<Card_Base>>	GetTrashCardList()					{ return TrashCardList; }				// 捨て札のカード一覧を取得
		std::shared_ptr<Card_Base>				GetBattleAreaCardList(int areaNo)	{ return BattleAreaCardList[areaNo]; }	// バトルエリアのカード一覧を取得
		std::shared_ptr<Card_Base>				GetHoldCard()						{ return HoldCard; }					// ホールド中のカードを取得

		/* セッター */
		// カード関連
		void SetDeckCardList(const std::vector<std::shared_ptr<Card_Base>>& cardList)	{ DeckCardList					= cardList; }	// デッキのカード一覧を設定
		void SetHandCardList(const std::vector<std::shared_ptr<Card_Base>>& cardList)	{ HandCardList					= cardList; }	// 手札のカード一覧を設定
		void SetTrashCardList(const std::vector<std::shared_ptr<Card_Base>>& cardList)	{ TrashCardList					= cardList; }	// 捨て札のカード一覧を設定
		void SetBattleAreaCard(int areaNo, const std::shared_ptr<Card_Base>& card)		{ BattleAreaCardList[areaNo]	= card; }		// バトルエリアのカードを設定
		void SetHoldCard(const std::shared_ptr<Card_Base>& card)						{ HoldCard						= card; }		// ホールド中のカードを設定

		/* 定数 */
		// バトルエリア番号
		static const int BATTLE_AREA_1		= 0;	// バトルエリア番号:1
		static const int BATTLE_AREA_2		= 1;	// バトルエリア番号:2
		static const int BATTLE_AREA_3		= 2;	// バトルエリア番号:3
		static const int BATTLE_AREA_4		= 3;	// バトルエリア番号:4
		static const int BATTLE_AREA_5		= 4;	// バトルエリア番号:5
		static const int BATTLE_AREA_MAX	= 5;	// バトルエリア番号総数

	private:
		/* 変数 */
		// カード関連
		std::vector<std::shared_ptr<Card_Base>> DeckCardList;							// デッキのカード一覧
		std::vector<std::shared_ptr<Card_Base>> HandCardList;							// 手札のカード一覧
		std::vector<std::shared_ptr<Card_Base>> TrashCardList;							// 捨て札のカード一覧
		std::shared_ptr<Card_Base>				BattleAreaCardList[BATTLE_AREA_MAX];	// バトルエリアのカード一覧
		std::shared_ptr<Card_Base>				HoldCard;								// ホールド中のカード

		/* 関数 */
};
