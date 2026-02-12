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
	std::shared_ptr<Card_Base> AddCard		= nullptr;
	std::shared_ptr<Shop_Item> pShopItem	= nullptr;
	switch (Level)
	{
		case 1:
			AddCard		= std::make_shared<Card_Arms_TravelerSword>();
			pShopItem	= std::make_shared<Shop_Item>(AddCard, 10);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Arms_TravelerShield>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 10);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Item_HealingPotion>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 15);
			this->ShopItemList.push_back(pShopItem);

			AddCard = std::make_shared<Card_Arms_GrassSword>();
			pShopItem = std::make_shared<Shop_Item>(AddCard, 20);
			this->ShopItemList.push_back(pShopItem);
			break;

		case 10:
			break;

		case 11:
			break;

		case 20:
			break;
	}

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
