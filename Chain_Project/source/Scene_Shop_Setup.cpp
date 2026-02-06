/* シーン"ショップ画面"の定義(ラインナップのセットアップ部分) */

/* 使用する要素のインクルード */
// ヘッダファイル
#include "Scene_Shop.h"
// 関連クラス
#include "Card_Include.h"

// ショップ設定
void Scene_Shop::ShopSetup(int Level)
{
	// 引数
	// Level <- ショップのラインナップの種類の判別に使用

	/* ラインナップ設定処理 */
	std::shared_ptr<Card_Base> AddCard = std::make_shared<Card_Arms_TravelerShield>();
	std::shared_ptr<Shop_Item> pShopItem = std::make_shared<Shop_Item>(AddCard, 5);
	this->ShopItemList.push_back(pShopItem);

	/* カードの初期設定 */
	for (auto& Item : this->ShopItemList)
	{
		std::shared_ptr<Card_Base> AddCard = Item->GetCard();

		/* 画像更新 */
		AddCard->UpdateImage();

		/* 陳列中状態に設定 */
		AddCard->SetCardState(Card_Base::CARDSTATE_SHOP_DISPLAY);
	}

	/* 描写座標の更新 */
	CardPosition_Setup();
}
