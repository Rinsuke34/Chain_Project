/* "剣の加護"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Spell_BlessingSword.h"
// 関連クラス
#include "DataList_Battle.h"

// コンストラクタ
Card_Spell_BlessingSword::Card_Spell_BlessingSword() : Card_Spell_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity = RARITY_COMMON;
	// カード名
	this->Name = "剣の加護";
	// スートリスト
	this->Suite_List.push_back(SUITE_ELEMENT);	// エレメント
	// ステータス
	this->Strength = 0;					// 攻撃力
	this->Diffence = 0;					// 防御力
	this->AttackRange = ATTACKRANGE_FRONT;	// 攻撃範囲
	// 画像の名前
	this->ImageName = "BlessingSword";	// 画像の名前
	// 説明文
	this->ExplanationText = "スート：けん をもつカードをやまふだからXまいドロー\nXはチェイン+１";
}

// カード効果(特殊効果)
void Card_Spell_BlessingSword::Card_Effect_Extra_Process()
{
	/* 山札を取得 */
	auto DeckCardList = this->pDataList_Battle->GetDeckCardList();
	int DrawCardMax = this->iNowChainCount + 1;	// ドローするカードの枚数 = チェイン数 + 1

	/* スート：剣 をもつカードを山札から２まいドローする */
	int Drawn_Card_Count = 0;	// ドローしたカードの枚数
	for (auto& Card : DeckCardList)
	{
		if (Card->GetSuiteList().size() > 0 && Card->GetSuiteList()[0] == SUITE_SWORD)
		{
			// スート：たびびと をもつカードなら
			this->pDataList_Battle->AddHandCard(Card);		// 手札に加える
			this->pDataList_Battle->RemoveDeckCard(Card);	// 山札から削除する
			Drawn_Card_Count++;								// ドローしたカードの枚数を加算する
			if (Drawn_Card_Count >= DrawCardMax)
			{
				// X枚ドローしたらループを抜ける
				break;
			}
		}
	}
}
