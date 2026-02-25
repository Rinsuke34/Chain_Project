/* "王国軍の軍旗"カードの定義 */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Card_Item_RoyalFlag.h"
// 関連クラス	
#include "DataList_Battle.h"
#include "Card_Base.h"

// コンストラクタ
Card_Item_RoyalFlag::Card_Item_RoyalFlag() : Card_Item_Base()
{
	/* カード情報の設定 */
	// レアリティ
	this->iRarity	= RARITY_RARE;
	// カード名
	this->Name		= "王国軍の軍旗";
	// スートリスト
	this->Suite_List.push_back(SUITE_ROYAL);	// 王国
	// 画像の名前
	this->ImageName = "RoyalFlag";				// 画像の名前
	// 説明文
	this->ExplanationText = "スート：おうこく をもつカードをすてふだからXまいてふだにくわえる/nXはチェインすう";
}

// カード効果(特殊効果)
void Card_Item_RoyalFlag::Card_Effect_Extra_Process()
{
	/* 捨て札エリアのカードを取得 */
	auto TrashCardList = this->pDataList_Battle->GetTrashCardList();

	/* スート：おうこく をもつカードを捨て札エリアからX枚まで手札に加える */
	int Added_Card_Count = 0;	// 加えたカードの枚数
	for (auto& Card : TrashCardList)
	{
		if (Card->CheckSute(SUITE_ROYAL))
		{
			// スート：おうこく をもつカードなら
			this->pDataList_Battle->AddHandCard(Card);		// 手札に加える
			this->pDataList_Battle->RemoveTrashCard(Card);	// 捨て札から削除する
			Card->SetCardState(Card_Base::CARDSTATE_HAND);
			Added_Card_Count++;								// 加えたカードの枚数を加算する

			if (Added_Card_Count >= this->iNowChainCount)
			{
				// チェイン数枚加えたらループを抜ける
				break;
			}
		}
	}
}
