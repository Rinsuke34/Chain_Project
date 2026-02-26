/* "旅人のコンパス"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Item_TravelerCompass.h"
// 関連クラス	
#include "DataList_Battle.h"
#include "Card_Base.h"

// コンストラクタ
Card_Item_TravelerCompass::Card_Item_TravelerCompass() : Card_Item_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity	= RARITY_RARE;
	// カード名
	this->Name		= "旅人のコンパス";
	// スートリスト
	this->Suite_List.push_back(SUITE_TRAVELER);	// 旅人
	// 画像の名前
	this->ImageName = "TravelerCompass";			// 画像の名前
	// 説明文
	this->ExplanationText = "/cysスート：たびびと/ceをもつカードをやまふだから２まいドロー\nてふだのスート：たびびと をもつカードのこうげきりょくを+Xする\nXはチェイン+１";
}

// カード効果(特殊効果)
void Card_Item_TravelerCompass::Card_Effect_Extra_Process()
{
	/* 山札を取得 */
	auto DeckCardList = this->pDataList_Battle->GetDeckCardList();

	/* スート：たびびと をもつカードを山札から２まいドローする */
	int Drawn_Card_Count = 0;	// ドローしたカードの枚数
	for (auto& Card : DeckCardList)
	{
		if (Card->CheckSute(SUITE_TRAVELER))
		{
			// スート：たびびと をもつカードなら
			this->pDataList_Battle->AddHandCard(Card);		// 手札に加える
			this->pDataList_Battle->RemoveDeckCard(Card);	// 山札から削除する
			Card->SetCardState(Card_Base::CARDSTATE_HAND);
			Drawn_Card_Count++;								// ドローしたカードの枚数を加算する
			if (Drawn_Card_Count >= 2)
			{
				// 2枚ドローしたらループを抜ける
				break;
			}
		}
	}

	/* 手札のスート：たびびと をもつカードにチェイン数分の攻撃バフを与える */
	for (int i = 0; i < this->pDataList_Battle->GetHandCardList().size(); i++)
	{
		auto HandCard = this->pDataList_Battle->GetHandCardList()[i];
		if (HandCard != nullptr && HandCard->CheckSute(SUITE_TRAVELER))
		{
			HandCard->Add_Strength_Buff(this->iNowChainCount + 1);	// 攻撃力バフを設定:チェイン数
			HandCard->UpdateImage();								// 画像を更新する
		}
	}
}
